#include "hal/hal.h"

#include "hal/bluetooth.h"
#include "hal/bluetooth_uuid.h"
#include "hal/web.h"

void HAL_Setup() {
    //HAL_SetupWiFi();
    //HAL_SetupWebServer();
    HAL_SetupBluetooth();
}

void HAL_Loop() {

}

std::string HAL_BT_GetValue(Service_UUID_Index sI, Char_UUID_Index cI) {
    
    return getCharacteristicValue(sI, cI);
}

void HAL_BT_SetValue(Service_UUID_Index sI, Char_UUID_Index cI, const char *val) {
    setCharacteristicValue(sI, cI, val);
}

void HAL_BT_SetValue(Service_UUID_Index sI, Char_UUID_Index cI, std::string val) {
    setCharacteristicValue(sI, cI, val.c_str());
}