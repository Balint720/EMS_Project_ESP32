#pragma once

#include <WiFi.h>

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

void websiteResponseWiFi(WiFiClient*);

void readResponseWiFi(WiFiClient*);

void wifiServerLoop(WiFiServer*);

void processRequestLine(char*, enum HTTP_Request_Part*);