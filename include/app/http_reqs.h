#pragma once

// Forward declarations - WiFi.h is included in http_reqs.cpp
class WiFiClient;
class WiFiServer;

enum Website {
    HELLO_WORLD,
    GOODBYE_WORLD,
    ICON
};

enum HTTP_Request_Part {
    START_LINE,
    HEADER,
    BODY
};

#if INCLUDE_WIFI
void websiteResponseWiFi(WiFiClient*);
void readResponseWiFi(WiFiClient*);
void wifiServerLoop(WiFiServer*);
void processRequestLine(char*, enum HTTP_Request_Part*);
#else
// Stub functions when WiFi is not included
inline void websiteResponseWiFi(WiFiClient*) {}
inline void readResponseWiFi(WiFiClient*) {}
inline void wifiServerLoop(WiFiServer*) {}
inline void processRequestLine(char*, enum HTTP_Request_Part*) {}
#endif