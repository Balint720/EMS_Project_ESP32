#include "spi.h"

#include <SPI.h>
#include <Arduino.h>

#define SPI_FLASH_SETTINGS  \
SPISettings(                \
                            \
)                           \


// GPIO pins connected to on board flash memory with SPI
#define PIN_SPI_SCK_FLASH       6
#define PIN_SPI_CS_FLASH        11
#define PIN_SPI_DO_FLASH        7
#define PIN_SPI_DI_FLASH        8


void setupSPI_Flash() {
    pinMode(PIN_SPI_CS_FLASH, OUTPUT);

    SPI.begin(
        PIN_SPI_SCK_FLASH,
        PIN_SPI_DO_FLASH,
        PIN_SPI_DI_FLASH,
        PIN_SPI_CS_FLASH
    );
}

void endSPI() {
    SPI.end();
}