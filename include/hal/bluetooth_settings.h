#pragma once

/*
*   Bluetooth configuration file (safe version)
*/

#include "hal/bluetooth_uuid.h"
#include <cstdint>
#include <BLECharacteristic.h>

/*
*   UUID type
*/
enum UUID_TYPE {
    SIG,
    CUSTOM
};

/*
*   Characteristic settings
*/
struct characteristic_settings {
    const char *name;
    const char *uuid;
    Char_UUID_Index uuid_ind;
    UUID_TYPE uuid_type;
    uint32_t properties;
};

/*
*   Service settings
*/
struct service_settings {
    const char *name;
    const char *uuid;
    UUID_TYPE uuid_type;
    int characteristic_count;
    const characteristic_settings *characteristics;   // pointer to array
};

/*
*   Bluetooth settings
*/
struct bluetooth_settings {
    const char *device_name;
    int service_count;
    const service_settings *services;                 // pointer to array
};

/*
*   ---- Static configuration data ----
*/

/* Characteristics for service 1 */
static const characteristic_settings s_s_1_characteristics[] = {
    {
        "Waveform Type",
        char_uuid_map[WAVEFORM],
        WAVEFORM,
        CUSTOM,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    },
    {
        "Frequency [Hz]",
        char_uuid_map[FREQUENCY],
        FREQUENCY,
        CUSTOM,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    },
    {
        "Pulse Width [%]",
        char_uuid_map[PULSEWIDTH],
        PULSEWIDTH,
        CUSTOM,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    },
    {
        "Amplitude [mA]",
        char_uuid_map[AMPLITUDE],
        AMPLITUDE,
        CUSTOM,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    },
    {
        "State [int]",
        char_uuid_map[FG_STATE],
        FG_STATE,
        CUSTOM,
        BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
    }
        
};

/* Service definitions */
static const service_settings services[] = {
    {
        "Function generator settings",
        service_uuid_map[FUNCTIONGEN],
        CUSTOM,
        5,
        s_s_1_characteristics
    },
    {
        "Recipes",
        service_uuid_map[RECIPE],
        CUSTOM,
        0,
        nullptr
    }
};

/* Top-level Bluetooth settings */
static const bluetooth_settings b_s = {
    "EMS Control Device",
    2,
    services
};

/*
*   UUID helper functions
*/
static inline const char* bl_settings_getUUID(const service_settings *ss) {
    return ss->uuid;
}

static inline const char* bl_settings_getUUID(const characteristic_settings *cs) {
    return cs->uuid;
}