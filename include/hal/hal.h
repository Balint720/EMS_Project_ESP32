#pragma once

#include "hal/bluetooth/bluetooth_uuid.h"

#include "app/function_gen_recipe.h"

#include <string>

/*
*   Base Hardware Abstraction Layer functions
*/
namespace HAL {
    void    HAL_Setup();
    void    HAL_Loop();
    void    HAL_ReactToValueChange(Char_UUID_Index);
}
