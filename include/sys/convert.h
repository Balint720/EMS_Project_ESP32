#pragma once

#include <string>
#include <cstring>

#include <limits>

typedef uint8_t byte;

inline static int packetToInt(std::string data) {
#if defined(INT_DATAPACKET_BIG_ENDIAN)
    int res = 0;
    for (uint8_t i : data) {
        res <<= 8;
        res |= i;
    }
    return res;
#elif defined(INT_DATAPACKET_LITTLE_ENDIAN)
    int res = 0;
    int shiftcount = 0;
    for (uint8_t i : data) {
        res |= (i << shiftcount);
        shiftcount += 8;
    }
    return res;
#else
#error NO ENDIANNESS OF INTEGER DATA DEFINED
#endif
}

inline static double packetToDouble(std::string data) {
    if (data.length() != 4 && data.length() != 8) {
        return std::numeric_limits<double>::quiet_NaN();
    }
#if defined(FLOAT_DATAPACKET_LITTLE_ENDIAN)
    byte *res = new byte(data.length());
    for (int i = 0; i < data.length(); i++) {
        res[i] = data[i];
    }
    if (data.length() == 4) {
        float f = *((float*)res);
        double d = f;
        return d;
    }
    else {
        double d = *((double*) res);
        return d;
    }
#elif defined(FLOAT_DATAPACKET_BIG_ENDIAN)
    byte *res = new byte(data.length());
    for (int i = 0; i < data.length(); i++) {
        res[i] = data[data.length() - 1 - i];
    }
    if (data.length() == 4) {
        float f = *((float*)res);
        double d = f;
        return d;
    }
    else {
        double d = *((double*) res);
        return d;
    }
#else
#error NO ENDIANNESS OF INTEGER DATA DEFINED
#endif
}

inline static std::string intToPacket(int data, size_t size = sizeof(int)) {
#if defined(INT_DATAPACKET_BIG_ENDIAN)
    std::string res;
    res.resize(size);
    for (size_t i = size - 1; i >= 0; i--) {
        res[i] = (data >> (8 * i)) & 0xFF;
    }
    return res;
#elif defined(INT_DATAPACKET_LITTLE_ENDIAN)
    std::string res;
    res.resize(size);
    for (size_t i = 0; i < size; i++) {
        res[i] = (data >> (8 * i)) & 0xFF;
    }
    return res;
#else
    #error NO ENDIANNESS OF INTEGER DATA DEFINED
#endif
}

inline static std::string doubleToPacket(double data) {
#if defined(FLOAT_DATAPACKET_LITTLE_ENDIAN)
    std::string res;
    byte *bData = new byte(sizeof(double));
    memcpy(bData, &data, sizeof(double));
    res.resize(sizeof(double));
    for (size_t i = 0; i < sizeof(double); i++) {
        res[i] = bData[i];
    }
    return res;
#elif defined(FLOAT_DATAPACKET_BIG_ENDIAN)
    std::string res;
    byte *bData = new byte(sizeof(double));
    memcpy(bData, &data, sizeof(double));
    res.resize(sizeof(double));
    for (size_t i = 0; i < sizeof(double); i++) {
        res[i] = bData[sizeof(double) - 1 - i];
    }
    return res;
#else
    #error NO ENDIANNESS OF FLOAT DATA DEFINED
#endif
}

inline static int packetToInt(const char* data, size_t len) {
    return packetToInt(std::string(data, len));
}
inline static double packetToDouble(const char* data, size_t len) {
    return packetToDouble(std::string(data, len));
}

inline static uint16_t combine8to16(uint8_t a, uint8_t b) {
    uint16_t val;
    val = (a << 8) | b;
    return val;
}