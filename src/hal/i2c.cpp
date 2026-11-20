#include "hal/i2c.h"
#include "hal/spi.h"

#include <Arduino.h>
#include <Wire.h>

#include "hal/gpio.h"

TwoWire* HAL_DigAnalogConvWire;

void setupDACWire() {
    HAL_DigAnalogConvWire = new TwoWire{0};
    HAL_DigAnalogConvWire->begin(PIN_SPI_CS_1, PIN_SPI_CLK_1);

    for (uint8_t addr = 1; addr < 127; addr++) {
    HAL_DigAnalogConvWire->beginTransmission(addr);
    if (HAL_DigAnalogConvWire->endTransmission() == 0) {
      Serial.printf("Found device at 0x%02X\n", addr);
    }
  }
}

void HAL_SetupI2C() {
    setupDACWire();
}