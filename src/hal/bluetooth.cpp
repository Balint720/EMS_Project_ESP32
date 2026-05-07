#include "hal/bluetooth.h"

#include "hal/bluetooth_settings.h"
#include "hal/bluetooth_callbacks.h"

#include "sys/sys.h"

#include <Arduino.h>

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLEDescriptor.h>

#define DESC_CHARACTERISTIC_USER_DESCRIPTION "2901"

// BLEServer available for all functions within this file
static BLEServer *pServer = nullptr;

void clientDisconnectCallback();

/*
*   Bluetooth Setup for Hardware Layer
*/
void HAL_SetupBluetooth() {
    debug_print("Setting up Bluetooth Low Energy\n");
    
    BLEDevice::init(b_s.device_name);
    
    pServer = BLEDevice::createServer();
    pServer->setCallbacks(new OurServerCallbacks());

    // Services
    BLEService *pServices[b_s.service_count];
    BLECharacteristic **pCharacteristics[b_s.service_count];
    for (int i = 0; i < b_s.service_count; i++) {
        // Save current to make more readable
        const struct service_settings *curr_s_s = &b_s.services[i];
        
        pServices[i] = pServer->createService(bl_settings_getUUID(curr_s_s));
        
        // Adding characteristics
        if (curr_s_s->characteristic_count > 0) {
            // Maybe should consider not using C memory allocation...
            pCharacteristics[i] = (BLECharacteristic**)malloc(sizeof(*pCharacteristics) * curr_s_s->characteristic_count);

            debug_print("Characteristic count: %d\n", curr_s_s->characteristic_count);
            for (int j = 0; j < curr_s_s->characteristic_count; j++) {
                // Save current to make more readable
                const struct characteristic_settings *curr_c_s = &curr_s_s->characteristics[j];
                debug_print("Current characteristic being set: %s\n", curr_c_s->name);

                pCharacteristics[i][j] = pServices[i]->createCharacteristic(bl_settings_getUUID(curr_c_s), curr_c_s->properties);
                
                // Setting default value (here is where we should load stored value from memory)
                pCharacteristics[i][j]->setValue("\x0A");

                // Setting callback
                pCharacteristics[i][j]->setCallbacks(new OurCharacteristicCallbacks(curr_c_s->uuid_ind));

                // Adding description for characteristic
                // Is using new in 2026 the way?
                BLEDescriptor *desc = new BLEDescriptor(DESC_CHARACTERISTIC_USER_DESCRIPTION);
                desc->setValue(curr_c_s->name);
                desc->setAccessPermissions(ESP_GATT_PERM_READ);

                pCharacteristics[i][j]->addDescriptor(desc);

                pServices[i]->addCharacteristic(pCharacteristics[i][j]);
            }
        }
        
        // Starting each service after it gets created
        pServices[i]->start();
    }

    // Advertising the services we created
    BLEAdvertising *pAdvertising = pServer->getAdvertising();

    for (int i = 0; i < b_s.service_count; i++) {
        pAdvertising->addServiceUUID(b_s.services[i].uuid);
    }
    
    // These commands are copied from this example:
    // https://github.com/espressif/arduino-esp32/blob/master/libraries/BLE/examples/Server/Server.ino
    // It seems they have to do something with the interval of advertisements sent out?
    // https://www.bluetooth.com/wp-content/uploads/Files/Specification/HTML/CSS_v11/out/en/supplement-to-the-bluetooth-core-specification/data-types-specification.html#UUID-30c6e42e-5327-f52c-d79e-1b174095c712
    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);
    pAdvertising->setMaxPreferred(0x12);

    BLEDevice::startAdvertising();
}


std::string getCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI) {
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

void setCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI, std::string val) {
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

void setCharacteristicValue(Service_UUID_Index sI, Char_UUID_Index cI, const char* val) {
    setCharacteristicValue(sI, cI, std::string(val));
}

void clientDisconnectCallback() {

}