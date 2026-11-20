#pragma once

#include <Wire.h>

extern TwoWire* dac_I2C_wire;

void HAL_SetupI2C();