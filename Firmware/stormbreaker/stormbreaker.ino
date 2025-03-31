#include <BitBang_I2C.h>

#include <Wire.h>
#include <twi.h>
#include <twi_pins.h>

#include <tinyNeoPixel.h>

#define CHRG_STAT PIN_PC0

#define SDA2 PIN_PC2
#define SCL2 PIN_PC3

#define FAN PIN_PA4
#define LED PIN_PA5
#define NUMPIXELS 2

#define PWR_ON PIN_PA1

#define BUTTON PIN_PA2


#define SOFT_PWR PIN_PB3 //Out to Wii
#define SOFT_SHUT PIN_PB4 //In from Wii

BBI2C bbi2c;

bool isPowered = false;
bool isCharging = false;

bool buttonTriggered = false;
bool triggeredSoftShutdown = false;

float battVolt = 3.7;
float minBattVolt = 2.7;

float battVoltLevels[5] = {4.2, 3.7, 3.0, 2.8, 2.7};

bool powerError = false;

byte chargeStatus = 0b00;

uint8_t bqAddr = 0x6A;

byte maxCurrent = 0x3F; // 3.25A
byte chrgCurrent = 0b1000010; // 4224mA

bool ilimEnabled = false;

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

  pinMode(SOFT_PWR, OUTPUT);
  digitalWrite(SOFT_PWR, LOW);
  pinMode(SOFT_SHUT, INPUT);

  pinMode(FAN, OUTPUT);

  pinMode(PWR_ON, OUTPUT);
  digitalWrite(PWR_ON, HIGH);

  attachInterrupt(digitalPinToInterrupt(BUTTON), powerButton, FALLING);
  attachInterrupt(digitalPinToInterrupt(SOFT_SHUT), softShutdown, RISING);
  attachInterrupt(digitalPinToInterrupt(CHRG_STAT), chargingStatus, CHANGE);



  memset(&bbi2c, 0, sizeof(bbi2c));
  bbi2c.bWire = false;
  bbi2c.iSDA = SDA2;
  bbi2c.iSCL = SCL2;
  I2CInit(&bbi2c, 100000L);

  Wire.onReceive(receiveDataWire);
  Wire.onRequest(transmitDataWire);
  Wire.begin(0x54);

  pixels.begin();

  setupBQ();
}

void loop() {
  if (isPowered) {
    monitorBatt();
  }
  if (isCharging) {
    chargingStatus();
  }
}

void powerButton() {
  if (!buttonTriggered) {
    buttonTriggered = true;
    delay(200);
    if (digitalRead(BUTTON) == LOW) {
      if (isPowered) {
        triggerShutdown();        
      }
      else {
        getBattVoltage();
        delay(200);
        if ((battVolt > minBattVolt) || isCharging) {
          consoleOn();
        }
        else {
          powerLED(5);
        }
      }
    }
    while (digitalRead(BUTTON) == LOW) {
      delay(200);
    }
    delay(200);
    buttonTriggered = false;
  }
}

void triggerShutdown() {
  if (!triggeredSoftShutdown) {
    digitalWrite(SOFT_PWR, HIGH);
    delay(30);
    digitalWrite(SOFT_PWR, LOW);
    triggeredSoftShutdown = true;
    delay(2500);
    if (isPowered) { // Wii did not shut itself down safely, time out and force a power-off
      consoleOff();
      triggeredSoftShutdown = false;
    }
  }
}

void softShutdown() {
  if (triggeredSoftShutdown) {
    delay(100);
    consoleOff();
    triggeredSoftShutdown = false;
  }
}

void chargingStatus() {
  unsigned char chrgStat = 0x00;
  I2CReadRegister(&bbi2c, bqAddr, 0x0B, &chrgStat, 0x8);
  chargeStatus = ((chrgStat & 0b00011000) >> 3);

  if (chargeStatus == 0b00) {
    isCharging = false;
    if (isPowered) {
      monitorBatt();
    }
    else {
      powerLED(0);
    }
  }
  else {
    isCharging = true;
    if (chargeStatus == 0b11) {
      powerLED(7);
    }
    else {
      powerLED(6);
    }
  }
  delay(500);
}


