#pragma once

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

#include "hal/bluetooth_uuid.h"

void HAL_SetupBluetooth();

std::__cxx11::string getCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI);
void setCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI, const char *val);