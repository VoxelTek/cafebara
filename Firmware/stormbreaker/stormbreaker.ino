#include <BitBang_I2C.h>

#include <Wire.h>
#include <twi.h>
#include <twi_pins.h>

#include <tinyNeoPixel.h>

#include <EEPROM.h>

#define CHRG_STAT PIN_PC0

#define SDA2 PIN_PC2
#define SCL2 PIN_PC3

#define FAN PIN_PA4
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

byte ver = EEPROM.read(ADDR_VER);

byte* requestedReg;

bool isRequesting = false; // Has the Wii sent a byte to request data?

BBI2C bbi2c;

bool isPowered = false;
bool isCharging = false;

bool buttonTriggered = false;
bool triggeredSoftShutdown = false;

byte battVolt = 0b1000101; //~3.7V
byte minBattVolt = 0b0010011; //~2.7

byte battVoltLevels[5] = {0b01011111, 0b1000101, 0b0100010, 0b0011000, 0b0010011}; // 4.2, 3.7, 3.0, 2.8, 2.7

byte pwrErrorStatus = 0x00;

byte chargeStatus = 0b00;

uint8_t bqAddr = 0x6A;

byte maxCurrent = 0x3F; // 3.25A
//byte chrgCurrent = 0b1000010; // 4224mA
byte chrgCurrent = EEPROM.read(ADDR_CHRGCURRENT);
byte preCurrent = EEPROM.read(ADDR_PRECURRENT);
byte termCurrent = EEPROM.read(ADDR_TERMCURRENT);
byte chrgVoltage = EEPROM.read(ADDR_CHRGVOLTAGE);

const bool ilimEnabled = false;



tinyNeoPixel pixels = tinyNeoPixel(NUMPIXELS, LED, NEO_GRB + NEO_KHZ800);



int I2CWriteRegister(BBI2C *pI2C, unsigned char iAddr, unsigned char reg, unsigned char value) {
    unsigned char buffer[2];
    buffer[0] = reg;   // Register address
    buffer[1] = value; // Data to write

    return I2CWrite(pI2C, iAddr, buffer, 2);
}



void setup() {
  pinMode(BUTTON, INPUT_PULLUP);
  pinMode(CHRG_STAT, INPUT);
  pinMode(TEMP_ALERT, INPUT_PULLUP);

  pinMode(SOFT_PWR, OUTPUT);
  digitalWrite(SOFT_PWR, LOW);
  pinMode(SOFT_SHUT, INPUT);

  pinMode(FAN, OUTPUT);

  pinMode(PWR_ON, OUTPUT);
  digitalWrite(PWR_ON, HIGH); // Makes sure console is off

  attachInterrupt(digitalPinToInterrupt(BUTTON), powerButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(SOFT_SHUT), softShutdown, RISING);
  attachInterrupt(digitalPinToInterrupt(CHRG_STAT), chargingStatus, CHANGE);
  attachInterrupt(digitalPinToInterrupt(TEMP_ALERT), overTemp, FALLING);



  memset(&bbi2c, 0, sizeof(bbi2c));
  bbi2c.bWire = false;
  bbi2c.iSDA = SDA2;
  bbi2c.iSCL = SCL2;
  I2CInit(&bbi2c, 100000L);

  Wire.onReceive(receiveDataWire);
  Wire.onRequest(transmitDataWire);
  Wire.begin(STORM_I2C);

  pixels.begin();

  setupBQ();
}

void loop() {
  if (isPowered) {
    monitorBatt();
    delay(1500);
  }
  if (isCharging) {
    chargingStatus();
    delay(1500);
  }
}

void overTemp() {
  triggerShutdown();
  powerLED(5);
  enableShipping();
}

void powerButton() {
  if (!buttonTriggered) {
    buttonTriggered = true; // Make sure other instances of this function don't run. They shouldn't anyway, but eh.
    delay(250); // Wait in order to prevent debouncing
    if (digitalRead(BUTTON) == LOW) {
      if (isPowered) {
        triggerShutdown(); // Is it on? Turn it off.
      }
      else {
        getBattVoltage();
        delay(200);
        if ((battVolt > minBattVolt) || isCharging) {
          consoleOn(); // Voltage is high enough or currently charging, turn on console
        }
        else {
          powerLED(5); // Flash light, battery too low
        }
      }
    }
    while (digitalRead(BUTTON) == LOW) { // Wait for button to stop being pressed
      delay(100);
    }
    delay(250); // Debounce protection
    buttonTriggered = false;
  }
}

void triggerShutdown() {
  if (!triggeredSoftShutdown) {
    digitalWrite(SOFT_PWR, HIGH); // Trigger the soft shutdown sequence on the Wii
    delay(30);
    digitalWrite(SOFT_PWR, LOW);
    triggeredSoftShutdown = true;
    delay(2500);
    if (isPowered && triggeredSoftShutdown) { // Wii did not shut itself down safely, time out and force a power-off
      consoleOff();
      triggeredSoftShutdown = false;
    }
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
  if (pwrErrorStatus != 0x00) {
    triggerShutdown();
    powerLED(5);
    if (pwrErrorStatus & 0b00101000) {
      enableShipping();
      return;
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
      setLED(0, 255, 0, 128, true);
    break;

    case 2:
      setLED(255, 255, 0, 128, true);
    break;

    case 3:
      setLED(255, 127, 0, 128, true);
    break;

    case 4:
      setLED(255, 0, 0, 128, true);
    break;

    case 5:
      for (int i = 0; i < 5; i++) {
        setLED(255, 0, 0, 128, true);
        delay(100);
        setLED(0, 0, 0, 0, false);
        delay(100);
      }
      powerLED(0);
    break;

    case 6:
      setLED(0, 0, 255, 64, true);
    break;

    case 7:
      setLED(255, 183, 197, 64, true);
    break;

    default:
      powerLED(0);
    break;
  }
}

void consoleOn() {
  /*REG02*/
  unsigned char reg02 = (0b11111100); // Disable D+/D- detection and such
  // Continuous battery voltage reading if console is on
  I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);

  digitalWrite(PWR_ON, LOW);
  isPowered = true;

  monitorBatt();
}

void consoleOff() {
  /*REG02*/
  unsigned char reg02 = (0b00111100); // Disable D+/D- detection and such
  // Disable continuous battery voltage reading
  I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);

  digitalWrite(PWR_ON, HIGH);
  isPowered = false;

  monitorBatt();
}

void enableShipping() {
  /*REG09*/
  unsigned char reg09 = 0b00100000; // Enable shipping mode
  I2CWriteRegister(&bbi2c, bqAddr, 0x09, reg09);
}

void setupBQ() {
  /*REG00*/
  unsigned char reg00 = ((maxCurrent) | (ilimEnabled << 6)); // Set max current and ILIM disabled.
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
  unsigned char reg05 = ((preCurrent << 4) | termCurrent); // Set term charge current
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

  if (!Wire.available()) {
    isRequesting = true;

    switch (reg) {
      case 0x00: // Get version
        requestedReg = &ver;
      break;

      case 0x04:
        //fan
      break;

      case 0x0B:
        enableShipping();
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

      case 0x26:
        requestedReg = &battVolt;
      break;
    }
  }
}

void transmitDataWire() {

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

void monitorBatt() {
  getBattVoltage();
  delay(100);
  if (isCharging || !isPowered) { // If not turned on, or charging, let the chargingStatus function handle it
    powerLED(0);
    chargingStatus();
    return;
  }
  if (battVolt < minBattVolt) {
    triggerShutdown(); // Battery too low, emergency shutdown
    powerLED(5);
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