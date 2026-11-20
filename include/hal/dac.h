#pragma once

#include <stdint.h>
#include <app/funcgen_state.h>

namespace HAL {
    void HAL_SetupDAC();
    void HAL_LoopDAC();
    void HAL_SetDirectDAC(WaveOutput value);

    uint32_t getFgenADCInput();
    uint32_t getFgenADCInput2();
}