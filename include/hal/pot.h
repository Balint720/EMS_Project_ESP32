#pragma once

#include <stdint.h>
#include "app/function_gen_vars.h"

void initPot();

Errors setValueOfResistance(uint8_t resistance);
Errors setValueOfResistanceScaled(double resistance);

double getValueOfResistance();

void HAL_SetupPotmeter();