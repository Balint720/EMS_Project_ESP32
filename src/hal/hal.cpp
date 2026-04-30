#include "hal/hal.h"

#include "hal/bluetooth.h"
#include "hal/bluetooth_uuid.h"
#include "hal/web.h"
#include "hal/funcgen_chip.h"
#include "hal/spi.h"
#include "hal/pot.h"

#include "sys/sys.h"


void HAL_Setup() {
    #if INCLUDE_WIFI
    HAL_SetupWiFi();
    HAL_SetupWebServer();
    #endif
    HAL_SetupBluetooth();
    debug_print("Bluetooth setup completed!\n");
    debug_print("Setting up SPI...\n");
    HAL_SetupSPI();
    debug_print("SPI setup completed!\n");
    debug_print("Setting up Potmeter...\n");
    HAL_SetupPotmeter();
    debug_print("Potmeter setup completed!\n");
    debug_print("Setting up Function Generator Chip...\n");
    HAL_SetupFuncGenChip();
    debug_print("Function Generator chip setup completed!\n");
}

static unsigned long timer = millis();

void HAL_Loop() {
    unsigned long curr = millis();
    if (curr - timer >= 10000) {
        timer = curr;
        debug_print("Func gen chip variables:\n");
        debug_print("Frequency: %f Hz\nState: %d\n", getFreqAD9833(), getStateAD9833());
    }
}

void HAL_ReactToValueChange(Char_UUID_Index cI) {
    switch (cI) {
        case FREQUENCY:
            setFreqAD9833(getFreq());
            break;
        case FG_STATE:
            if (isOn()) startAD9833();
            else        stopAD9833();
            break;
        case PULSEWIDTH:
            setResistance(getPulseWidth());
            break;
        case AMPLITUDE:
            //setResistance(255/getAmp());
            break;
        case WAVEFORM:
            setWaveformAD9833(getWaveform());
            break;
        default:
            break;
    }
}

std::string HAL_BT_GetValue(Service_UUID_Index sI, Char_UUID_Index cI) {
    
    return getCharacteristicValue(sI, cI);
}

void HAL_BT_SetValue(Service_UUID_Index sI, Char_UUID_Index cI, const char *val) {
    setCharacteristicValue(sI, cI, val);
}

void HAL_BT_SetValue(Service_UUID_Index sI, Char_UUID_Index cI, std::string val) {
    setCharacteristicValue(sI, cI, val.c_str());
}

int HAL_SaveRecipeToFlash(const funcGenRecipe_st *recipe, int index) {
    writeRecipe(index, recipe);
    return 0;
}

int HAL_LoadRecipeFromFlash(int index, funcGenRecipe_st* out) {
    readRecipe(index, out);
    debug_print("First two recipe steps read:\n");
    debug_print("Number one:\nFrequency: %d\nAmplitude: %f\nPulse width: %d\nWaveform: %d\n", out->recipe[0].freq, out->recipe[0].amp, out->recipe[0].pulseWidth, out->recipe[0].waveform);
    debug_print("Number two:\nFrequency: %d\nAmplitude: %f\nPulse width: %d\nWaveform: %d\n", out->recipe[1].freq, out->recipe[1].amp, out->recipe[1].pulseWidth, out->recipe[1].waveform);

    return 0;
}