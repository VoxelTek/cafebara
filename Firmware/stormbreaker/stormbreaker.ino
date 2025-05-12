#include <BitBang_I2C.h>

#include <Wire.h>
#include <twi.h>
#include <twi_pins.h>

#include <tinyNeoPixel.h>

#include <EEPROM.h>

#include <avr/sleep.h>

#define CHRG_STAT PIN_PC0

#define SDA2 PIN_PC2
#define SCL2 PIN_PC3

#define FAN PIN_PB2

#define LED PIN_PA5
#define NUMPIXELS 2

#define PWR_ON PIN_PA1 // Output
#define BUTTON PIN_PA2

#define TEMP_ALERT PIN_PA7

#define SOFT_PWR PIN_PB3 //Out to Wii
#define SOFT_SHUT PIN_PB4 //In from Wii

#define STORM_I2C 0x50

#define ADDR_VER 0x00

#define ADDR_CHRGCURRENT 0x01
#define ADDR_TERMCURRENT 0x02
#define ADDR_PRECURRENT 0x03
#define ADDR_CHRGVOLTAGE 0x04
#define ADDR_FANSPEED 0x05

byte ver = 0x01; // v0.1 (ver / 10)

byte* requestedReg;

bool isRequesting = false; // Has the Wii sent a byte to request data?

BBI2C bbi2c;

bool isPowered = false; // Is the Wii powered?
bool isCharging = false; // Is the BQ charging the batteries?

bool buttonTriggered = false; // Has the button been triggered?
bool triggeredSoftShutdown = false; // Did we trigger the soft shutdown?

bool isOverTemp = false;

byte battCharge;

byte battVolt = 0b1000101; //~3.7V
const byte minBattVolt = 0b0010011; //~2.7V

byte battVoltLevels[5] = {0b01011111, 0b1000101, 0b0100010, 0b0011000, 0b0010011}; // 4.2, 3.7, 3.0, 2.8, 2.7

byte battChrgLevels[9] = {minBattVolt, 0x1c, 0x26, 0x30, 0x39, 0x43, 0x4c, 0x56, 0x5f};

byte pwrErrorStatus = 0x00;

byte chargeStatus = 0b00;

const uint8_t bqAddr = 0x6A;

byte maxCurrent = 0x3F; // 3.25A
//byte chrgCurrent = 0b1000010; // 4224mA

/*
byte chrgCurrent = EEPROM.read(ADDR_CHRGCURRENT);
byte preCurrent = EEPROM.read(ADDR_PRECURRENT);
byte termCurrent = EEPROM.read(ADDR_TERMCURRENT);
byte chrgVoltage = EEPROM.read(ADDR_CHRGVOLTAGE);
*/

byte chrgCurrent;
byte preCurrent;
byte termCurrent;
byte chrgVoltage;
byte fanSpeed;

const bool ilimEnabled = false;



tinyNeoPixel pixels = tinyNeoPixel(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);



int I2CWriteRegister(BBI2C *pI2C, unsigned char iAddr, unsigned char reg, unsigned char value) {
    unsigned char buffer[2];
    buffer[0] = reg;   // Register address
    buffer[1] = value; // Data to write

    return I2CWrite(pI2C, iAddr, buffer, 2);
}


void firstTimeCheck() {
  if (EEPROM.read(ADDR_VER) == 0) { // Check if there's no data in the EEPROM
    chrgCurrent = 0b1000010; // 4224mA
    preCurrent = 0b0001;
    termCurrent = 0b0011;
    chrgVoltage = 0b010111;
    fanSpeed = 0xFF;
    writeToEEPROM(); 
  }
  else {
    chrgCurrent = EEPROM.read(ADDR_CHRGCURRENT);
    preCurrent = EEPROM.read(ADDR_PRECURRENT);
    termCurrent = EEPROM.read(ADDR_TERMCURRENT);
    chrgVoltage = EEPROM.read(ADDR_CHRGVOLTAGE);
    fanSpeed = EEPROM.read(ADDR_FANSPEED);
  }
  EEPROM.write(ADDR_VER, ver);
}


