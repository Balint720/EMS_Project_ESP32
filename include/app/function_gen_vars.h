#pragma once

/*
*   Note about function generator storage and access
*   The function generator variable will be a static variable within a seperate source file
*   This is done to make it "private" and not be directly modifiable, rather only done with functions
*   Functions should do error checking and have hard limits in place
*/

/*
*   Types of waveforms
*/
enum Waveform {
    SINUS,
    SQUARE,
    TRIANGLE,
    SAWTOOTH
};

/*
*   Errors that are returned by the functions that are used to configure the function generator
*/
enum Errors {
    NO_ERROR,
    OVER_MAXIMUM,
    UNDER_MINIMUM,
    INVALID_INPUT
};

/*
*   This struct contains the values that will be used to configure the function generator
*/
struct funcGenVars_t {
    enum Waveform waveform;
    unsigned int pulseWidth;
    double freq;
    double amp;
};

/*
*   Getters and setters
*/
enum Waveform getWaveform();
unsigned int getPulseWidth();
double getFreq();
double getAmp();
bool isOn();

enum Errors setWaveform(enum Waveform, funcGenVars_t *st = nullptr);
enum Errors setPulseWidth(unsigned int, funcGenVars_t *st = nullptr);
enum Errors setFreq(double, funcGenVars_t *st = nullptr);
enum Errors setAmp(double, funcGenVars_t *st = nullptr);

enum Errors turnOn();
enum Errors turnOff();

/*
*   HAL Interface functions
*/
void APP_LoopFuncGen();
void APP_SetupFuncGen();
void APP_SetupGlobalFuncGenVars();

/*
*   Other useful function generator related functions
*/
void printFuncGenVars();