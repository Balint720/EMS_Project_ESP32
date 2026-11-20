#pragma once

#include <stdint.h>

namespace HAL {
    void HAL_SetupADC();
    void HAL_LoopADC();

    uint32_t getFgenADCInput();
    uint32_t getFgenADCInput2();
}