void setup() {
  ADCPowerOptions(ADC_DISABLE); // We don't need the ADC, turn it off
  set_sleep_mode(SLEEP_MODE_PWR_DOWN); // Set sleep to low power mode
  sleep_enable(); // Enable sleeping, don't activate sleep yet though

  // Set unused pins to outputs
  pinMode(PIN_PA3, OUTPUT);
  pinMode(PIN_PA4, OUTPUT);
  pinMode(PIN_PA6, OUTPUT);
  pinMode(PIN_PB5, OUTPUT);
  pinMode(PIN_PC1, OUTPUT);

  firstTimeCheck(); // Get settings from EEPROM

  //battChrgLevels = {chrgVoltage, 0x56, 0x4c, 0x43, 0x39, 0x30, 0x26, 0x1c};

  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(CHRG_STAT, INPUT);
  pinMode(TEMP_ALERT, INPUT_PULLUP);

  pinMode(SOFT_PWR, OUTPUT);
  digitalWrite(SOFT_PWR, LOW);
  pinMode(SOFT_SHUT, INPUT);

  pinMode(FAN, OUTPUT);
  digitalWrite(FAN, LOW);

  pinMode(PWR_ON, OUTPUT);
  digitalWrite(PWR_ON, HIGH); // Makes sure console is off

  attachInterrupt(digitalPinToInterrupt(BUTTON), powerButton, LOW);
  attachInterrupt(digitalPinToInterrupt(SOFT_SHUT), softShutdown, RISING);
  attachInterrupt(digitalPinToInterrupt(CHRG_STAT), chargingStatus, CHANGE);
  attachInterrupt(digitalPinToInterrupt(TEMP_ALERT), overTemp, LOW);
  
  setFan(false); // Turn off fan


  memset(&bbi2c, 0, sizeof(bbi2c));
  bbi2c.bWire = false;
  bbi2c.iSDA = SDA2;
  bbi2c.iSCL = SCL2;
  I2CInit(&bbi2c, 0xFFFF);

  Wire.onReceive(receiveDataWire);
  Wire.onRequest(transmitDataWire);
  Wire.begin(STORM_I2C);

  pixels.begin();

  setupBQ();
}

void loop() {
  if (isPowered) {
    monitorBatt();
    delay(500);
  }
  else if (isCharging) {
    chargingStatus();
    delay(500);
  }
  else {
    sleep_cpu(); // The console isn't on, nor is it charging. Enter sleep to save power.
  }
}

void overTemp() {
  triggerShutdown(); // Trigger shutdown process
  analogWrite(FAN, 0xff); // fan at max speed, keep console cool
  powerLED(5);
  isOverTemp = true;
  for (int i = 0; i < 120; i++) { // 2 min to cool down
    delay(1000);
  }
  isOverTemp = false;
  setFan(false); // disable cooling fan
}

void powerButton() { // Power button has been pressed
  if (!buttonTriggered && digitalRead(BUTTON) == LOW) {
    buttonTriggered = true; // Make sure other instances of this function don't run. They shouldn't, hopefully, but eh.
    delay(1000); // Wait in order to prevent bouncing and accidental presses
    if (digitalRead(BUTTON) == LOW) {
      if (isPowered) {
        triggerShutdown(); // Is it on? Turn it off.
      }
      else {
        getBattVoltage();
        if (((battVolt > minBattVolt) || isCharging) && !isOverTemp && (pwrErrorStatus == 0x00)) { 
        // Check that either the battery is charged enough, or console is charging, 
        // AND make sure there's no over-temp issues
        // AND make sure there's no power errors
          consoleOn(); // Voltage is high enough or currently charging, turn on console
        }
        else {
          powerLED(5); // Flash red light, battery too low OR over temp
        }
      }
    }
    while (digitalRead(BUTTON) == LOW) { // Wait for button to stop being pressed
      delay(100);
    }
    delay(250); // Debounce protection...kinda
    buttonTriggered = false;
  }
}

void triggerShutdown() {
  if (!triggeredSoftShutdown && isPowered) {
    digitalWrite(SOFT_PWR, HIGH); // Trigger the soft shutdown sequence on the Wii
    delay(30);
    digitalWrite(SOFT_PWR, LOW);
    triggeredSoftShutdown = true;
    delay(2500);
    if (isPowered && triggeredSoftShutdown) { // Wii did not shut itself down safely, timeout and force a power-off
      consoleOff();
      triggeredSoftShutdown = false;
    }
  }
  else if (!isPowered) {
    consoleOff(); // If some bug happens, and the console is on while this thinks it isn't, might as well make sure it's shut down.
  }
}

void softShutdown() {
  if (digitalRead(SOFT_SHUT) == HIGH) { // Check that the pin is actually high
    delay(100);
    consoleOff();
    triggeredSoftShutdown = false;
  }
}

