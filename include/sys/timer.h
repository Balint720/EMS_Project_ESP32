#pragma once

#include <cstdint>

constexpr uint32_t kTimer1Frequency = 1000000UL;

enum class HW_TIMERS {
    FUNCTION_FREQUENCY
};

void SYS_SetupTimers();

void SYS_SetTimerInterruptTime(HW_TIMERS timer_name, uint64_t microseconds, bool autoreload = true, uint64_t autoreload_cnt = 0);