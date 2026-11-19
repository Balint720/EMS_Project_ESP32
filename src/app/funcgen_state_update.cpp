#include "sys/funcgen_state.h"
#include "hal/timer.h"
#include "app/function_gen_vars.h"
#include "app/funcgen_state_update.h"

#include "esp32-hal-timer.h"

#include <cmath>
#include <limits>
#include <hal/dac.h>
#include <sys/sys.h>

static esp_timer_handle_t squareWaveTimer;
static uint32_t squarePeriodUs = 1000000;
static uint32_t squarePulseWidthUs = 500000;
static bool squareWaveRunning = false;

static uint32_t getPulseWidthForPeriod() {
    const uint32_t halfPeriodUs = squarePeriodUs / 2;
    if (halfPeriodUs <= 1) {
        return 1;
    }
    return squarePulseWidthUs < halfPeriodUs ? squarePulseWidthUs : halfPeriodUs - 1;
}

static uint32_t getDelayForState(SquareWaveState state) {
    const uint32_t pulseWidthUs = getPulseWidthForPeriod();
    const uint32_t halfPeriodUs = squarePeriodUs / 2;

    switch (state) {
        case A_PLUS:
        case A_MINUS:
            return pulseWidthUs;
        case NEUTRAL:
        case NEUTRAL2:
            return halfPeriodUs > pulseWidthUs ? halfPeriodUs - pulseWidthUs : 1;
        default:
            return 1;
    }
}

static void scheduleNextState(bool stopExisting = true);

static void advanceSquareWave(void*) {
    if (!squareWaveRunning) {
        return;
    }

    SYS_CurrWave.gotoNextState();
    // HAL_SetDirectDAC(SYS_CurrWave.getOutput());
    scheduleNextState(false);
}

static void scheduleNextState(bool stopExisting) {
    if (!squareWaveRunning || squareWaveTimer == nullptr) {
        return;
    }

    if (stopExisting) {
        esp_timer_stop(squareWaveTimer);
    }
    esp_timer_start_once(squareWaveTimer,
        static_cast<uint64_t>(getDelayForState(SYS_CurrWave.getState())));
}

void APP_SetupSquareWaveTimer() {
    if (squareWaveTimer != nullptr) {
        return;
    }

    esp_timer_create_args_t settings{};
    settings.callback = advanceSquareWave;
    settings.arg = nullptr;
    settings.dispatch_method = ESP_TIMER_TASK;
    settings.name = "squareWaveTimer";
    ESP_ERROR_CHECK(esp_timer_create(&settings, &squareWaveTimer));
    debug_print("Set up Square Wave Timer\n");
}

void APP_StartSquareWaveTimer(double frequency_hz, uint32_t pulse_width_us) {
    APP_SetupSquareWaveTimer();
    APP_UpdateSquareWaveFrequency(frequency_hz);
    APP_UpdateSquareWavePulseWidth(pulse_width_us);

    esp_timer_stop(squareWaveTimer);
    squareWaveRunning = true;
    SYS_CurrWave.turnOn();
    SYS_CurrWave.gotoGivenState(A_PLUS);
    scheduleNextState();
}

void APP_UpdateSquareWavePulseWidth(uint32_t pulse_width_us) {
    squarePulseWidthUs = pulse_width_us;

    if (squareWaveRunning) {
        scheduleNextState();
    }
}

void APP_UpdateSquareWaveFrequency(double frequency_hz) {
    if (!std::isfinite(frequency_hz) || frequency_hz <= 0.0) {
        return;
    }

    double periodUs = 1000000.0 / frequency_hz;
    if (periodUs < 2.0) {
        periodUs = 2.0;
    }
    else if (periodUs > static_cast<double>(std::numeric_limits<uint32_t>::max())) {
        periodUs = std::numeric_limits<uint32_t>::max();
    }
    squarePeriodUs = static_cast<uint32_t>(periodUs);

    if (squareWaveRunning) {
        scheduleNextState();
    }
}

void APP_StopSquareWaveTimer() {
    squareWaveRunning = false;
    if (squareWaveTimer != nullptr) {
        esp_timer_stop(squareWaveTimer);
    }
    SYS_CurrWave.turnOff();
    SYS_CurrWave.gotoGivenState(NEUTRAL);
    // HAL_SetDirectDAC(SYS_CurrWave.getOutput());
}