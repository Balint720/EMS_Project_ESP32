#include "app/function_gen_vars.h"

#include "hal/hal.h"

#include "sys/convert.h"

#include <Arduino.h>

#include "hal/bluetooth_uuid.h"
#include "hal/adc.h"

#include "app/valchange_request.h"

/*
*   Storing values that we get from bluetooth
*/
static void UpdateVarsToBtVals() {
    setAmp(HAL_BT_GetValueDouble(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::AMPLITUDE));
    setPulseWidth(HAL_BT_GetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::PULSEWIDTH));
    setFreq(HAL_BT_GetValueDouble(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::FREQUENCY));
    setWaveform((Waveform)HAL_BT_GetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::WAVEFORM));
    if (HAL_BT_GetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::FG_STATE)) turnOn();
    else                                                                                turnOff();
}

/*
*   Saving values to bluetooth values
*/
static void UpdateBtValsToVars() {
    debug_print("Amplitude\n");
    HAL_BT_SetValueDouble(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::AMPLITUDE, getAmp());
    HAL_BT_SetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::PULSEWIDTH, getPulseWidth());
    HAL_BT_SetValueDouble(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::FREQUENCY, getFreq());
    HAL_BT_SetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::WAVEFORM, (int)getWaveform());
    HAL_BT_SetValueInt(Service_UUID_Index::SETTINGS, Char_UUID_Index::FG_STATE, (int)isOn());
}

static unsigned long int lastCheck = millis();

/*
*   Func generator variables that are inaccessible outside of this file
*/
static struct funcGenVars_st *funcGenVars;
static struct funcGenVars_st *funcGenVars_MIN;
static struct funcGenVars_st *funcGenVars_MAX;
static bool funcGenOn = false;
static FuncGenMode funcGenMode = FREE;

void APP_SetupFuncGen() {
    UpdateBtValsToVars();
}

void APP_LoopFuncGen() {
#if defined(PRINT_DEBUG_SERIAL)
    unsigned long t = millis();
    if (lastCheck + 10000 < t) {
        lastCheck = t;

        debug_print("\n");
        debug_print("Amplitude: %f\n", getAmp());
        debug_print("Pulse width: %d\n", getPulseWidth());
        debug_print("Frequency: %f\n", getFreq());
        debug_print("Waveform: %d\n", getWaveform());
    }
#endif
    static unsigned int currentStepOfRecipe = 0;
    static unsigned long time = 0;

    switch (funcGenMode) {
        case FREE:
            time = 0;
            break;
        case RECIPE_MODE:
            if (time + 10000 < millis() || time == 0) {
                currentStepOfRecipe++;
                if (currentStepOfRecipe >= getCurrentRecipe()->stepCount) {
                    turnOff();
                    funcGenMode = FREE;
                    time = 0;
                }
                else {
                    funcGenVars_st curr = getCurrentRecipe()->recipe[currentStepOfRecipe];
                    APP_FuncGenChangeRequest(Char_UUID_Index::AMPLITUDE, curr.amp);
                    APP_FuncGenChangeRequest(Char_UUID_Index::FREQUENCY, curr.freq);
                    APP_FuncGenChangeRequest(Char_UUID_Index::PULSEWIDTH, curr.pulseWidth);
                    APP_FuncGenChangeRequest(Char_UUID_Index::WAVEFORM, curr.waveform);
                    time = millis();
                }
            }
            break;
    }
}

void APP_SetupGlobalFuncGenVars() {
    funcGenVars = new funcGenVars_st {
        .waveform = SQUARE,
        .pulseWidth = 0,
        .freq = 1,
        .amp = 5.0
    };
    funcGenVars_MIN = new funcGenVars_st {
        .waveform = SINUS,
        .pulseWidth = 0,
        .freq = 0,
        .amp = 0.0
    };
    funcGenVars_MAX = new funcGenVars_st {
        .waveform = SAWTOOTH,
        .pulseWidth = 400,
        .freq = 25000000,
        .amp = 100.0
    };

    funcGenOn = false;
    funcGenMode = FREE;
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

enum Errors setWaveform(enum Waveform w, funcGenVars_st* st) {
    if (funcGenMode == RECIPE_MODE) return BUSY_WITH_RECIPE;
    funcGenVars->waveform = w;
    return NO_ERROR;
}

enum Errors setPulseWidth(unsigned int pulseWidth, funcGenVars_st* st) {
    if (funcGenMode == RECIPE_MODE) return BUSY_WITH_RECIPE;
    funcGenVars_st *chosenSt = st;
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
enum Errors setFreq(double freq, funcGenVars_st* st) {
    if (funcGenMode == RECIPE_MODE) return BUSY_WITH_RECIPE;
    funcGenVars_st *chosenSt = st;
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
enum Errors setAmp(double amp, funcGenVars_st* st) {
    if (funcGenMode == RECIPE_MODE) return BUSY_WITH_RECIPE;
    funcGenVars_st *chosenSt = st;
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
    if (funcGenMode == RECIPE_MODE) return BUSY_WITH_RECIPE;
    funcGenOn = true;
    return NO_ERROR;
}

enum Errors turnOff() {
    if (funcGenMode == RECIPE_MODE) return BUSY_WITH_RECIPE;
    funcGenOn = false;
    return NO_ERROR;
}

enum Errors startRecipe() {
    if (funcGenMode == FREE) {
        funcGenMode = RECIPE_MODE;
        return NO_ERROR;
    }
    else return BUSY_WITH_RECIPE;
}

enum Errors stopRecipe() {
    if (funcGenMode == RECIPE_MODE) {
        funcGenMode = FREE;
        return NO_ERROR;
    }
    else return INVALID_INPUT;
}