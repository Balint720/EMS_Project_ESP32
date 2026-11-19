#pragma once

#define PIN_FGENOUTPUT_READ 2
#define PIN_FGENOUTPUT_LED 4

// SPI pins connected to AD9833 and resistor
#define PIN_SPI_D_1        (uint8_t)23
#define PIN_SPI_CLK_1      (uint8_t)18
#define PIN_SPI_CS_1       (uint8_t)5

void HAL_SetupGPIO();