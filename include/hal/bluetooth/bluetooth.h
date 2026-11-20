#pragma once

#include <string>

#include "hal/bluetooth/bluetooth_uuid.h"

namespace HAL {
    void    HAL_SetupBluetooth();

    int     HAL_BT_GetValueInt(Service_UUID_Index, Char_UUID_Index);
    float   HAL_BT_GetValueFloat(Service_UUID_Index, Char_UUID_Index);
    double  HAL_BT_GetValueDouble(Service_UUID_Index, Char_UUID_Index);

    void    HAL_BT_SetValueInt(Service_UUID_Index, Char_UUID_Index, int);
    void    HAL_BT_SetValueFloat(Service_UUID_Index, Char_UUID_Index, float);
    void    HAL_BT_SetValueDouble(Service_UUID_Index, Char_UUID_Index, double);

    std::string     getCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI);
    void            setCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI, const std::string &val);
}
