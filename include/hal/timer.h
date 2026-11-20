#pragma once

#include <cstdint>


namespace HAL {
    constexpr uint32_t kTimer1Frequency = 1000000UL;

    

    enum class HW_TIMERS {
        FUNCTION_FREQUENCY
    };

    void HAL_SetupTimers();

    void HAL_SetTimerInterruptTime(HW_TIMERS timer_name, uint64_t microseconds, bool autoreload = true, uint64_t autoreload_cnt = 0);
}
