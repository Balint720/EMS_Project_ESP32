#include <Arduino.h>
#include <WiFi.h>

#include "app/http_reqs.h"
#include "hal/bluetooth.h"

#define MAX_ATTEMPT_COUNT 10

//const char* WiFi_SSID = "UPC4D8A1AB";
//const char* WiFi_PASS = "avzBed3jhsdx";
const char* WiFi_SSID = "Vodafone-2B15";
const char* WiFi_PASS = "7P3NtMhhbgTp8pUB";

int lightDelay = 10;

WiFiServer server;

// put function declarations here:
void scanNetworks();

void setup() {
  
  pinMode(21, OUTPUT);

  analogWrite(21, 128);
  
  // Serial
  Serial.begin(9600);

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

  while ((status = WiFi.status()) != WL_CONNECTED) {
    Serial.println("Attempting connection to SSID...");
    Serial.print("Wifi status:");
    Serial.print(WiFi.status());
    Serial.print("\n");
    WiFi.begin(WiFi_SSID, WiFi_PASS);      // Attempt joining WiFi
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

  // Setting up server
  server = WiFiServer(80);
  server.begin();
  Serial.println("Server started");

  // Setting up bluetooth
  HAL_SetupBluetooth();

}

void loop() {
  wifiServerLoop(&server);
  /*
  // listen for incoming clients

  WiFiClient client = server.available();

  if (client) {

    Serial.println("new client");

    // an http request ends with a blank line

    bool currentLineIsBlank = true;

    while (client.connected()) {

      if (client.available()) {

        char c = client.read();

        Serial.write(c);

        // if you've gotten to the end of the line (received a newline

        // character) and the line is blank, the http request has ended,

        // so you can send a reply

        if (c == '\n' && currentLineIsBlank) {

          // send a standard http response header

          client.println("HTTP/1.1 200 OK");

          client.println("Content-Type: text/html");

          client.println("Connection: close");  // the connection will be closed after completion of the response

          client.println("Refresh: 5");  // refresh the page automatically every 5 sec

          client.println();

          client.println("<!DOCTYPE HTML>");

          client.println("<html>");

          // output the value of each analog input pin

          for (int analogChannel = 0; analogChannel < 6; analogChannel++) {

            int sensorReading = analogRead(analogChannel);

            client.print("analog input ");

            client.print(analogChannel);

            client.print(" is ");

            client.print(sensorReading);

            client.println("<br />");

          }

          client.println("</html>");

          break;

        }

        if (c == '\n') {

          // you're starting a new line

          currentLineIsBlank = true;

        } else if (c != '\r') {

          // you've gotten a character on the current line

          currentLineIsBlank = false;

        }

      }

    }

    // give the web browser time to receive the data

    delay(1);

    // close the connection:

    client.stop();

    Serial.println("client disconnected");
  }
  */

  
}

void scanNetworks() {
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