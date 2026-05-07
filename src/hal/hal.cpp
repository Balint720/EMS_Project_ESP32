#include "hal/hal.h"

#include "hal/bluetooth.h"
#include "hal/bluetooth_uuid.h"
#include "hal/web.h"
#include "hal/funcgen_chip.h"
#include "hal/spi.h"
#include "hal/pot.h"
#include "hal/adc.h"

#include "sys/sys.h"
#include "sys/convert.h"


void HAL_Setup() {

    debug_print("Setting up ADC...\n");
    HAL_SetupADC();
    debug_print("ADC setup completed!\n");
    #if INCLUDE_WIFI
    HAL_SetupWiFi();
    HAL_SetupWebServer();
    #endif
    debug_print("Setting up bluetooth...\n");
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
    HAL_LoopADC();
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
            debug_print("Pulse width change not implemented!\n");
            break;
        case AMPLITUDE:
            setValueOfResistance(calcResistorValueToSet(getAmp()));
            break;
        case WAVEFORM:
            setWaveformAD9833(getWaveform());
            break;
        default:
            break;
    }
}

int HAL_BT_GetValueInt(Service_UUID_Index sI, Char_UUID_Index cI) {
    return packetToInt(getCharacteristicValue(sI, cI));
}

float HAL_BT_GetValueFloat(Service_UUID_Index sI, Char_UUID_Index cI) {
    return packetToDouble(getCharacteristicValue(sI, cI));
}

double HAL_BT_GetValueDouble(Service_UUID_Index sI, Char_UUID_Index cI) {
    return packetToDouble(getCharacteristicValue(sI, cI));
}

void HAL_BT_SetValueInt(Service_UUID_Index sI, Char_UUID_Index cI, int value) {
    setCharacteristicValue(sI, cI, intToPacket(value));
}

void HAL_BT_SetValueFloat(Service_UUID_Index sI, Char_UUID_Index cI, float value) {
    setCharacteristicValue(sI, cI, doubleToPacket(value));
}
void HAL_BT_SetValueDouble(Service_UUID_Index sI, Char_UUID_Index cI, double value) {
    setCharacteristicValue(sI, cI, doubleToPacket(value));
}

int HAL_SaveRecipeToFlash(const funcGenRecipe_st *recipe, int index) {
    writeRecipe(index, recipe);
    return 0;
}

int HAL_LoadRecipeFromFlash(int index, funcGenRecipe_st* out) {
    readRecipe(index, out);

    return 0;
}