#include "hal/hal.h"

#include "hal/bluetooth/bluetooth.h"
#include "hal/bluetooth/bluetooth_uuid.h"
#include "hal/web.h"
#include "hal/funcgen_chip.h"
#include "hal/spi.h"
#include "hal/pot.h"
#include "hal/adc.h"
#include "hal/serial.h"
#include "hal/gpio.h"

#include "sys/sys.h"
#include "sys/convert.h"
#include "app/funcgen_state.h"
#include "hal/timer.h"
#include "app/funcgen_state_update.h"
#include <hal/dac.h>


void HAL_Setup() {
    debug_print("Setting up GPIO pins...\n");
    HAL_SetupGPIO();
    debug_print("GPIO setup completed!\n");
    debug_print("Setting up ADC...\n");
    HAL_SetupADC();
    debug_print("ADC setup completed!\n");
    #if INCLUDE_WIFI
    HAL_SetupWiFi();
    HAL_SetupWebServer();
    #endif
    #if INCLUDE_BT
    debug_print("Setting up bluetooth...\n");
    HAL_SetupBluetooth();
    debug_print("Bluetooth setup completed!\n");
    #endif
    debug_print("Setting up SPI...\n");
    HAL_SetupSPI();
    debug_print("SPI setup completed!\n");
    debug_print("Setting up Potmeter...\n");
    HAL_SetupPotmeter();
    debug_print("Potmeter setup completed!\n");
    debug_print("Setting up Function Generator Chip...\n");
    HAL_SetupFuncGenChip();
    debug_print("Function Generator chip setup completed!\n");
    // debug_print("Setting up DAC...\n");
    // HAL_SetupDAC();
    // debug_print("DAC setup completed!\n");
}

static unsigned long timer = millis();

void HAL_Loop() {
    HAL_LoopADC();
    HAL_LoopSerialRead();
    digitalWrite(PIN_FGENOUTPUT_LED, APP_CurrWave.getState() == SquareWaveState::A_PLUS || APP_CurrWave.getState() == SquareWaveState::A_MINUS); 
}

void HAL_ReactToValueChange(Char_UUID_Index cI) {
    switch (cI) {
        case FREQUENCY:
            //APP_SetFreqAD9833(getFreq());
            SYS_SetTimerInterruptTime(HW_TIMERS::FUNCTION_FREQUENCY, frequencyToMicroseconds(getFreq()));
            APP_UpdateSquareWaveFrequency(getFreq());
            break;
        case FG_STATE:
            //if (isOn()) startAD9833();
            //else        stopAD9833();
            if (isOn()) {
                APP_StartSquareWaveTimer(getFreq(), getPulseWidth());
            }
            else {
                APP_StopSquareWaveTimer();
            }
            break;
        case PULSEWIDTH:
            APP_UpdateSquareWavePulseWidth(getPulseWidth());
            break;
        case AMPLITUDE:
            // setValueOfResistance(calcResistorValueToSet(getAmp()));
            //setValueOfResistance((getAmp()/10000)*255);
            break;
        case WAVEFORM:
            //APP_SetWaveFormAD9833(getWaveform());
            if (getWaveform() == SQUARE && isOn()) {
                APP_StartSquareWaveTimer(getFreq(), getPulseWidth());
            }
            else if (getWaveform() != SQUARE) {
                APP_StopSquareWaveTimer();
            }
            break;
        default:
            break;
    }
}