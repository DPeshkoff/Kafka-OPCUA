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
// SKETCH: 322112 bytes (compressed: 231829) (30%)
// GLOBAL VARIABLES: 31860 bytes (38%)
//////////////////////////////////////////////////////////////////////
// PRIVATE DATA
#define SSID_IDENTIFIER       ${ssid}
#define PASSWORD_IDENTIFIER   ${password}
#define SECRET_SECURITY_TOKEN ${token}
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

// WiFi network name and password
const char* ssid = SSID_IDENTIFIER;
const char* password = PASSWORD_IDENTIFIER;

// Client security token
String SECURITY_TOKEN = SECRET_SECURITY_TOKEN;

// Using 80th port
AsyncWebServer server(80);

// JSON static capacity
const int32_t capacity = 650;  // 128

// Most recent Arduino answer
StaticJsonDocument<capacity> latest_json;

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
<link href="//maxcdn.bootstrapcdn.com/bootstrap/4.1.1/css/bootstrap.min.css" rel="stylesheet" id="bootstrap-css">
<script src="//maxcdn.bootstrapcdn.com/bootstrap/4.1.1/js/bootstrap.min.js"></script>
<script src="//cdnjs.cloudflare.com/ajax/libs/jquery/3.2.1/jquery.min.js"></script>
<link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.3.1/css/all.css"
integrity="sha384-mzrmE5qonljUremFsqc01SB46JvROS7bZs3IO2EmfFsd15uHvIt+Y8vEf7N7fWAU" crossorigin="anonymous">
<style type="text/css">
body, html {
  font-family: Arial, Verdana, sans-serif;
margin: 0;
padding: 0;
height: 100%;
background: #353535;
overflow-x: hidden;
min-height: 100%;
}
.cardbox {
  height: 220px;
  width: 350px;
  margin-top: auto;
  margin-bottom: auto;
  background: #a1a1a1;
  position: relative;
  display: flex;
  justify-content: center;
  flex-direction: column;
  padding: 10px;
  box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
  border-radius: 5px;
}
.cardbox-error {
  height: 100px;
  width: 350px;
  margin-top: auto;
  margin-bottom: auto;
  background: #cc9595;
  position: relative;
  display: flex;
  justify-content: center;
  flex-direction: column;
  padding: 10px;
  box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
  border-radius: 5px;
}
.icon {
  margin-top: 8px;
  margin-left: 5px;
  width: 30px;
}
.main-container {
  margin-top: 20px;
}
h2 {
  font-weight: bold;
  font-size: 20px;
}
.cardbox-answer {
  height: 180px;
  width: 350px;
  margin-top: auto;
  margin-bottom: auto;
  background: #97cc95;
  position: relative;
  display: flex;
  justify-content: center;
  flex-direction: column;
  padding: 10px;
  box-shadow: 0 4px 8px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19);
  border-radius: 5px;
}
h3 {
  font-weight: bold;
  font-size: 14px;
}
.updatebutton {
width: 100%;
background: #2b71c0 !important;
color: white !important;
}
.updatebutton:focus {
box-shadow: none !important;
outline: 0px !important;
}
.input-token:focus {
box-shadow: none !important;
outline: 0px !important;
}
.bottom-links {
margin-top: 15px;
}
.updatebutton:hover {
background: #2b71c0 !important;
color: white !important;
box-shadow: 0 4px 5px 0 rgba(0, 0, 0, 0.2), 0 6px 20px 0 rgba(0, 0, 0, 0.19); 
}
.link {
color: #2865aa;
}
.updatebutton:active {
width: 100%;
background: #353535 !important;
color: white !important;
}
.link:hover {
color: #353535;
}
</style>
</head>
<body>
<script>
function%NOPROTO% Update(%NOPROTO%)%NOPROTO%{
let xmlHttp = new XMLHttpRequest();
xmlHttp.responseType = 'json';
xmlHttp.open("GET", "../update", true); 
xmlHttp.setRequestHeader('Authorization', 'Token ' + document.getElementById('input-token').value);
xmlHttp.send(null);
xmlHttp.onload = function() {
if (xmlHttp.status != 200) {
  document.body.innerHTML += '<div class="container main-container h-100"> <div class="d-flex justify-content-center h-100"> <div class="cardbox-error"> <div class="d-flex justify-content-center"> <h4>Error #' + xmlHttp.status + '</h4></div><div class="d-flex justify-content-center"> <p>' + xmlHttp.statusText;
} else { 
  document.body.innerHTML += '<div class="container main-container h-100"> <div class="d-flex justify-content-center h-100"> <div class="cardbox-answer"> <div class="d-flex justify-content-center"> <h4>Iteration #' + xmlHttp.response.iteration + '</h4></div><div class="d-flex justify-content-center"> <p>Raw temperature: ' + xmlHttp.response.temperature + '</p></div><div class="d-flex justify-content-center"> <p>Raw brightness: ' + xmlHttp.response.brightness;
}
};
}
</script>
<div class="container main-container h-100">
  <div class="d-flex justify-content-center h-100">
    <div class="cardbox">
      <div class="d-flex justify-content-center">
        <h2>NodeMCU ESP8266 Web Server</h2>
      </div>
      <div class="d-flex justify-content-center">
        %IPADDR%
      </div>
      <div class="d-flex justify-content-center main-container">
        <div class="input-group mb-2">
          <div class="input-group-append">
            <span class="icon"><i class="fas fa-key"></i></span>
          </div>
            <input type="text" name="token" id="input-token" class="form-control input-token" value=""
