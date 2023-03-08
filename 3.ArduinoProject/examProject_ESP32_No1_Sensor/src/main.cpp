#include <Arduino.h>
#include "WiFi.h"


int ledPin = 2;
int pirPin = 12;
int soundPin = A0;


int pirValue = LOW;
uint16_t soundValue;

// Wi-Fi ssid and password
const char* ssid = "12345678";
const char* password = "12345678";

void ledBlinkingFast(){
    digitalWrite(ledPin,HIGH);
    delay(500);
    digitalWrite(ledPin,LOW);
    delay(500);
}

void ledBlinkingSlow(){
    digitalWrite(ledPin,HIGH);
    delay(1000);
    digitalWrite(ledPin,LOW);
    delay(1000);
}

void wifiConnect(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    WiFi.setSleep(false);
    Serial.println("Start to connect to wifi ..");

    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        ledBlinkingFast();
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
}

void setup() {
    Serial.begin(115200);

    pinMode(ledPin,OUTPUT);
    digitalWrite(ledPin,LOW);

    // Wifi Setup
    wifiConnect();
    Serial.println("Start to sense ...");


}

void loop() {
    pirValue = digitalRead(pirPin);
    soundValue = analogRead(soundPin);
    Serial.println(soundValue);
    Serial.println(pirValue);
    delay(100);

    if (soundValue >= 2000) {
        Serial.println("We sense a sound.. ");
    }

    if(pirValue == HIGH){
        Serial.println("We sense a move..");
        digitalWrite(ledPin,HIGH);
    } else {
        digitalWrite(ledPin, LOW);
    }

}