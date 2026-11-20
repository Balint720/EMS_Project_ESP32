#include "app/function_gen_vars.h"

#include "hal/hal.h"

#include "sys/convert.h"

#include <Arduino.h>

#include "hal/bluetooth/bluetooth_uuid.h"
#include "hal/adc.h"

#include "app/valchange_request.h"
#include <hal/bluetooth/bluetooth.h>

/*
*   Func generator variables that are inaccessible outside of this file
*/
static struct funcGenVars_st *funcGenVars;
static constexpr struct funcGenVars_st funcGenVars_MIN {
        .waveform = SINUS,
        .pulseWidth = 0,
        .freq = 0,
        .amp = 0.0
};

static constexpr struct funcGenVars_st funcGenVars_MAX {
    .waveform = SAWTOOTH,
    .pulseWidth = 5000000,
    .freq = 25000000,
    .amp = 10000.0
};
static bool funcGenOn = false;
static FuncGenMode funcGenMode = FREE;

/*
*   APP Interface functions
*/

void APP_SetupFuncGen() {
    #if INCLUDE_BT
    updateBtValsToVars();
    #endif
}

void APP_SetupGlobalFuncGenVars() {
    funcGenVars = new funcGenVars_st {
        .waveform = SQUARE,
        .pulseWidth = 0,
        .freq = 1,
        .amp = 5.0
    };

    funcGenOn = false;
    funcGenMode = FREE;
}

void APP_LoopFuncGen() {
    static unsigned int currentStepOfRecipe = -1;
    static unsigned long time = 0;
    static time_t currentStepTime = 0;

    switch (funcGenMode) {
        case FREE:
            currentStepOfRecipe = -1;
            break;
        case RECIPE_MODE:
            if (time + currentStepTime < millis() || currentStepOfRecipe == -1) {
                currentStepOfRecipe++;
                if (currentStepOfRecipe >= APP_GetCurrentRecipe()->stepCount) {
                    debug_print("Recipe finished.\n");
                    currentStepOfRecipe = -1;
                    APP_TurnOffFgen();
                    funcGenMode = FREE;
                    time = 0;
                }
                else {
                    debug_print("Moving to step %d.\n", currentStepOfRecipe);
                    currentStepTime = APP_GetCurrentStepTimeIndexed(currentStepOfRecipe);
                    funcGenVars_st curr = APP_GetCurrentRecipe()->recipe[currentStepOfRecipe];
                    debug_print("Trying to set amplitude to %f\n", curr.amp);
                    //APP_FuncGenChangeRequest(Char_UUID_Index::AMPLITUDE, curr.amp);
                    vTaskDelay(5);
                    debug_print("Trying to set frequency to %f\n", curr.freq);
                    APP_FuncGenChangeRequest<double>(Char_UUID_Index::FREQUENCY, curr.freq);
                    vTaskDelay(5);
                    debug_print("Trying to set pulsewidth to %d\n", curr.pulseWidth);
                    //APP_FuncGenChangeRequest(Char_UUID_Index::PULSEWIDTH, curr.pulseWidth);
                    vTaskDelay(5);
                    debug_print("Trying to set waveform to %d\n", curr.waveform);
                    APP_FuncGenChangeRequest<int>(Char_UUID_Index::WAVEFORM, curr.waveform);
                    vTaskDelay(5);
                    time = millis();
                }
            }
            break;
    }
}

/*
*   Storing values that we get from bluetooth
*/
static void UpdateVarsToBtVals() {
    APP_SetAmp(HAL::HAL_BT_GetValueDouble(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::AMPLITUDE));
    APP_SetPulseWidth(HAL::HAL_BT_GetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::PULSEWIDTH));
    APP_SetFreq(HAL::HAL_BT_GetValueDouble(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::FREQUENCY));
    APP_SetWaveForm((Waveform)HAL::HAL_BT_GetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::WAVEFORM));
    if (HAL::HAL_BT_GetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::FG_STATE)) APP_TurnOnFgen();
    else                                                                                APP_TurnOffFgen();
}

/*
*   Saving values to bluetooth values
*/
static void updateBtValsToVars() {
    HAL::HAL_BT_SetValueDouble(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::AMPLITUDE, getAmp());
    HAL::HAL_BT_SetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::PULSEWIDTH, getPulseWidth());
    HAL::HAL_BT_SetValueDouble(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::FREQUENCY, getFreq());
    HAL::HAL_BT_SetValueInt(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::WAVEFORM, (int)getWaveform());
    HAL::HAL_BT_SetValueInt(Service_UUID_Index::SETTINGS, Char_UUID_Index::FG_STATE, (int)isOn());
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

enum Errors APP_SetWaveForm(enum Waveform w, funcGenVars_st* st) {
    funcGenVars_st *chosenSt = st;
    if (chosenSt == nullptr) chosenSt = funcGenVars;
    chosenSt->waveform = w;
    return NO_ERROR;
}

enum Errors APP_SetPulseWidth(unsigned int pulseWidth, funcGenVars_st* st) {
    funcGenVars_st *chosenSt = st;
    if (chosenSt == nullptr) chosenSt = funcGenVars;

    if (pulseWidth < funcGenVars_MIN.pulseWidth) {
        return UNDER_MINIMUM;
    }
    else if (pulseWidth > funcGenVars_MAX.pulseWidth) {
        return OVER_MAXIMUM;
    }
    else {
        chosenSt->pulseWidth = pulseWidth;
        return NO_ERROR;
    }
}
enum Errors APP_SetFreq(double freq, funcGenVars_st* st) {
    funcGenVars_st *chosenSt = st;
    if (chosenSt == nullptr) chosenSt = funcGenVars;

    if (freq < funcGenVars_MIN.freq) {
        return UNDER_MINIMUM;
    }
    else if (freq > funcGenVars_MAX.freq) {
        return OVER_MAXIMUM;
    }
    else {
        chosenSt->freq = freq;
        return NO_ERROR;
    }
}
enum Errors APP_SetAmp(double amp, funcGenVars_st* st) {
    funcGenVars_st *chosenSt = st;
    if (chosenSt == nullptr) chosenSt = funcGenVars;

    if (isnan(amp)) {
        return INVALID_INPUT;
    }
    else if (amp < funcGenVars_MIN.amp) {
        return UNDER_MINIMUM;
    }
    else if (amp > funcGenVars_MAX.amp) {
        return OVER_MAXIMUM;
    }
    else {
        chosenSt->amp = amp;
        return NO_ERROR;
    }
}

enum Errors APP_TurnOnFgen() {
    funcGenOn = true;
    return NO_ERROR;
}

enum Errors APP_TurnOffFgen() {
    funcGenOn = false;
    return NO_ERROR;
}

enum Errors APP_StartFgenRecipe() {
    if (funcGenMode == FREE) {
        funcGenMode = RECIPE_MODE;
        return NO_ERROR;
    }
    else return BUSY_WITH_RECIPE;
}

enum Errors APP_StopFgenRecipe() {
    if (funcGenMode == RECIPE_MODE) {
        funcGenMode = FREE;
        return NO_ERROR;
    }
    else return INVALID_INPUT;
}