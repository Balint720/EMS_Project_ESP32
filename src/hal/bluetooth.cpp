#include "hal/bluetooth.h"

#include "hal/bluetooth_settings.h"

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

static BLEServer *pServer = nullptr;

void HAL_SetupBluetooth() {
    Serial.println("Setting up Bluetooth Low Energy");

    BLEDevice::init(b_s.device_name);
    
    pServer = BLEDevice::createServer();

    // Services
    BLEService *pServices[b_s.service_count];
    BLECharacteristic **pCharacteristics[b_s.service_count];
    for (int i = 0; i < b_s.service_count; i++) {
        const struct service_settings *curr_s_s = &b_s.services[i];

        pServices[i] = pServer->createService(bl_settings_getUUID(curr_s_s));
        
        if (curr_s_s->characteristic_count > 0) {
            pCharacteristics[i] = (BLECharacteristic**)malloc(sizeof(*pCharacteristics) * curr_s_s->characteristic_count);

            for (int j = 0; j < curr_s_s->characteristic_count; j++) {
                const struct characteristic_settings *curr_c_s = &curr_s_s->characteristics[j];

                pCharacteristics[i][j] = pServices[i]->createCharacteristic(bl_settings_getUUID(curr_c_s), curr_c_s->properties);
                pCharacteristics[i][j]->setValue("Hello");

                pServices[i]->addCharacteristic(pCharacteristics[i][j]);
            }
        }

        pServices[i]->start();
    }
    /*
    BLEService *pService = pServer->createService(SERVICE_UUID);
    BLECharacteristic *pCharacteristic_WF =
        pService->createCharacteristic(CHARACTERISTIC_UUID_WF, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);

    pCharacteristic_WF->setValue("Hello world says Neil");
    pService->addCharacteristic(pCharacteristic_WF);
    pService->start();
    */
    BLEAdvertising *pAdvertising = pServer->getAdvertising();

    for (int i = 0; i < b_s.service_count; i++) {
        pAdvertising->addServiceUUID(b_s.services[i].uuid);
    }
    
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMaxPreferred(0x12);
    BLEDevice::startAdvertising();
    Serial.println("Characteristic defined! Now you can read it in your phone.");
}


std::__cxx11::string getCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI) {
    if (pServer == nullptr) {
        Serial.println("ERROR: Bluetooth Server is not initialized!!!");
        return "";
    }

    BLEService *pService = pServer->getServiceByUUID(service_uuid_map[sI]);
    if (pService == nullptr) {
        Serial.println("ERROR: Service with given uuid doesn't exist.");
        return "";
    }

    BLECharacteristic *pChar = pService->getCharacteristic(char_uuid_map[cI]);
    if (pChar == nullptr) {
        Serial.println("ERROR: Characteristic with given uuid doesn't exist.");
        return "";
    }

    return pChar->getValue();
}

void setCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI, const char *val) {
    if (pServer == nullptr) {
        Serial.println("ERROR: Bluetooth Server is not initialized!!!");
        return;
    }

    BLEService *pService = pServer->getServiceByUUID(service_uuid_map[sI]);
    if (pService == nullptr) {
        Serial.println("ERROR: Service with given uuid doesn't exist.");
        return;
    }

    BLECharacteristic *pChar = pService->getCharacteristic(char_uuid_map[cI]);
    if (pChar == nullptr) {
        Serial.println("ERROR: Characteristic with given uuid doesn't exist.");
        return;
    }

    pChar->setValue(val);
}