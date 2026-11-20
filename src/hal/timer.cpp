#include "hal/timer.h"

#include <Arduino.h>
#include <esp32-hal-timer.h>

#include <FunctionalInterrupt.h>
#include "hal/dac.h"

static hw_timer_t *timer1;
static esp_timer_handle_t squareWaveTimer;

static uint8_t testOutput = 0;

namespace HAL {
    hw_timer_t* timerEnumToTimer(HW_TIMERS input) {
        switch (input) {
            case HW_TIMERS::FUNCTION_FREQUENCY:
                return timer1;
            default:
                return nullptr;
        }
    }


    void setupTimer() {
        timer1 = timerBegin(kTimer1Frequency);
        HAL_SetTimerInterruptTime(HW_TIMERS::FUNCTION_FREQUENCY, 500000UL);
    }

    void HAL_SetTimerInterruptTime(HW_TIMERS timer_name, uint64_t microseconds, bool autoreload, uint64_t autoreload_cnt) {
        timerAlarm(timerEnumToTimer(timer_name), microseconds, autoreload, autoreload_cnt);
    }

    void HAL_AttachFunctionToTimer(std::function<void>(void)) {
        
    }

    void HAL_SetupTimers() {
        setupTimer();
    }
}
