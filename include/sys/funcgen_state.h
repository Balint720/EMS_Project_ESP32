#pragma once

#include <cstdint>

class SquareWave {
    private:
        uint16_t max_val_;
        uint16_t output_;

    public:
        void turnOn();
        void turnOff();
        void setOn(bool);
        void toggle();

        void set_max_val(uint16_t val);
};

extern SquareWave SYS_CurrWave;