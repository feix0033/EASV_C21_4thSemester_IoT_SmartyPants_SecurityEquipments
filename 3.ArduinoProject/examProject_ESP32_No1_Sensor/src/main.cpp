#include <Arduino.h>
#include "WiFi.h"


int ledPin = 2;
int pirPin = 12;
int soundPin = A0;

int pirValue;
uint16_t soundValue;

void setup() {
    // Serial.
    Serial.begin(115200);

    pinMode(ledPin,OUTPUT);
    digitalWrite(ledPin,LOW);

    // Wifi Setup
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

}

void loop() {
    pirValue = digitalRead(pirPin);
    soundValue = analogRead(soundPin);

    if(pirValue == 1 || soundValue >= 2500){
        Serial.println("you have been broke");
        digitalWrite(ledPin,HIGH);
        delay(500);
        digitalWrite(ledPin,LOW);
        delay(500);
    }
}

