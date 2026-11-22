#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

#include "settings.h"

const int LED_PIN = 2;

bool led_state = false;

unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;


void connectWifi(){
    Serial.print("connecting to: ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, passwd);

    for (int i = 0; WiFi.status()!=WL_CONNECTED && i<40; i++){
        delay(500);
        Serial.print(".");
    }

    if (WiFi.status()== WL_CONNECTED){
        Serial.print("ip: ");
        Serial.println(WiFi.localIP());
    }
    else{
        Serial.println("connection failed");
    }
}

void sentData(){
    if (WiFi.status() != WL_CONNECTED) return;
    HTTPClient http;
    http.begin(SERVER_URL);
    http.addHeader("Content-Type", "application/json");

    String json = "{\"name\":\"mememe\"}";
    Serial.println("sending data");

    Serial.print("code: ");
    Serial.println(http.POST(json));
    Serial.print("response: ");
    Serial.println(http.getString());
    
    http.end();
}

void setup(){
    // pinMode(LED_PIN, OUTPUT);
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    connectWifi();
}

void loop(){
    unsigned long currentTime = millis();
    if (currentTime - lastSendTime >= sendInterval){
        lastSendTime = currentTime;
        sentData();
    }
}
