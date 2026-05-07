#pragma once

#include <BLEServer.h>
#include <BLECharacteristic.h>
#include <BLEDescriptor.h>

#include "hal/bluetooth_uuid.h"

class OurServerCallbacks : public BLEServerCallbacks {
    public:
        void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) override;
        void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) override;
};

class OurCharacteristicCallbacks : public BLECharacteristicCallbacks {
    public:
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t* param) override;
        OurCharacteristicCallbacks(Char_UUID_Index assocChar);
    private:
        Char_UUID_Index assocChar;
};

class OurDescriptorCallbacks : public BLEDescriptorCallbacks {

};