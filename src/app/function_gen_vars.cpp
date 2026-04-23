#include "app/function_gen_vars.h"

#include "hal/hal.h"

#include "sys/convert.h"

#include <Arduino.h>
/*
*   Formatting values that we get from bluetooth
*/

#define UPDATE_TIME_MS 1000


static int getIntFromBTDataString(Service_UUID_Index sInd, Char_UUID_Index cInd) {
    try {
        int i = std::stoi(HAL_BT_GetValue(sInd, cInd));
        return i;
    }
    catch (std::invalid_argument& e) {
        Serial.println("ERROR: Invalid argument when converting string to int. Value from bluetooth was not a number.");
        return 0;
    }
    catch (std::out_of_range& e) {
        return 0;
    }
}
static double getDoubleFromBTDataString(Service_UUID_Index sInd, Char_UUID_Index cInd) {
    try {
        double d = std::stod(HAL_BT_GetValue(sInd, cInd));
        return d;
    }
    catch (std::invalid_argument& e) {
        Serial.println("ERROR: Invalid argument when converting string to double. Value from bluetooth was not a number.");
        return 0;
    }
    catch (std::out_of_range& e) {
        return 0;
    }
}
static Waveform getWFFromBTDataString(Service_UUID_Index sInd, Char_UUID_Index cInd) {
    return (Waveform)getIntFromBTDataString(sInd, cInd);
}

static int getIntFromBTData(Service_UUID_Index sInd, Char_UUID_Index cInd) {
    return packetToInt(HAL_BT_GetValue(sInd, cInd));
}
static double getDoubleFromBTData(Service_UUID_Index sInd, Char_UUID_Index cInd) {
    return packetToDouble(HAL_BT_GetValue(sInd, cInd));
}
static Waveform getWFFromBTData(Service_UUID_Index sInd, Char_UUID_Index cInd) {
    return (Waveform)packetToInt(HAL_BT_GetValue(sInd, cInd));
}

static void UpdateVars() {
    Serial.println("Entered");
    if (setWaveform(getWFFromBTData(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::WAVEFORM))!= NO_ERROR) {
        //HAL_BT_SetValue(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::WAVEFORM, std::to_string(getWaveform()));
    };
    if (setFreq(getIntFromBTData(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::FREQUENCY)) != NO_ERROR) {
        //HAL_BT_SetValue(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::FREQUENCY, std::to_string(getFreq()));
    };
    if (setAmp(getDoubleFromBTData(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::AMPLITUDE)) != NO_ERROR) {
        //HAL_BT_SetValue(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::AMPLITUDE, std::to_string(getAmp()));
    };
    if (setPulseWidth(getIntFromBTData(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::PULSEWIDTH) != NO_ERROR)) {
        //HAL_BT_SetValue(Service_UUID_Index::FUNCTIONGEN, Char_UUID_Index::PULSEWIDTH, std::to_string(getPulseWidth()));
    };
    Serial.println("Exiting");
}

static unsigned long int lastCheck = millis();

void APP_SetupFuncGen() {
    UpdateVars();
}

void APP_LoopFuncGen() {
    unsigned long t = millis();
    if (lastCheck + 3000 < t) {
        lastCheck = t;
        //UpdateVars();

        Serial.printf("\n");
        Serial.printf("Amplitude: %f\n", getAmp());
        Serial.printf("Pulse width: %d\n", getPulseWidth());
        Serial.printf("Frequency: %d\n", getFreq());
        Serial.printf("Waveform: %d\n", getWaveform());

    }
}

/*
*   Func generator variables that are inaccessible outside of this file
*/
static struct funcGenVars_t funcGenVars;
static struct funcGenVars_t funcGenVars_MIN = {
    .pulseWidth = 100,
    .freq = 10,
    .amp = 0.0
};
static struct funcGenVars_t funcGenVars_MAX = {
    .pulseWidth = 400,
    .freq = 60,
    .amp = 100.0
};

/*
*   Getters and setters
*/
enum Waveform getWaveform() {
    return funcGenVars.waveform;
}

unsigned int getPulseWidth() {
    return funcGenVars.pulseWidth;
}

unsigned int getFreq() {
    return funcGenVars.freq;
}

double getAmp() {
    return funcGenVars.amp;
}

enum Errors setWaveform(enum Waveform w) {
    funcGenVars.waveform = w;
    return NO_ERROR;
}

enum Errors setPulseWidth(unsigned int pulseWidth) {
    if (pulseWidth < funcGenVars_MIN.pulseWidth) {
        return UNDER_MINIMUM;
    }
    else if (pulseWidth > funcGenVars_MAX.pulseWidth) {
        return OVER_MAXIMUM;
    }
    else {
        funcGenVars.pulseWidth = pulseWidth;
        return NO_ERROR;
    }
}
enum Errors setFreq(unsigned int freq) {
    Serial.printf("Received value: %d", freq);
    if (freq < funcGenVars_MIN.freq) {
        return UNDER_MINIMUM;
    }
    else if (freq > funcGenVars_MAX.freq) {
        return OVER_MAXIMUM;
    }
    else {
        funcGenVars.freq = freq;
        return NO_ERROR;
    }
}
enum Errors setAmp(double amp) {
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
        funcGenVars.amp = amp;
        return NO_ERROR;
    }
}