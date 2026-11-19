#pragma once

#define PIN_FGENOUTPUT_READ 2
#define PIN_FGENOUTPUT_LED 4

#include <stdint.h>
#include <sys/funcgen_state.h>

void HAL_SetupDAC();
void HAL_LoopDAC();
void HAL_SetDirectDAC(WaveOutput value);

uint32_t getFgenADCInput();
uint32_t getFgenADCInput2();