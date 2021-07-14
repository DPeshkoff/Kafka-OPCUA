// Copyright 2021 DPeshkoff && let-robots-reign;
// Distributed under the GNU General Public License, Version 3.0. (See
// accompanying file LICENSE)
//////////////////////////////////////////////////////////////////////
// NodeMCU module (used as master)
//////////////////////////////////////////////////////////////////////
// TECHNICAL INFORMATION
// NodeMCU 1.0 (ESP-12E module)
// Chip: ESP8266EX
// Flash size: 4 MB
// MMU: 32KB cache + 32 KB IRAM
// CPU Frequency: 80 MHZ
// Upload speed: 115200
////
// SKETCH: 316400 bytes (compressed: 229423)
// GLOBAL VARIABLES: 31280 bytes
//////////////////////////////////////////////////////////////////////
// PRIVATE DATA
#define SSID_IDENTIFIER     ${ssid}
#define PASSWORD_IDENTIFIER ${password}
//////////////////////////////////////////////////////////////////////
// INCLUDES
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <SoftwareSerial.h>
//////////////////////////////////////////////////////////////////////

// SerialLink - bridge with Arduino Uno
SoftwareSerial SerialLink(D6, D5);  // RX, TX

//////////////////////////////////////////////////////////////////////

const char* ssid = SSID_IDENTIFIER;
const char* password = PASSWORD_IDENTIFIER;

// Using 80th port
AsyncWebServer server(80);

// Most recent Arduino answer
StaticJsonDocument<400> latest_json;

//////////////////////////////////////////////////////////////////////

// Using Flash memory
const PROGMEM char html_base[] = R"rawliteral(
<!DOCTYPE HTML>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="description" content="NodeMCU ESP8266 web server">
<meta name="keywords" content="NodeMCU ESP8266,IoT">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>NodeMCU ESP8266</title>
</head>
<body>
<center>
<h2>NodeMCU ESP8266 Web Server</h2>
%IPADDR%
<br/>
<button type="button" onClick="location.href='../update'">Request update</button>
</center>
</body>
</html>
)rawliteral";

//////////////////////////////////////////////////////////////////////

// %IPADDR% variable handling
String html_complete(const String& variable) {
    if (variable == "IPADDR") {
        String ip = "<h3>You are connected to " + WiFi.localIP().toString();
        +"</h3>";
        return ip;
    }
    return String();
}

//////////////////////////////////////////////////////////////////////

void setup() {
    // Serial is used as debug (default: USB, 115200 bauds)
    Serial.begin(115200);

    while (!Serial) {
        continue;
    }

    // SerialLink: 4800 bauds (less amount of errors)
    SerialLink.begin(4800);

    // Connect to WiFi
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);  // Wait to try again
        Serial.print("Connecting to ");
        Serial.println(ssid);
    }

    // Success, output out IP
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", html_base, html_complete);
    });

    server.onNotFound([](AsyncWebServerRequest* request) {
        request->send_P(404, "text/plain", "404 Not Found");
    });

    server.on("/update", HTTP_GET, [](AsyncWebServerRequest* request) {
        String json =
            "{\n\t\"iteration\": " + latest_json["iteration"].as<String>() +
            ",\n\t\"temperature\": " + latest_json["temperature"].as<String>() +
            ",\n\t\"brightness\": " + latest_json["brightness"].as<String>() +
            "\n}";

        request->send(200, "text/json", json);
        Serial.println("Server transferred latest JSON package");
        Serial.println("--------------------");
    });

    // Starting the server
    server.begin();
    Serial.println("Server started successfully");
    Serial.println("--------------------");
}

//////////////////////////////////////////////////////////////////////

void loop() {
    // Request answer from Arduino via SerialLink
    SerialLink.write("GET");

    if (SerialLink.available()) {
        StaticJsonDocument<400> package;

        // Read the JSON document from the SerialLink
        DeserializationError err = deserializeJson(package, SerialLink);

        if (err == DeserializationError::Ok) {
            // Debug output of the answer
            Serial.println("Received JSON from Arduino:");
            Serial.print(" Iteration = ");
            Serial.println(package["iteration"].as<uint32_t>());
            Serial.print(" Temperature = ");
            Serial.println(package["temperature"].as<uint32_t>());
            Serial.print(" Brightness = ");
            Serial.println(package["brightness"].as<uint32_t>());
            Serial.println("--------------------");

            // Saving it for web-server
            latest_json = package;

        } else {
            Serial.print("[Error] deserializeJson() returned ");
            Serial.println(err.c_str());

            // Flushing recent message
            while (SerialLink.available() > 0) {
                SerialLink.read();
            }
        }
    }

    delay(10000);  // Every 10s
}
