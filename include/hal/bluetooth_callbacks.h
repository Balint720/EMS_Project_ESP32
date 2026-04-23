#pragma once

#include <BLEServer.h>
#include <BLECharacteristic.h>
#include <BLEDescriptor.h>

#include "hal/bluetooth_uuid.h"
#include "app/valchange_request.h"

class OurServerCallbacks : public BLEServerCallbacks {
    public:
        void onConnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) override;
        void onDisconnect(BLEServer* pServer, esp_ble_gatts_cb_param_t *param) override;
};

class OurCharacteristicCallbacks : public BLECharacteristicCallbacks {
    public:
        void onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t* param) override;
        OurCharacteristicCallbacks();
        OurCharacteristicCallbacks(Char_UUID_Index assocFuncGen);
    private:
        bool isFuncGenVariableChar;
        Char_UUID_Index assocFuncGen;
};

class OurDescriptorCallbacks : public BLEDescriptorCallbacks {

};