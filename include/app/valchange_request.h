#pragma once

/*
*   Functions for sending data change requests to the app
*   Main use case is to give the bluetooth layer an interface to change the function generator stored values
*   I forgot the rest of the comment... I got distracted by something
*/
#include "app/function_gen_vars.h"
#include "hal/bluetooth_uuid.h"

#include <Arduino.h>

template <typename T>
bool APP_FuncGenChangeRequest(enum Char_UUID_Index fvt, T val) {
    switch (fvt) {
        case WAVEFORM:
            return setWaveform((Waveform)val) == NO_ERROR;
        case PULSEWIDTH:
            return setPulseWidth(val) == NO_ERROR;
        case FREQUENCY:
            Serial.println("Setting frequency");
            return setFreq(val) == NO_ERROR;
        case AMPLITUDE:
            return setAmp(val) == NO_ERROR;
        default:
            return false;
    }
}

template <typename T>
T APP_FuncGenGetRequest(enum Char_UUID_Index fvt) {
    switch (fvt) {
        case WAVEFORM:
            return (T)getWaveform();
        case PULSEWIDTH:
            return (T)getPulseWidth();
        case FREQUENCY:
            return (T)getFreq();
        case AMPLITUDE:
            return (T)getAmp();
        default:
            return 0;
    }
}