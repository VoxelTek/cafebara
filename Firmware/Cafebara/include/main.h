#include <avr/io.h>
#include <stddef.h>
#include "gpio.h"
#include "button.h"
#include "i2c_target.h"

static const gpio_t SDA         = {&PORTB, 1};
static const gpio_t SCL         = {&PORTB, 0};

static const gpio_t BQ_INT      = {&PORTC, 0};

static const gpio_t FAN         = {&PORTB, 2};

static const gpio_t LED         = {&PORTA, 1};

static const gpio_t PWR_EN      = {&PORTC, 3}; // Output
static const gpio_t BUTTON      = {&PORTA, 2};

struct button pwr_button;

static const gpio_t TEMP_ALERT  = {&PORTB, 5};
static const gpio_t HPD         = {&PORTB, 4};

int main();

bool setup();
void loop();

void getEEPROM();
void overTemp();
void buttonHeld();
void chargingStatus();
void initFan();
void setFan(bool active, uint8_t speed);
void powerLED(uint8_t mode);
void consoleOn();
void consoleOff();
void enableShipping();
void setupBQ();
void writeToEEPROM();
void applyChanges();
void getBattVoltage();
bool i2c_bq_write(uint8_t addr, uint8_t reg, void const* buf, size_t len, void* context);
bool i2c_bq_read(uint8_t addr, uint8_t reg, void const* buf, size_t len, void* context);
void setLED(uint8_t r, uint8_t g, uint8_t b, float bright, bool enabled);
void battChargeStatus();
void monitorBatt();
void checkHPDstatus();
void setupUSART();