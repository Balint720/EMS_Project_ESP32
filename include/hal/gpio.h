#pragma once

#include <cstdint>

#define PIN_FGENOUTPUT_READ 2
#define PIN_FGENOUTPUT_LED 4

// SPI pins connected to AD9833 and resistor
#define PIN_SPI_D_1        (uint8_t)23
#define PIN_SPI_CLK_1      (uint8_t)18
#define PIN_SPI_CS_1       (uint8_t)5

namespace HAL {
    void HAL_SetupGPIO();
    void HAL_SetPinDigital(uint8_t pin, uint8_t val);
    void HAL_SetPinPWN(uint8_t pin, uint8_t val);
}


