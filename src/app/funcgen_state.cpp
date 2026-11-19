#include "app/funcgen_state.h"
#include <algorithm>
#include <sys/sys.h>

#include "hal/dac.h"
#include <hal/gpio.h>

static constexpr WaveOutput kMaxVal = (static_cast<WaveOutput>(1) << ADC_BIT_COUNT) - 1;

SquareWave APP_CurrWave = SquareWave{};

SquareWave::SquareWave() {
    reset();
}

void SquareWave::setState(SquareWaveState toState) {
    state_ = toState;
    setOutputFromState();
}

void SquareWave::setOutputFromState() {
    if (!active_) {
        output_ = 0;
        return;
    }

    if (overriden_) {
        return;
    }

    switch (state_) {
        case A_PLUS:
            output_ = max_val_;
            negative_ = false;
            break;
        case A_MINUS:
            output_ = max_val_;
            negative_ = true;
            break;
        case NEUTRAL:
        case NEUTRAL2:
        default:
            output_ = 0;
            break;
    }
}

SquareWaveState SquareWave::gotoNextState() {
    SquareWaveState toState = static_cast<SquareWaveState>((state_ + 1) % 4);
    setState(toState);
    digitalWrite(PIN_FGENOUTPUT_LED, state_ == SquareWaveState::A_PLUS || state_ == SquareWaveState::A_MINUS);
    return state_;
}

void SquareWave::gotoGivenState(SquareWaveState newState) {
    if (newState < NEUTRAL || newState > A_MINUS) {
        return;
    }

    overriden_ = false;
    setState(newState);
    digitalWrite(PIN_FGENOUTPUT_LED, state_ == SquareWaveState::A_PLUS || state_ == SquareWaveState::A_MINUS);
}

void SquareWave::overrideOutput(WaveOutput newOutput, bool negative) {
    overriden_ = true;
    negative_ = negative;
    output_ = active_ ? std::max(newOutput, kMaxVal) : 0;
}

void SquareWave::turnOff() {
    active_ = false;
    overriden_ = false;
    setOutputFromState();
}

void SquareWave::turnOn() {
    active_ = true;
    overriden_ = false;
    setOutputFromState();
}

void SquareWave::setOn(bool on) {
    if (on) {
        turnOn();
    }
    else {
        turnOff();
    }
}

void SquareWave::reset() {
    state_ = NEUTRAL;
    max_val_ = kMaxVal/2;
    output_ = 0;
    negative_ = false;
    overriden_ = false;
    active_ = false;
}


void SquareWave::set_max_val(WaveOutput val) {
    max_val_ = std::min(val, kMaxVal);
    if (!overriden_) {
        setOutputFromState();
    }
}

SquareWaveState SquareWave::getState() {
    return state_;
}

WaveOutput SquareWave::getOutput() {
    return output_;
}