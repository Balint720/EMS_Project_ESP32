#pragma once

#include "hal/bluetooth_uuid.h"

#include <string>

void HAL_Setup();

std::__cxx11::string HAL_BT_GetValue(Service_UUID_Index, Char_UUID_Index);

void HAL_BT_SetValue(Service_UUID_Index, Char_UUID_Index, const char*);