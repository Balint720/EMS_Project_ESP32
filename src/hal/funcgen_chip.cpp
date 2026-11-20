#include "hal/funcgen_chip.h"

#include <AD9833.h>
#include <Arduino.h>

#include "hal/spi.h"

#include "sys/sys.h"
#include <hal/gpio.h>


static AD9833 *fgChip;

void HAL::initAD9833() {
    fgChip = new AD9833{PIN_SPI_CS_1, SPI_A};

    fgChip->begin();
}

void HAL::startAD9833() {
    fgChip->setPowerMode(AD9833_PWR_ON);
}

void HAL::stopAD9833() {
    //fgChip->setPowerMode(AD9833_PWR_DISABLE_DAC);
}

void HAL::resetAD9833() {
    fgChip->reset();
    testAD9833();
}

void HAL::testAD9833() {
    fgChip->setFrequency(1);
    fgChip->setWave(AD9833_SQUARE1);
}

void HAL::setFreqAD9833(float freq) {
    fgChip->setFrequency(freq);
}

float HAL::getFreqAD9833() {
    return fgChip->getFrequency();
}

void HAL::setWaveFormAD9833(Waveform wf) {
    switch (wf) {
        case SINUS:
            fgChip->setWave(AD9833_SINE);
            break;
        case TRIANGLE:
            fgChip->setWave(AD9833_TRIANGLE);
            break;
        case SQUARE:
            fgChip->setWave(AD9833_SQUARE1);
            break;
        case SAWTOOTH:
            fgChip->setWave(AD9833_SQUARE2);
    }
}

int HAL::getStateAD9833() {
    return fgChip->getPowerMode();
}

void HAL::HAL_SetupFuncGenChip() {
    initAD9833();
    startAD9833();
    testAD9833();
}