void chargingStatus() {
  unsigned char chrgStat = 0x00;
  I2CReadRegister(&bbi2c, bqAddr, 0x0B, &chrgStat, 0x8);
  chargeStatus = ((chrgStat & 0b00011000) >> 3);

  unsigned char errorStat = 0x00;
  I2CReadRegister(&bbi2c, bqAddr, 0x0C, &errorStat, 0x8);
  pwrErrorStatus = errorStat;
  if (pwrErrorStatus != 0x00) { // Uh oh, *something* is wrong
    triggerShutdown();
    if (pwrErrorStatus & 0b00100000) { // oh jeez stuff is hot this is really bad
      overTemp();
      return;
    }
    else if (pwrErrorStatus & 0b00001000) { // ???? the battery is TOO charged????
      enableShipping();
    }
  }

  if (chargeStatus == 0b00) {
    isCharging = false;
    if (isPowered) {
      monitorBatt();
    }
    else {
      powerLED(0); // Not on, not charging

    }
  }
  else {
    isCharging = true;
    if (chargeStatus == 0b11) { // Finished charging
      powerLED(7);
    }
    else {
      powerLED(6); // Charging, not complete
    }
  }
  delay(100);
}

void setFan(bool active) {
  if (active) {
    analogWrite(FAN, fanSpeed);
  }
  else {
    digitalWrite(FAN, LOW);
  }
}


void powerLED(uint8_t mode) {
  /*
  0 = All LEDs off
  1 = Full charge -- Green
  2 = Medium charge -- Yellow
  3 = Low charge -- Orange
  4 = About to run out -- Red
  5 = Low-power shutdown, or other error -- Couple red flashes
  6 = Charging -- Soft blue
  7 = Charging, full -- Soft pink
  */
  switch (mode) {
    case 0:
      setLED(0, 0, 0, 0, false);
    break;

    case 1:
      setLED(0, 255, 0, 128, true); // Green
    break;

    case 2:
      setLED(255, 255, 0, 128, true); // Yellow
    break;

    case 3:
      setLED(255, 128, 0, 128, true); // Orange
    break;

    case 4:
      setLED(255, 0, 0, 128, true); // Red
    break;

    case 5:
      for (int i = 0; i < 5; i++) { // Flash red
        setLED(255, 0, 0, 128, true);
        delay(100);
        setLED(0, 0, 0, 0, false);
        delay(100);
      }
      powerLED(0);
    break;

    case 6:
      setLED(0, 0, 255, 64, true); // Dim blue
    break;

    case 7:
      setLED(255, 183, 197, 64, true); // Dim sakura pink
    break;

    default:
      powerLED(0); // off
    break;
  }
}

void consoleOn() {
  /*REG02*/
  unsigned char reg02 = (0b11111100); // Disable D+/D- detection and such
  // Continuous battery voltage reading if console is on
  I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);

  digitalWrite(PWR_ON, LOW); // Activate MOSFET
  isPowered = true;

  setFan(true); // Enable fan,

  monitorBatt();
}

void consoleOff() {
  /*REG02*/
  unsigned char reg02 = (0b00111100); // Disable D+/D- detection and such
  // Disable continuous battery voltage reading
  I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);

  digitalWrite(PWR_ON, HIGH); // Deactivate MOSFET
  isPowered = false;

  setFan(false);

  monitorBatt();
}

void enableShipping() {
  /*REG09*/
  unsigned char reg09 = 0b00100000; // Enable shipping mode
  I2CWriteRegister(&bbi2c, bqAddr, 0x09, reg09);
}

void setupBQ() {
  /*REG00*/
  unsigned char reg00 = ((maxCurrent) | (ilimEnabled << 6)); // Set max current and ILIM.
  I2CWriteRegister(&bbi2c, bqAddr, 0x00, reg00);

  /*REG02*/
  unsigned char reg02 = (0b00111100); // Disable D+/D- detection and such
  if (isPowered){
    reg02 |= 0b11000000; // Continuous battery voltage reading if console is on
  }
  I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);

  /*REG03*/
  unsigned char reg03 = (0b00011010); // Disable OTG
  I2CWriteRegister(&bbi2c, bqAddr, 0x03, reg03);

  /*REG04*/
  unsigned char reg04 = (0x00 | chrgCurrent); // Set fast charge current
  I2CWriteRegister(&bbi2c, bqAddr, 0x04, reg04);

  /*REG05*/
  unsigned char reg05 = ((preCurrent << 4) | (0b1111 & termCurrent)); // Set term charge current
  I2CWriteRegister(&bbi2c, bqAddr, 0x05, reg05);

  /*REG06*/
  unsigned char reg06 = (0b00000011 | (chrgVoltage << 2)); // Set max battery voltage
  I2CWriteRegister(&bbi2c, bqAddr, 0x06, reg06);
}

