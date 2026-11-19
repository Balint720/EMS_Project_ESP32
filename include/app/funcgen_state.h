#pragma once

#include <cstdint>

#ifndef ADC_BIT_COUNT
    #define ADC_BIT_COUNT 12
#endif

#if ADC_BIT_COUNT < 9
    typedef uint8_t WaveOutput;
#elif ADC_BIT_COUNT < 16
    typedef uint16_t WaveOutput;
#elif ADC_BIT_COUNT < 32
    typedef uint32_t WaveOutput;
#else
    typedef uint64_t WaveOutput;
#endif

enum SquareWaveState {
    NEUTRAL,
    A_PLUS,
    NEUTRAL2,
    A_MINUS
};

class SquareWave {
    private:
        SquareWaveState state_;
        WaveOutput max_val_;
        WaveOutput output_;
        bool negative_;
        bool overriden_;
        bool active_;

        void setState(SquareWaveState);
        void setOutputFromState();

    public:
        SquareWave();

        SquareWaveState gotoNextState();
        void gotoGivenState(SquareWaveState);
        void overrideOutput(WaveOutput output, bool negative = false);
        void turnOff();
        void turnOn();
        void setOn(bool);
        void reset();

        void set_max_val(WaveOutput val);

        SquareWaveState getState();
        WaveOutput getOutput();
};

extern SquareWave APP_CurrWave;