#pragma once

// Note: AD9833.h is included in funcgen_chip.cpp
// Function declarations below are sufficient for the header

#include "app/function_gen_vars.h"

void initAD9833();
void startAD9833();
void stopAD9833();
void testAD9833();

void setFreqAD9833(float freq);
float getFreqAD9833();
int getStateAD9833();
void setWaveformAD9833(Waveform wf);

void HAL_SetupFuncGenChip();