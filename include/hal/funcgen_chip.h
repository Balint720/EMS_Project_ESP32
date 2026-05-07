#pragma once

// Note: AD9833.h is included in funcgen_chip.cpp
// Function declarations below are sufficient for the header

#include "app/function_gen_vars.h"

#include <stdint.h>
#include <algorithm>

// Function that calculates what resistance to set based on given milliAmps
// Is only true for current setup, it is not hardware independent
static inline uint16_t calcResistorValueToSet(double mA) {
    return std::min((uint16_t)(std::max(160.0 * ((mA-2.0)/100.0), 0.0)), (uint16_t)160);
}

void initAD9833();
void startAD9833();
void stopAD9833();
void testAD9833();

void setFreqAD9833(float freq);
float getFreqAD9833();
int getStateAD9833();
void setWaveformAD9833(Waveform wf);

void HAL_SetupFuncGenChip();