void powerLED(uint8_t mode) {
  /*
  0 = All LEDs off
  1 = Full charge -- Green?
  2 = Medium charge -- Yellow?
  3 = Low charge -- Orange?
  4 = About to run out -- Fast-breathing red?
  5 = Low-power shutdown -- Couple red flashes
  6 = Charging -- Slow-breathing blue 
  7 = Charging, full -- Soft white/pink?
  */

}

void consoleOn() {
  /*REG02*/
  unsigned char reg02 = 0x00;
  I2CReadRegister(&bbi2c, bqAddr, 0x02, &reg02, 0x8);
  reg02 = (reg02 & 0b00111111) | 0b11000000; // Enable constant ADC reading
  I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);

  digitalWrite(PWR_ON, LOW);
  isPowered = true;

  monitorBatt();
}

void consoleOff() {
  /*REG02*/
  unsigned char reg02 = 0x00;
  I2CReadRegister(&bbi2c, bqAddr, 0x02, &reg02, 0x8);
  reg02 = (reg02 & 0b00111111); // Disable constant ADC reading
  I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);

  digitalWrite(PWR_ON, HIGH);
  isPowered = false;

  if (!isCharging) {
    powerLED(0);
  }
}

void enableShipping() {
  /*REG09*/
  unsigned char reg09 = 0x00;
  I2CReadRegister(&bbi2c, bqAddr, 0x09, &reg09, 0x8);
  reg09 = (reg09 & 0b11011111) | 0b00100000; // Enable shipping mode
  I2CWriteRegister(&bbi2c, bqAddr, 0x09, reg09);
}

void setupBQ() {
  /*REG00*/
  unsigned char reg00 = 0x00;
  reg00 = (maxCurrent) | (ilimEnabled << 6); // Set max current and ILIM enabled.
  I2CWriteRegister(&bbi2c, bqAddr, 0x00, reg00);

  /*REG02*/
  unsigned char reg02 = 0x00;
  I2CReadRegister(&bbi2c, bqAddr, 0x02, &reg02, 0x8); // Grab existing values to write back
  reg02 = (reg02 & 0b11111100); // Disable D+/D- detection and such
  I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);

  /*REG03*/
  unsigned char reg03 = 0x00;
  I2CReadRegister(&bbi2c, bqAddr, 0x03, &reg03, 0x8);
  reg03 = (reg03 & 0b11011111); // Disable OTG
  I2CWriteRegister(&bbi2c, bqAddr, 0x03, reg03);

  /*REG04*/
  unsigned char reg04 = 0x00;
  I2CReadRegister(&bbi2c, bqAddr, 0x04, &reg04, 0x8);
  reg04 = (reg04 & 0b10000000) | chrgCurrent; // Set max fast charge current
  I2CWriteRegister(&bbi2c, bqAddr, 0x04, reg04);
}


void getBattVoltage() {
  if (!isPowered) {
    unsigned char reg02 = 0x00;
    I2CReadRegister(&bbi2c, bqAddr, 0x02, &reg02, 0x8); 
    reg02 = (reg02 & 0b01111111) | 0b10000000; // Get single reading
    I2CWriteRegister(&bbi2c, bqAddr, 0x02, reg02);
    delay(200);
  }
  unsigned char adcRegStatus = 0;
  I2CReadRegister(&bbi2c, bqAddr, 0x0E, &adcRegStatus, 0x8);
  adcRegStatus &= 0b1111111;
  battVolt = adcRegStatus / 50;
}



void receiveDataWire(int16_t numBytes) {
  
}

void transmitDataWire() {

}

void setLED(uint8_t r, uint8_t g, uint8_t b, bool enabled) {
  if (!enabled) {
    pixels.clear();
  }
  else {
    for (int i = 0; i < NUMPIXELS; i++) {

    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(i, pixels.Color(r, g, b)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(100); // Delay for a period of time (in milliseconds).

    }
  }

}

void monitorBatt() {
  getBattVoltage();
  delay(500);
  if (isCharging || !isPowered) {
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