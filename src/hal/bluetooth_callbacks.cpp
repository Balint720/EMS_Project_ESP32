#include "hal/bluetooth_callbacks.h"

#include <Arduino.h>
#include <BLEDevice.h>

#include "hal/bluetooth_uuid.h"

#include "sys/convert.h"

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
OurCharacteristicCallbacks::OurCharacteristicCallbacks() : isFuncGenVariableChar(false) {}
OurCharacteristicCallbacks::OurCharacteristicCallbacks(Char_UUID_Index assocFuncGen) : isFuncGenVariableChar(true), assocFuncGen(assocFuncGen) {}
void OurCharacteristicCallbacks::onWrite(BLECharacteristic *pCharacteristic, esp_ble_gatts_cb_param_t* param) {
    Serial.println("Write request received.");
    if (isFuncGenVariableChar) {
        Serial.println("Function generator variable...");
        std::string btVal;
        int res_i;
        double res_d;
        switch (assocFuncGen) {
            case WAVEFORM:
            case PULSEWIDTH:
            case FREQUENCY:
                Serial.println("Pulsewidth or Frequency");
                //btVal = std::string((char*)param->write.value, (size_t)param->write.len);
                res_i = packetToInt((char*)param->write.value, (size_t)param->write.len);

                if (APP_FuncGenChangeRequest(assocFuncGen, res_i)) {
                    pCharacteristic->setValue(param->write.value, param->write.len);
                }
                else {
                    pCharacteristic->setValue(
                        intToPacket(
                            APP_FuncGenGetRequest<int>(assocFuncGen)
                        )
                    );
                }
                break;
            case AMPLITUDE:
                Serial.println("Amplitude");
                double res_d = packetToDouble((char*)param->write.value, (size_t)param->write.len);
                
                if (APP_FuncGenChangeRequest(assocFuncGen, res_d)) {
                    pCharacteristic->setValue(param->write.value, param->write.len);
                }
                pCharacteristic->setValue(
                        doubleToPacket(
                            APP_FuncGenGetRequest<double>(assocFuncGen)
                        )
                    );
                break;
                
        }
    }
    
}