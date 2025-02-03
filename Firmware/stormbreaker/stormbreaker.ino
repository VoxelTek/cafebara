#include <BitBang_I2C.h>

#include <Wire.h>
#include <twi.h>
#include <twi_pins.h>

#define CHRG_STAT PIN_PC0

#define SDA2 PIN_PC2
#define SCL2 PIN_PC3



BBI2C bbi2c;

bool isPowered = false;
bool isCharging = false;

float battVolt = 0.0;

uint8_t bqAddr = 0x6A;




void setup() {
  memset(&bbi2c, 0, sizeof(bbi2c));
  bbi2c.bWire = false;
  bbi2c.iSDA = SDA2;
  bbi2c.iSCL = SCL2;
  I2CInit(&bbi2c, 100000L);

  Wire.onReceive(receiveDataWire);
  Wire.onRequest(transmitDataWire);
  Wire.begin(0x54);
}

void loop() {
  if (isPowered) {
    getBattVoltage();
  }



}


void getBattVoltage() {
  uint8_t adcRegStatus = 0;
  I2CRead(&bbi2c, bqAddr, *pu8Data, 0x0E);
  adcRegStatus &= 0b1111111;
  battVolt = adcRegStatus / 50;
}
void receiveDataWire(int16_t numBytes) {
  
}

void transmitDataWire() {

}