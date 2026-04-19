#include "app/http_reqs.h"
#include <string.h>

// Globals
static enum Website g_currWeb = HELLO_WORLD;

void websiteResponseWiFi(WiFiClient* client) {
    // Response Header
    client->println("HTTP/1.1 200 OK");

    // Response Content
    switch (g_currWeb) {
        case HELLO_WORLD:
            client->println("Content-Type: text/html");
            client->println("");
            client->println("<!DOCTYPE HTML>");
            client->println("<html>");
            client->println("<head>");
            client->println("<title>ESP32 Web Server</title>");
            client->println("</head>");
            client->println("<body>");
            client->println("<h1>Hello World!</h1>");
            client->println("</body>");
            client->println("</html>");
            break;
        case GOODBYE_WORLD:
            client->println("Content-Type: text/html");
            client->println("");
            client->println("<!DOCTYPE HTML>");
            client->println("<html>");
            client->println("<head>");
            client->println("<title>ESP32 Web Server</title>");
            client->println("</head>");
            client->println("<body>");
            client->println("<h1>Goodbye World!</h1>");
            client->println("</body>");
            client->println("</html>");
            break;
        case ICON:
            client->println("Content-Type: image/x-icon");
            client->println("");
            break;
    }
    
}

void wifiServerLoop(WiFiServer* server) {
    WiFiClient client = server->available();

    if (client) {
        readResponseWiFi(&client);
        delay(1);

        client.stop();

        Serial.println("Client disconnected");
    }
}

void readResponseWiFi(WiFiClient* client) {
    enum HTTP_Request_Part currPartOfReq = START_LINE;
    char line[255];
    int i = 0;
    bool emptyLine = true;
    while (client->connected()) {
        if (client->available()) {
            int c = client->read();

            if (c == '\n' && emptyLine) {
                websiteResponseWiFi(client);
                break;
            }

            if (c == '\n') {
                emptyLine = true;
                line[i] = '\0';
                Serial.println(line);
                processRequestLine(line, &currPartOfReq);
                i = 0;
            } else if (c != '\r') {
                emptyLine = false;
                line[i++] = c;
            }
        }
    }
    
}

void processRequestLine(char* line, enum HTTP_Request_Part* part) {
    switch (*part) {
        case START_LINE:
        {
            Serial.println("START_LINE");
            char* word = strtok(line, " ");
            while (word) {
                if (strcmp(word, "/hello") == 0) {
                    Serial.println("Word is hello");
                    g_currWeb = HELLO_WORLD;
                }
                else if (strcmp(word, "/goodbye") == 0) {
                    Serial.println("Word is goodbye");
                    g_currWeb = GOODBYE_WORLD;
                }
                else if (strcmp(word, "/favico.ico") == 0) {
                    g_currWeb = ICON;
                }
                word = strtok(NULL, " ");
            }
            (*part) = HEADER;
            break;
        }
        case HEADER:
            break;
        case BODY:
            break;
        default:
            Serial.println("Error: HTTP_Request_Part unidentified");
            break;
    }
}