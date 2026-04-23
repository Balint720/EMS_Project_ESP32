#pragma once

#include "hal/bluetooth_uuid.h"

#include <string>

void HAL_Setup();
void HAL_Loop();

std::string HAL_BT_GetValue(Service_UUID_Index, Char_UUID_Index);

void HAL_BT_SetValue(Service_UUID_Index, Char_UUID_Index, const char*);
void HAL_BT_SetValue(Service_UUID_Index, Char_UUID_Index, std::string);