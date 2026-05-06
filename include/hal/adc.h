#pragma once

#define PIN_FGENOUTPUT_READ 2
#define PIN_FGENOUTPUT_LED 4

#include <stdint.h>

void HAL_SetupADC();
void HAL_LoopADC();

uint32_t getFgenADCInput();
uint32_t getFgenADCInput2();