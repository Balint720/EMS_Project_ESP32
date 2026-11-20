#include "hal/i2c.h"
#include "hal/spi.h"

#include <Arduino.h>
#include <Wire.h>

TwoWire* dac_I2C_wire;

void setupForDAC() {
    dac_I2C_wire = new TwoWire{0};
    dac_I2C_wire->begin(PIN_SPI_CS_1, PIN_SPI_CLK_1);

    for (uint8_t addr = 1; addr < 127; addr++) {
    dac_I2C_wire->beginTransmission(addr);
    if (dac_I2C_wire->endTransmission() == 0) {
      Serial.printf("Found device at 0x%02X\n", addr);
    }
  }
}

void HAL_SetupI2C() {
    setupForDAC();
}