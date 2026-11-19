#include "hal/gpio.h"

#include <Arduino.h>


void HAL_SetupGPIO() {
    pinMode(PIN_FGENOUTPUT_LED, OUTPUT);
}