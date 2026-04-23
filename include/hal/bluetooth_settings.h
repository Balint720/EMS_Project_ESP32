#pragma once

/*
*   Bluetooth configuration file
*/

#include "hal/bluetooth_uuid.h"

#include <cstdint>
#include <BLECharacteristic.h>


union uuid_t {
    char sig[4+1];
    char custom[32+4+1];
};

enum UUID_TYPE {
    SIG,
    CUSTOM
};

struct characteristic_settings {
    const char *name;
    const char* uuid;
    const enum Char_UUID_Index uuid_ind;
    const enum UUID_TYPE uuid_type;
    const uint32_t properties;
};

struct service_settings {
    const char *name;
    const char* uuid;
    enum UUID_TYPE uuid_type;
    const int characteristic_count;
    const struct characteristic_settings characteristics[];
};

struct bluetooth_settings {
    const char *device_name;
    const int service_count;
    const struct service_settings services[];
};

static constexpr struct service_settings s_s_1 {
    .name = "Function generator settings",
    .uuid = service_uuid_map[Service_UUID_Index::FUNCTIONGEN],
    .uuid_type = CUSTOM,
    .characteristic_count = 4,
    .characteristics = {
        {
            .name = "Waveform Type",
            .uuid = char_uuid_map[WAVEFORM],
            .uuid_ind = WAVEFORM,
            .uuid_type = CUSTOM,
            .properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
        },
        {
            .name = "Frequency [Hz]",
            .uuid = char_uuid_map[FREQUENCY],
            .uuid_ind = FREQUENCY,
            .uuid_type = CUSTOM,
            .properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
        },
        {
            .name = "Pulse Width [%]",
            .uuid = char_uuid_map[PULSEWIDTH],
            .uuid_ind = PULSEWIDTH,
            .uuid_type = CUSTOM,
            .properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
        },
        {
            .name = "Amplitude [mA]",
            .uuid = char_uuid_map[AMPLITUDE],
            .uuid_ind = AMPLITUDE,
            .uuid_type = CUSTOM,
            .properties = BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
        }
    }
};


static constexpr struct bluetooth_settings b_s {
    .device_name = "EMS Control Device",
    .service_count = 1,
    .services = {
        s_s_1
    }
};

static const char* bl_settings_getUUID(const struct service_settings *ss) {
    switch (ss->uuid_type) {
        case SIG:
            return ss->uuid;
        case CUSTOM:
            return ss->uuid;
        default:
            return ss->uuid;
    }
}

static const char* bl_settings_getUUID(const struct characteristic_settings *cs) {
    switch (cs->uuid_type) {
        case SIG:
            return cs->uuid;
        case CUSTOM:
            return cs->uuid;
        default:
            return cs->uuid;
    }
}

