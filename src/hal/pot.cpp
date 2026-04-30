#include "hal/pot.h"

#include <MCP_POT.h>

#include "hal/spi.h"

static MCP_POT *pot;

void initPot() {
    pot = new MCP_POT(25, -1, -1, SPI_A);

    pot->begin();
}

void setResistance(uint16_t resistance) {
    pot->setValue(resistance);
}

void HAL_SetupPotmeter() {
    initPot();
}