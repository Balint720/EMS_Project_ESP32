#include "sys/timer.h"

#include <Arduino.h>
#include <esp32-hal-timer.h>

#include <FunctionalInterrupt.h>
#include "hal/dac.h"

static hw_timer_t *timer1;


static uint8_t testOutput = 0;

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
    SYS_SetTimerInterruptTime(HW_TIMERS::FUNCTION_FREQUENCY, 500000UL);
    timerAttachInterrupt(timer1, []() {
        digitalWrite(PIN_FGENOUTPUT_LED, testOutput);
        testOutput = !testOutput;
    });
}

void SYS_SetTimerInterruptTime(HW_TIMERS timer_name, uint64_t microseconds, bool autoreload, uint64_t autoreload_cnt) {
    timerAlarm(timerEnumToTimer(timer_name), microseconds, autoreload, autoreload_cnt);
}

void SYS_AttachFunctionToTimer(std::function<void>(void)) {
    
}

void SYS_SetupTimers() {
    pinMode(PIN_FGENOUTPUT_LED, OUTPUT);
    setupTimer();
}