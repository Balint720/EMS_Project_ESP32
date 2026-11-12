#include <MCP4725.h>

static constexpr float kMaxVoltage;

static MCP4725 *dac_chip;

void initMCP4725() {
    dac_chip = new MCP4725{0b000};

    dac_chip->begin();
}

void setPercent(float percentage) {
    dac_chip->setPercentage(percentage);
}

void setDirect(uint16_t val) {
    dac_chip->setValue(val);
}