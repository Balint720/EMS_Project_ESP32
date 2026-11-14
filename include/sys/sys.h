#pragma once

#include <Arduino.h>

#ifdef PRINT_DEBUG_SERIAL
    #define debug_print(x, ...) Serial.printf(x, ##__VA_ARGS__)
#else
    #define debug_print(x, ...) ;
#endif

void SYS_Setup();