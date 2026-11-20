#include "hal/i2c.h"

#include <Wire.h>
#include "hal/gpio.h"

TwoWire *HAL_DigAnalogConvWire;

void HAL_SetupI2C() {
    HAL_DigAnalogConvWire = new TwoWire{0};
    HAL_DigAnalogConvWire->begin(PIN_SPI_CS_1, PIN_SPI_CLK_1);
}