#include "hal/pot.h"

#include <MCP_POT.h>

#include "hal/spi.h"

static constexpr double kMaxResVal = 10000.0;
static constexpr double kUnitResVal = kMaxResVal/255.0;

static MCP_POT *pot;

void initPot() {
    pot = new MCP_POT(25, -1, -1, SPI_A);

    pot->begin(24u);
}

Errors setValueOfResistance(uint8_t resistance) {
    pot->setValue(resistance);
    return NO_ERROR;
}

Errors setValueOfResistanceScaled(double resistance)
{
    if (resistance < 0.0) return INVALID_INPUT;
    uint8_t scaled = static_cast<uint8_t>(std::min((resistance / kMaxResVal)*255.0,255.0));
    setValueOfResistance(scaled);
    return NO_ERROR;
}

double getValueOfResistance() {
    return pot->getValue() * kUnitResVal;
}

void HAL_SetupPotmeter() {
    initPot();
}