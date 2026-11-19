#pragma once

#include <stdint.h>

void HAL_SetupADC();
void HAL_LoopADC();

uint32_t getFgenADCInput();
uint32_t getFgenADCInput2();