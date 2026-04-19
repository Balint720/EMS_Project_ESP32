#include "hal/hal.h"

#include "hal/bluetooth.h"
#include "hal/bluetooth_uuid.h"

void HAL_Setup() {
    HAL_SetupBluetooth();
}


std::__cxx11::string HAL_BT_GetValue(Service_UUID_Index sI, Char_UUID_Index cI) {
    return getCharacteristicValue(sI, cI);
    std::__cxx11::string s;
}

void HAL_BT_SetValue(Service_UUID_Index sI, Char_UUID_Index cI, const char *val) {
    setCharacteristicValue(sI, cI, val);
}