#include "hal/web.h"

#include <WiFi.h>
#include <Arduino.h>

#include "app/http_reqs.h"

static WiFiServer server;

static void scanNetworks();

void HAL_SetupWiFi() {
    scanNetworks();
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }

  // Setting up WiFi
  int status = WL_IDLE_STATUS;
  int attemptCount = 0;

  while ((status = WiFi.status()) != WL_CONNECTED && attemptCount++ < MAX_ATTEMPT_COUNT) {
    Serial.println("Attempting connection to SSID...");
    Serial.print("Wifi status:");
    Serial.print(WiFi.status());
    Serial.print("\n");
    WiFi.begin(WIFI_SSID, WIFI_PASS);      // Attempt joining WiFi
    delay(10000);         // Wait 10 seconds
  }
  
  if (attemptCount >= MAX_ATTEMPT_COUNT) {
    Serial.println("Failed to connect to WiFi");
    
  }
  else {
    Serial.println("Connected to WiFi");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }
}

void HAL_SetupWebServer() {
    // Setting up server
    server = WiFiServer(80);
    server.begin();
    Serial.println("Server started");
}

void HAL_WebServerLoop() {
    wifiServerLoop(&server);
}

static void scanNetworks() {
  // scan for nearby networks:
  Serial.println("** Scan Networks **");
  byte numSsid = WiFi.scanNetworks();

  // print the list of networks seen:
  Serial.print("SSID List:");
  Serial.println(numSsid);
  // print the network number and name for each network found:
  for (int thisNet = 0; thisNet < numSsid; thisNet++) {
    Serial.print(thisNet);
    Serial.print(") Network: ");
    Serial.println(WiFi.SSID(thisNet));
  }
}