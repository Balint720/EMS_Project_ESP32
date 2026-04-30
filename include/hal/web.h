#pragma once


#if defined(LOCATION_M)
#define WIFI_SSID "Vodafone-2B15"
#define WIFI_PASS "7P3NtMhhbgTp8pUB"
#elif defined(LOCATION_B)
#define WIFI_SSID "UPC4D8A1AB"
#define WIFI_PASS "avzBed3jhsdx"
#else
#error NO LOCATION DEFINED
#endif

#define MAX_ATTEMPT_COUNT 10

#if INCLUDE_WIFI
void HAL_SetupWiFi();
void HAL_SetupWebServer();
#else
// Stub functions when WiFi is not included
inline void HAL_SetupWiFi() {}
inline void HAL_SetupWebServer() {}
#endif