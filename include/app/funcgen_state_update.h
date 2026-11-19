#pragma once

#include <cstdint>

void APP_SetupSquareWaveTimer();
void APP_StartSquareWaveTimer(double frequency_hz, uint32_t pulse_width_us);
void APP_UpdateSquareWavePulseWidth(uint32_t pulse_width_us);
void APP_UpdateSquareWaveFrequency(double frequency_hz);
void APP_StopSquareWaveTimer();
