#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include "settings.h"

#define LED_PIN 2

WebServer server(80);

bool led_state = false;

const char html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 LED Control</title>

    <style>
        body {
            font-family: sans-serif;
            background: #20232a;
            color: white;
            text-align: center;
            padding-top: 80px;
        }

        button {
            padding: 15px 30px;
            margin: 10px;
            font-size: 18px;
            cursor: pointer;
            border: none;
            border-radius: 8px;
        }

        .on {
            background: #4caf50;
            color: white;
        }

        .off {
            background: #e53935;
            color: white;
        }
    </style>
</head>

<body>
    <h1>ESP32 LED Control</h1>

    <h2 id="status">LED is OFF</h2>

    <button class="on" onclick="controlLED('on')">
        Turn ON
    </button>

    <button class="off" onclick="controlLED('off')">
        Turn OFF
    </button>

    <script>
        async function controlLED(state) {
            try {
                const response = await fetch('/led/' + state, {
                    method: 'POST'
                });

                if (!response.ok) {
                    throw new Error('Request failed');
                }

                document.getElementById('status').textContent =
                    'LED is ' + state.toUpperCase();

            } catch (error) {
                document.getElementById('status').textContent =
                    'Connection error';
            }
        }
    </script>
</body>
</html>
)rawliteral";

void handle_root() {
    server.send(200, "text/html", html);
}
void handle_on(){
    digitalWrite(LED_PIN, HIGH);
    led_state = true;

    server.send(200, "text/plain", "ON");
}
void handle_off(){
    digitalWrite(LED_PIN, LOW);
    led_state = false;

    server.send(200, "text/plain", "OFF");
}

void setup(){
    // pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);


    Serial.println("connecting");

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);

    while(WiFi.status()!=WL_CONNECTED){
        delay(500);
        Serial.print(".");
    }
    Serial.println("ip:");
    Serial.println(WiFi.localIP());
   
    server.on("/", HTTP_GET, handle_root);
    server.on("/led/on", HTTP_POST, handle_on);
    server.on("/led/off", HTTP_POST, handle_off);

    server.begin();
    Serial.println("HTTP server started!");
}

void loop(){
    server.handleClient();
}
