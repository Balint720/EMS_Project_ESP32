#include "sys/funcgen_state.h"
#include <algorithm>

static constexpr uint16_t kMaxVal = 4095;

void SquareWave::turnOn() {
    output_ = max_val_;
}
void SquareWave::turnOff() {
    output_ = 0;
}
void SquareWave::setOn(bool val) {
    if (val)    turnOn();
    else        turnOff();
}
void SquareWave::toggle() {
    if (output_ != 0)   turnOff();
    else                turnOn();
}

void SquareWave::set_max_val(uint16_t val) {
    max_val_ = std::max(val, kMaxVal);
    if (output_ != 0) output_ = max_val_;
}

SquareWave SYS_CurrWave = SquareWave{};