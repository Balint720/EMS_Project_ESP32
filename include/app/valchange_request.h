#pragma once

/*
*   Functions for sending data change requests to the app
*   Main use case is to give the bluetooth layer an interface to change the function generator stored values
*   I forgot the rest of the comment... I got distracted by something
*/
#include "app/function_gen_vars.h"
#include "hal/bluetooth_uuid.h"
#include "hal/hal.h"
#include "sys/sys.h"

#include <Arduino.h>

template <typename T>
bool APP_FuncGenChangeRequest(enum Char_UUID_Index fvt, T val) {
    switch (fvt) {
        case Char_UUID_Index::WAVEFORM:
            if (setWaveform((Waveform)val) == NO_ERROR) {
                HAL_ReactToValueChange(Char_UUID_Index::WAVEFORM);
                debug_print("Waveform changed to %d.\n", val);
                return true;
            }
            else return false;
        case Char_UUID_Index::PULSEWIDTH:
            if (setPulseWidth(val) == NO_ERROR) {
                HAL_ReactToValueChange(Char_UUID_Index::WAVEFORM);
                debug_print("Pulsewidth changed to %d.\n", val);
                return true;
            }
            else return false;
        case Char_UUID_Index::FREQUENCY:
            if (setFreq(val) == NO_ERROR) {
                HAL_ReactToValueChange(Char_UUID_Index::FREQUENCY);
                debug_print("Frequency changed to %f.\n", val);
                return true;
            }
            else return false;
        case Char_UUID_Index::AMPLITUDE:
            if (setAmp(val) == NO_ERROR) {
                HAL_ReactToValueChange(Char_UUID_Index::AMPLITUDE);
                debug_print("Amplitude changed to %f.\n", val);
                return true;
            }
            else return false;
        case Char_UUID_Index::FG_STATE:
            if (val) {
                return turnOn();
            }
            else {
                return turnOff();
            }
        case Char_UUID_Index::RECIPE_INFO:
        {
            uint16_t *x = (uint16_t*)&val;
#if defined(INT_DATAPACKET_LITTLE_ENDIAN)
            uint8_t rec = ((*x) >> 0);
            uint8_t step = ((*x) >> 8);
#elif defined(INT_DATAPACKET_BIG_ENDIAN)
            uint8_t rec = ((*x) >> 8);
            uint8_t step = (*x);
#else
    #error No int endianness defined
#endif
            debug_print("Recipe: %x\tStep: %x\n", rec, step);
            if (setRecipeSelectionNumber(rec)) {
                if (setRecipeStepNumber(step)) {
                    return true;
                }
            }
            return false;
        }
        case Char_UUID_Index::RECIPE_ACTIONS:
        {
            uint16_t *x = (uint16_t*)&val;
            uint8_t cmd = ((*x) >> 8);
            uint8_t v = (*x);
            debug_print("Cmd: %x\tVal: %x\n", cmd, v);
            return actionOnRecipe(cmd, v);
        }
        case Char_UUID_Index::RECIPE_WAVEFORM:
            if (setWaveform((Waveform)val, getCurrentRecipeStep()) == NO_ERROR) {
                return true;
            }
            else return false;
        case Char_UUID_Index::RECIPE_PULSEWIDTH:
            if (setPulseWidth(val, getCurrentRecipeStep()) == NO_ERROR) {
                return true;
            }
            else return false;
        case Char_UUID_Index::RECIPE_FREQUENCY:
            if (setFreq(val, getCurrentRecipeStep()) == NO_ERROR) {
                return true;
            }
            else return false;
        case Char_UUID_Index::RECIPE_AMPLITUDE:
            if (setAmp(val, getCurrentRecipeStep()) == NO_ERROR) {
                return true;
            }
            else return false;
        case Char_UUID_Index::RECIPE_TIME:
            return (setCurrentStepTime(val) == 0);
        default:
            return false;
    }
}

template <typename T>
T APP_FuncGenGetRequest(enum Char_UUID_Index fvt, int index = 0) {
    switch (fvt) {
        case WAVEFORM:
            return (T)getWaveform();
        case PULSEWIDTH:
            return (T)getPulseWidth();
        case FREQUENCY:
            return (T)getFreq();
        case AMPLITUDE:
            return (T)getAmp();
        case FG_STATE:
            return (T)isOn();
        case RECIPE_INFO:
            if (index == 0)     return (T)getRecipeSelectionNumber();
            else                return (T)getRecipeStepNumber();
        case RECIPE_WAVEFORM:
            return (T)getCurrentRecipeStep()->waveform;
        case RECIPE_PULSEWIDTH:
            return (T)getCurrentRecipeStep()->pulseWidth;
        case RECIPE_FREQUENCY:
            return (T)getCurrentRecipeStep()->freq;
        case RECIPE_AMPLITUDE:
            return (T)getCurrentRecipeStep()->amp;
        default:
            return 0;
    }
}