#include "hal/dac.h"

#include <MCP4725.h>

static MCP4725 *dac_chip;

void initMCP4725() {
    dac_chip = new MCP4725{0b000};

    dac_chip->begin();
}

void setPercent(float percentage) {
    dac_chip->setPercentage(percentage);
}


void HAL_SetDirectDAC(WaveOutput value) {
    dac_chip->setValue(static_cast<uint16_t>(value));
}

void HAL_SetupDAC() {
    initMCP4725();
}