placeholder="Insert token" spellcheck="false"></input>
          </div>
        </div>
       
        <div class="d-flex justify-content-center">
          <button type="button" onclick="Update()" name="button" class="btn updatebutton">Update</button>
        </div>
        <div class="bottom-links">
          <div class="d-flex justify-content-center">
            <p> Visit our <a href="https://github.com/DPeshkoff/Kafka-OPCUA/" class="link"> GitHub</a></p>
          </div>
        </div>
        
      </div>
  </div>
</div>
</body>
</html>
)rawliteral";

//////////////////////////////////////////////////////////////////////

// %IPADDR% variable handling
String html_complete(const String& variable) {
    if (variable == "IPADDR") {
        String ip =
            "<h3>You are connected to " + WiFi.localIP().toString() + "</h3>";
        return ip;
    }
    if (variable == "NOPROTO") {
        return String();
    }
    return String();
}

//////////////////////////////////////////////////////////////////////

void setup() {
    // Serial is used as debug (default: USB, 115200 bauds)
    Serial.begin(115200);

    Serial.setTimeout(5000);

    while (!Serial) {
        continue;
    }

    // SerialLink: 4800 bauds (less amount of errors)
    SerialLink.begin(4800);

    SerialLink.setTimeout(5000);

    // Connect to WiFi
    WiFi.begin(ssid, password);

    Serial.printf("\nConnecting to %s.", ssid);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);  // Wait to try again
        Serial.print(".");
    }
    Serial.println("successful");

    // Success, output out IP
    Serial.println(WiFi.localIP());

    server.on("/", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send_P(200, "text/html", html_base, html_complete);
    });

    server.onNotFound([](AsyncWebServerRequest* request) {
        request->send(404, "text/plain", "404 Not Found");
    });

    server.on("/update", HTTP_GET, [](AsyncWebServerRequest* request) {
        if (request->hasHeader("Authorization")) {
            AsyncWebHeader* auth = request->getHeader("Authorization");
            Serial.printf("User tried to authorize with: %s\n",
                          auth->value().c_str());
            if (auth->value() == SECURITY_TOKEN) {
                String json =
                    "{\n\t\"iteration\": " +
                    latest_json["iteration"].as<String>() +
                    ",\n\t\"temperature\": " +
                    latest_json["temperature"].as<String>() +
                    ",\n\t\"brightness\": " +
                    latest_json["brightness"].as<String>() +
                    "\n}";

                request->send(200, "text/json", json);
                Serial.println("Server transferred latest JSON package");
                Serial.println("--------------------");
            } else {
                Serial.println("Authorization failed: wrong token");
                request->send(401, "text/plain",
                              "401 Unauthorized: wrong token");
            }
        } else {
            Serial.println("Authorization failed: no token provided");
            AsyncWebServerResponse* response = request->beginResponse(
                401, "text/plain", "401 Unauthorized: no token provided");
            response->addHeader("WWW-Authenticate", "Token $TOKEN");
            request->send(response);
        }
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
        StaticJsonDocument<capacity> package;

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
