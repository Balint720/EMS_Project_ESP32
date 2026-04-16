#include "function_gen_vars.h"

/*
*   Func generator variables that are inaccessible outside of this file
*/
static struct funcGenVars_t funcGenVars;
static struct funcGenVars_t funcGenVars_MIN = {
    .amp = 0,
    .freq = 10,
    .pulseWidth = 100
};
static struct funcGenVars_t funcGenVars_MAX = {
    .amp = 100,
    .freq = 60,
    .pulseWidth = 400
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
    if (amp < funcGenVars_MIN.amp) {
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