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
bool APP_FuncGenChangeRequest(enum Char_UUID_Index fvt, T val, funcGenVars_st *st = nullptr, int index = 0) {
    if (fvt >= Char_UUID_Index::RECIPE_WAVEFORM && fvt <= Char_UUID_Index::RECIPE_LENGTH && st == nullptr)
    {
        debug_print("No recipe step given!\n");
        return false;
    }
        
    switch (fvt) {
        case Char_UUID_Index::WAVEFORM:
            if (setWaveform((Waveform)val) == NO_ERROR) {
                HAL_ReactToValueChange(Char_UUID_Index::WAVEFORM);
                return true;
            }
            else return false;
        case Char_UUID_Index::PULSEWIDTH:
            if (setPulseWidth(val) == NO_ERROR) {
                HAL_ReactToValueChange(Char_UUID_Index::WAVEFORM);
                return true;
            }
            else return false;
        case Char_UUID_Index::FREQUENCY:
            if (setFreq(val) == NO_ERROR) {
                HAL_ReactToValueChange(Char_UUID_Index::FREQUENCY);
                return true;
            }
            else return false;
        case Char_UUID_Index::AMPLITUDE:
            if (setAmp(val) == NO_ERROR) {
                HAL_ReactToValueChange(Char_UUID_Index::AMPLITUDE);
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
            if (index == 0)         return setRecipeSelectionNumber(val);
            else                    return setRecipeStepNumber(val);
        case Char_UUID_Index::RECIPE_WAVEFORM:
            if (setWaveform((Waveform)val, st) == NO_ERROR) {
                return true;
            }
            else return false;
        case Char_UUID_Index::RECIPE_PULSEWIDTH:
            if (setPulseWidth(val, st) == NO_ERROR) {
                return true;
            }
            else return false;
        case Char_UUID_Index::RECIPE_FREQUENCY:
            if (setFreq(val, st) == NO_ERROR) {
                return true;
            }
            else return false;
        case Char_UUID_Index::RECIPE_AMPLITUDE:
            if (setAmp(val, st) == NO_ERROR) {
                return true;
            }
            else return false;
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