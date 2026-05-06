#include "app/function_gen_vars.h"

#include "hal/hal.h"

#include "sys/convert.h"

#include <Arduino.h>

#include "hal/bluetooth_uuid.h"
#include "hal/adc.h"
/*
*   Formatting values that we get from bluetooth
*/

static void UpdateVars() {
    setAmp(HAL_BT_GetValueFloat(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::AMPLITUDE));
    setPulseWidth(HAL_BT_GetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::PULSEWIDTH));
    setFreq(HAL_BT_GetValueFloat(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::FREQUENCY));
    setWaveform((Waveform)HAL_BT_GetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::WAVEFORM));

}

static unsigned long int lastCheck = millis();

void APP_SetupFuncGen() {
    UpdateVars();
}

void APP_LoopFuncGen() {
    unsigned long t = millis();
    if (lastCheck + 10000 < t) {
        lastCheck = t;
        //UpdateVars();

        Serial.printf("\n");
        Serial.printf("Amplitude: %f\n", getAmp());
        Serial.printf("Pulse width: %d\n", getPulseWidth());
        Serial.printf("Frequency: %f\n", getFreq());
        Serial.printf("Waveform: %d\n", getWaveform());

        Serial.printf("ADC output: %d\n", HAL_GetFuncGenOutput());
        Serial.printf("ADC output: %d\n", getFgenADCInput2());


    }
}

/*
*   Func generator variables that are inaccessible outside of this file
*/
static struct funcGenVars_t *funcGenVars;
static struct funcGenVars_t *funcGenVars_MIN;
static struct funcGenVars_t *funcGenVars_MAX;
static bool funcGenOn = false;

void APP_SetupGlobalFuncGenVars() {
    funcGenVars = new funcGenVars_t();
    funcGenVars_MIN = new funcGenVars_t {
        .waveform = SQUARE,
        .pulseWidth = 0,
        .freq = 0,
        .amp = 0.0
    };
    funcGenVars_MAX = new funcGenVars_t {
        .waveform = SQUARE,
        .pulseWidth = 400,
        .freq = 25000000,
        .amp = 100.0
    };

    funcGenOn = false;
}

/*
*   Getters and setters
*/
enum Waveform getWaveform() {
    return funcGenVars->waveform;
}

unsigned int getPulseWidth() {
    return funcGenVars->pulseWidth;
}

double getFreq() {
    return funcGenVars->freq;
}

double getAmp() {
    return funcGenVars->amp;
}

bool isOn() {
    return funcGenOn;
}

enum Errors setWaveform(enum Waveform w, funcGenVars_t* st) {
    funcGenVars->waveform = w;
    return NO_ERROR;
}

enum Errors setPulseWidth(unsigned int pulseWidth, funcGenVars_t* st) {
    funcGenVars_t *chosenSt = st;
    if (chosenSt == nullptr) chosenSt = funcGenVars;

    if (pulseWidth < funcGenVars_MIN->pulseWidth) {
        return UNDER_MINIMUM;
    }
    else if (pulseWidth > funcGenVars_MAX->pulseWidth) {
        return OVER_MAXIMUM;
    }
    else {
        chosenSt->pulseWidth = pulseWidth;
        return NO_ERROR;
    }
}
enum Errors setFreq(double freq, funcGenVars_t* st) {
    funcGenVars_t *chosenSt = st;
    if (chosenSt == nullptr) chosenSt = funcGenVars;

    if (freq < funcGenVars_MIN->freq) {
        return UNDER_MINIMUM;
    }
    else if (freq > funcGenVars_MAX->freq) {
        return OVER_MAXIMUM;
    }
    else {
        chosenSt->freq = freq;
        return NO_ERROR;
    }
}
enum Errors setAmp(double amp, funcGenVars_t* st) {
    funcGenVars_t *chosenSt = st;
    if (chosenSt == nullptr) chosenSt = funcGenVars;

    if (isnan(amp)) {
        return INVALID_INPUT;
    }
    else if (amp < funcGenVars_MIN->amp) {
        return UNDER_MINIMUM;
    }
    else if (amp > funcGenVars_MAX->amp) {
        return OVER_MAXIMUM;
    }
    else {
        chosenSt->amp = amp;
        return NO_ERROR;
    }
}

enum Errors turnOn() {
    funcGenOn = true;
    return NO_ERROR;
}

enum Errors turnOff() {
    funcGenOn = false;
    return NO_ERROR;
}