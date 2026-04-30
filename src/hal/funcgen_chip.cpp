#include "hal/funcgen_chip.h"

#include <AD9833.h>
#include <Arduino.h>

#include "hal/spi.h"

#include "sys/sys.h"


static AD9833 *fgChip;

void initAD9833() {
    fgChip = new AD9833{PIN_SPI_CS_1, SPI_A};

    fgChip->begin();
}

void startAD9833() {
    fgChip->setPowerMode(AD9833_PWR_ON);
}

void stopAD9833() {
    fgChip->setPowerMode(AD9833_PWR_DISABLE_DAC);
}

void testAD9833() {
    fgChip->setFrequency(2);
    fgChip->setWave(AD9833_SQUARE1);
}

void setFreqAD9833(float freq) {
    fgChip->setFrequency(freq);
}

float getFreqAD9833() {
    return fgChip->getFrequency();
}

void setWaveformAD9833(Waveform wf) {
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
    }
}

int getStateAD9833() {
    return fgChip->getPowerMode();
}

void HAL_SetupFuncGenChip() {
    initAD9833();
    startAD9833();
    testAD9833();
}