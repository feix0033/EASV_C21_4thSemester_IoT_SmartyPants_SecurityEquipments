#include <Arduino.h>
#include "WiFi.h"
#include "../.pio/libdeps/esp32dev/PubSubClient/src/PubSubClient.h"


int ledPin = 2;
int pirPin = 12;
int soundPin = A0;

char triggerForCam[1];


int pirValue = LOW;
uint16_t soundValue;

// Wi-Fi ssid and password
const char* ssid = "12345678";
const char* password = "12345678";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "YOUR_MQTT_BROKER_IP_ADDRESS";

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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

void callback(char* topic, byte* message, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;

    for (int i = 0; i < length; i++) {
        Serial.print((char)message[i]);
        messageTemp += (char)message[i];
    }
    Serial.println();

    // Feel free to add more if statements to control more GPIOs with MQTT

    // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
    // Changes the output state according to the message
    if (String(topic) == "esp32/output") {
        Serial.print("Changing output to ");
        if(messageTemp == "on"){
            Serial.println("on");
            digitalWrite(ledPin, HIGH);
        }
        else if(messageTemp == "off"){
            Serial.println("off");
            digitalWrite(ledPin, LOW);
        }
    }
}



void setup() {
    Serial.begin(115200);

    pinMode(ledPin,OUTPUT);
    digitalWrite(ledPin,LOW);

    // Wifi Setup
    wifiConnect();
    Serial.println("Start to sense ...");
    client.setServer(mqtt_server, 1883);
    client.setCallback(callback);


}


void reconnect() {
    // Loop until we're reconnected
    while (!client.connected()) {
        Serial.print("Attempting MQTT connection...");
        // Attempt to connect
        if (client.connect("ESP8266Client")) {
            Serial.println("connected");
            // Subscribe
            client.subscribe("esp32/output");
        } else {
            Serial.print("failed, rc=");
            Serial.print(client.state());
            Serial.println(" try again in 5 seconds");
            // Wait 5 seconds before retrying
            delay(5000);
        }
    }
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

    if (!client.connected()) {
        reconnect();
    }
    client.loop();

    long now = millis();
    if (now - lastMsg > 5000) {
        lastMsg = now;

        // Temperature in Celsius
//        temperature = bme.readTemperature();
        // Uncomment the next line to set temperature in Fahrenheit
        // (and comment the previous temperature line)
        //temperature = 1.8 * bme.readTemperature() + 32; // Temperature in Fahrenheit

        // Convert the value to a char array
//        char tempString[8];
//        dtostrf(temperature, 1, 2, tempString);
//        Serial.print("Temperature: ");
//        Serial.println(tempString);
//        client.publish("esp32/temperature", tempString);
        client.publish("esp32/cam",triggerForCam);
//        humidity = bme.readHumidity();

        // Convert the value to a char array
//        char humString[8];
//        dtostrf(humidity, 1, 2, humString);
//        Serial.print("Humidity: ");
//        Serial.println(humString);
//        client.publish("esp32/humidity", humString);
    }

}