#include "hal/bluetooth_callbacks.h"

#include <Arduino.h>
#include <BLEDevice.h>

#include "hal/bluetooth_uuid.h"

#include "app/valchange_request.h"

#include "sys/convert.h"
#include "sys/sys.h"

/*
*   Server
*/
void OurServerCallbacks::onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    BLEDevice::stopAdvertising();
}
void OurServerCallbacks::onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) {
    BLEDevice::startAdvertising();
}



/*
*   Characteristic
*/
OurCharacteristicCallbacks::OurCharacteristicCallbacks(Char_UUID_Index assocChar) : assocChar(assocChar) {}
void OurCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t* param) {
    debug_print("Write request received.\n");
    std::string btVal;
    int res_i;
    double res_d;
    uint8_t info[2];
    switch (assocChar) {
        case Char_UUID_Index::WAVEFORM:
        case Char_UUID_Index::PULSEWIDTH:
        case Char_UUID_Index::FG_STATE:
        case Char_UUID_Index::RECIPE_WAVEFORM:
        case Char_UUID_Index::RECIPE_PULSEWIDTH:
        {
            res_i = packetToInt((char*)param->write.value, (size_t)param->write.len);

            if (APP_FuncGenChangeRequest(assocChar, res_i)) {
                pCharacteristic->setValue(param->write.value, param->write.len);
            }
            else {
                pCharacteristic->setValue(
                    intToPacket(
                        APP_FuncGenGetRequest<int>(assocChar)
                    )
                );
            }
            break;
        }
        case Char_UUID_Index::AMPLITUDE:
        case Char_UUID_Index::FREQUENCY:
        case Char_UUID_Index::RECIPE_AMPLITUDE:
        case Char_UUID_Index::RECIPE_FREQUENCY:
        {
            double res_d = packetToDouble((char*)param->write.value, (size_t)param->write.len);
            
            if (APP_FuncGenChangeRequest(assocChar, res_d)) {
                pCharacteristic->setValue(param->write.value, param->write.len);
            }
            else {
                pCharacteristic->setValue(
                    doubleToPacket(
                        APP_FuncGenGetRequest<double>(assocChar)
                    )
                );
            }
            break;
        }
        case Char_UUID_Index::RECIPE_INFO:
        {
            // Multiple uint8_t values stored: one is selected recipe, other is selected step
            info[0] = packetToInt((char*)param->write.value, sizeof(uint8_t));
            info[1] = packetToInt((char*)(param->write.value + 1), sizeof(uint8_t));

            debug_print("First value: %d\nSecond value: %d\n", info[0], info[1]);

            if (APP_FuncGenChangeRequest(assocChar, info[0], nullptr, 0)) {
                btVal = std::string((char*)param->write.value, sizeof(uint8_t));
            }
            else {
                btVal = intToPacket(APP_FuncGenGetRequest<uint8_t>(assocChar, 0));
            }
            
            if (APP_FuncGenChangeRequest(assocChar, info[1], nullptr, 1)) {
                btVal.append(std::string((char*)(param->write.value+1), sizeof(uint8_t)));
            }
            else {
                btVal.append(intToPacket(APP_FuncGenGetRequest<uint8_t>(assocChar, 1)));
            }

            pCharacteristic->setValue(btVal);
            break;
        }
    }
    
}