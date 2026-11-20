#include "hal/gpio.h"

#include <Arduino.h>


void HAL::HAL_SetupGPIO() {
    pinMode(PIN_FGENOUTPUT_LED, OUTPUT);
}

void HAL::HAL_SetPinDigital(uint8_t pin, uint8_t val) {

}
void HAL::HAL_SetPinPWN(uint8_t pin, uint8_t val) {
    
}