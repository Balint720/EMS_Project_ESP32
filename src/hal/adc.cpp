#include "hal/adc.h"

#include <Arduino.h>

#include "sys/sys.h"

static uint32_t fGenOutputmV = 0;
static uint16_t fGenOutput = 0;

const int channel = 0;      // LEDC channel
const int freq = 5000;      // 5 kHz PWM
const int resolution = 12;   // 12-bit (0-4096)

static void setupFgenRead() {
    pinMode(PIN_FGENOUTPUT_READ, INPUT);
    analogSetPinAttenuation(PIN_FGENOUTPUT_READ, ADC_0db);

    // LED that follows value
    ledcAttach(PIN_FGENOUTPUT_LED, freq, resolution);
}

void HAL_SetupADC() {
    setupFgenRead();
}

void HAL_LoopADC() {
    fGenOutputmV = analogReadMilliVolts(PIN_FGENOUTPUT_READ);
    fGenOutput = analogRead(PIN_FGENOUTPUT_READ);
    ledcWrite(channel, fGenOutput);
}

uint32_t getFgenADCInput() {
    return fGenOutputmV;
}

uint32_t getFgenADCInput2() {
    return fGenOutput;
}