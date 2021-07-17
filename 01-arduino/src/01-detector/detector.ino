// Copyright 2021 DPeshkoff && let-robots-reign;
// Distributed under the GNU General Public License, Version 3.0. (See
// accompanying file LICENSE)
//////////////////////////////////////////////////////////////////////
// TECHNICAL INFORMATION
// Arduino/Genuino Uno
////
// SKETCH: 7182 bytes (22%)
// GLOBAL VARIABLES: 446 bytes (21%)
//////////////////////////////////////////////////////////////////////
// INCLUDES
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
//////////////////////////////////////////////////////////////////////

// SerialLink - bridge with NodeMCU
SoftwareSerial SerialLink(5, 6);  // RX, TX

// JSON static capacity
const int32_t capacity = 600;

// Global iteration counter
uint32_t iteration = 0;

// Define if we have already send JSON package
bool sent_json = false;

// Seven-segment Dispay numbers
// Segmentation: [A|B|C|D|E|F|G|DP]
byte NumberSegments[10] = {
    0b11111101, 0b01100000, 0b11011010, 0b11110010, 0b01100110,
    0b10110110, 0b10111111, 0b11100000, 0b11111110, 0b11110111,
};

// Seven-segment Dispay numbers
byte DisplayPins[8] = {
    3, 13, 12, 11, 10, 9, 7, 8,
};

//////////////////////////////////////////////////////////////////////

void setup() {
    // Serial is used as debug (default: USB, 9600 bauds)
    Serial.begin(9600);

    while (!Serial) {
        continue;
    }

    // SerialLink: 4800 bauds (less amount of errors)
    SerialLink.begin(4800);

    SerialLink.setTimeout(5000);

    Serial.println("Iteration\tRaw temperature\tRaw brightness\tVoltage");

    // Register seven-segment display
    for (auto i : DisplayPins) {
        pinMode(i, OUTPUT);
    }
}

//////////////////////////////////////////////////////////////////////

void loop() {
    // Get current second
    uint32_t number = (millis() / 1000) % 10;

    // Get indicator mask for each number
    uint32_t mask = NumberSegments[number];

    // Enable neccessary segments
    for (size_t i = 0; i < 8; ++i) {
        bool EnableSegment = bitRead(mask, i);
        digitalWrite(DisplayPins[i], EnableSegment);
    }

    // Each 10s
    if ((number == 0) and (!sent_json)) {
        // Get raw brightness and temperature from A0 and A1

        uint32_t brightness = analogRead(0);
        uint32_t temperature = analogRead(1);

        // Print debug info over Serial

        Serial.print(iteration);
        Serial.print("\t\t");
        Serial.print(temperature);
        Serial.print("\t\t");
        Serial.print(brightness);

        // Prepare JSON document

        StaticJsonDocument<capacity> package;

        package["iteration"] = iteration;
        package["temperature"] = temperature;
        package["brightness"] = brightness;

        // Send the JSON document over SerialLink
        serializeJson(package, SerialLink);

        sent_json = true;

        ++iteration;
    } else if (number != 0) {
        sent_json = false;
    }
}
