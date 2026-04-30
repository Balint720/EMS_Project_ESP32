#pragma once

#include <string>

#include "hal/bluetooth_uuid.h"

void HAL_SetupBluetooth();

std::string getCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI);
void setCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI, const char *val);