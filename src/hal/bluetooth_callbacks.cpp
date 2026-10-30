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
    switch (assocChar) {
        case Char_UUID_Index::WAVEFORM:
        case Char_UUID_Index::PULSEWIDTH:
        case Char_UUID_Index::FG_STATE:
        case Char_UUID_Index::RECIPE_WAVEFORM:
        case Char_UUID_Index::RECIPE_PULSEWIDTH:
        case Char_UUID_Index::RECIPE_TIME:
        {
            int res_i = packetToInt((char*)param->write.value, (size_t)param->write.len);

            if (APP_FuncGenChangeRequest(assocChar, res_i)) {
                pCharacteristic->setValue(param->write.value, param->write.len);
            }
            else {
                const std::string value = intToPacket(
                    APP_FuncGenGetRequest<int>(assocChar)
                );
                pCharacteristic->setValue(
                    reinterpret_cast<const uint8_t *>(value.data()),
                    value.size()
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
                const std::string value = doubleToPacket(
                    APP_FuncGenGetRequest<double>(assocChar)
                );
                pCharacteristic->setValue(
                    reinterpret_cast<const uint8_t *>(value.data()),
                    value.size()
                );
            }
            break;
        }
        case Char_UUID_Index::RECIPE_INFO:
        {
            // Multiple uint8_t values stored: one is selected recipe, other is selected step
            uint16_t info = packetToInt((char*)param->write.value, 2*sizeof(uint8_t));

            debug_print("Info value in hex: %x\n", info);

            APP_FuncGenChangeRequest<uint16_t>(assocChar, info);

            

            break;
        }
        case Char_UUID_Index::RECIPE_ACTIONS:
        {
            // Multiple uint8_t values stored: one is selected recipe, other is selected step
            uint16_t info = packetToInt((char*)param->write.value, 2*sizeof(uint8_t));

            debug_print("Info value in hex: %x\n", info);

            APP_FuncGenChangeRequest<uint16_t>(assocChar, info);
            break;
        }
    }
    
}