void writeToEEPROM() {
  EEPROM.write(ADDR_CHRGCURRENT, chrgCurrent);
  EEPROM.write(ADDR_PRECURRENT, preCurrent);
  EEPROM.write(ADDR_TERMCURRENT, termCurrent);
  EEPROM.write(ADDR_CHRGVOLTAGE, chrgVoltage);
  EEPROM.write(ADDR_FANSPEED, fanSpeed);
}

void applyChanges() {
  writeToEEPROM();
  setFan(isPowered);
  setupBQ();
}


void getBattVoltage() {
  if (!isPowered) {
    unsigned char reg02 = (0b10111100); // Disable D+/D- detection and such
    // Get single voltage reading
    I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);
    delay(200);
  }
  unsigned char adcRegStatus = 0;
  I2CReadRegister(&bbi2c, bqAddr, 0x0E, &adcRegStatus, 0x8);
  adcRegStatus &= 0b1111111;
  battVolt = adcRegStatus;
}



void receiveDataWire(int16_t numBytes) {
  if (numBytes == 0) return;

  byte reg = Wire.read();

  switch (reg) {
    case 0x00: // Get version
      requestedReg = &ver;
    break;

    case 0x02:
      applyChanges(); // Apply and store current settings
      isRequesting = false;
      Wire.read();
      return;
    break;

    case 0x04:
      requestedReg = &fanSpeed;
    break;

    case 0x0B:
      enableShipping(); // Don't need to bother doing anything, we'll be losing power soon anyway
      delay(1000);
    break;

    case 0x10:
      requestedReg = &chrgCurrent;
    break;

    case 0x11:
      requestedReg = &termCurrent;
    break;

    case 0x12:
      requestedReg = &preCurrent;
    break;

    case 0x13:
      requestedReg = &chrgVoltage;
    break;

    case 0x15:
      requestedReg = &chargeStatus;
    break;

    case 0x24:
      battChargeStatus();
      requestedReg = &battCharge;
    break;

    case 0x26:
      getBattVoltage();
      requestedReg = &battVolt;
    break;
  }

  if (!Wire.available()) {
    isRequesting = true;
    return;
  }
  else {
    isRequesting = false;
    *requestedReg = Wire.read();
  }
}

void transmitDataWire() {
  if (!isRequesting) {
    return;
  }
  Wire.write(*requestedReg);
  isRequesting = false;
}

void setLED(uint8_t r, uint8_t g, uint8_t b, uint8_t bright, bool enabled) {
  if (!enabled) {
    pixels.clear();
  }
  else {
    for (int i = 0; i < NUMPIXELS; i++) {

      // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
      pixels.setPixelColor(i, pixels.Color(r, g, b));
      pixels.setBrightness(bright);

      pixels.show(); // This sends the updated pixel color to the hardware.

      delay(50); // Delay for a period of time (in milliseconds).

    }
  }
}

void battChargeStatus() {
  if (chargeStatus == 0b11) {
    battCharge = 0xff;
    return;
  }
  getBattVoltage();
  delay(100);
  
  float tmp;

  for (int i = 0; i < 8; i++) {
    if ((battVolt >= battChrgLevels[i]) && (battVolt < battChrgLevels[i+1])) {
      tmp = i + ((battVolt - battChrgLevels[i])/ (battChrgLevels[i+1] - battChrgLevels[i]));
      tmp *= 0x20;
      battCharge = int(tmp);
      return;
    }
  }
}

void monitorBatt() {
  getBattVoltage();
  delay(100);
  if (isCharging || !isPowered) { 
    // If not turned on, or if charging, let the chargingStatus function handle it
    powerLED(0);
    chargingStatus();
    return;
  }
  if (battVolt < minBattVolt) {
    triggerShutdown(); // Battery too low, emergency shutdown
    powerLED(5); // Show flashing red for error
    return;
  }
  if ((battVolt <= battVoltLevels[0]) && (battVolt > battVoltLevels[1])) {
    powerLED(1); // High charge
  }
  else if ((battVolt <= battVoltLevels[1]) && (battVolt > battVoltLevels[2])) {
    powerLED(2); // Medium charge
  }
  else if ((battVolt <= battVoltLevels[2]) && (battVolt > battVoltLevels[3])) {
    powerLED(3); // Low charge
  }
  else if ((battVolt <= battVoltLevels[3]) && (battVolt > battVoltLevels[4])) {
    powerLED(4); // ABOUT TO RUN OUT
  }
}