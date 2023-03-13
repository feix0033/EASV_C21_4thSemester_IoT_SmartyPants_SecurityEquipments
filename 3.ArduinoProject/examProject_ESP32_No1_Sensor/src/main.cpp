#include <Arduino.h>
#include "WiFi.h"
#include "../.pio/libdeps/esp32dev/Blynk/src/Blynk/BlynkTimer.h"
#include "../.pio/libdeps/esp32dev/Blynk/src/BlynkSimpleEsp32.h"
#include "../.pio/libdeps/esp32dev/Blynk/src/Blynk/BlynkHandlers.h"
#include "../.pio/libdeps/esp32dev/PubSubClient/src/PubSubClient.h"


#define BLYNK_TEMPLATE_ID "TMPLkv9OkR1o"
#define BLYNK_TEMPLATE_NAME "Quickstart Template"
#define BLYNK_AUTH_TOKEN "8MHFg1pmVwHBL8_RzEspb7Nl_pCF_Oml"

#define BLYNK_PRINT SERIAL
#define LEDPIN 2
#define PIRPIN 12
#define SOUNDPIN A0

int pirValue = LOW;
uint16_t soundValue = 0;

// Wi-Fi ssid and password
const char* ssid = "Evensnachi";
const char* pass = "12345678";
const char* mqttService = "mqtt.flespi.io";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

BlynkTimer timer;
int timerID;

void wifiConnect(){
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    WiFi.setSleep(false);
    Serial.println("Start to connect to wifi ..");

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println(WiFi.localIP());
}

void connectMQTTServer() {
//    String clientId = "esp32-Sensor-" + WiFi.macAddress();
    if(mqttClient.connect("ggg", "uszYF0QvKzAJ5kSCZByNuCbKukAMVf4fxu12kIoS7Mq1U8tHxPkRhksAsQcdV4gg","")){
        Serial.println("MQTT Service connected!");
        Serial.println("Server address: ");
        Serial.println(mqttService);
    }else{
        Serial.println("MQTT server connect fail.. ");
        Serial.println("Client state: ");
        Serial.println(mqttClient.state());
        delay(3000);
    }
}

void pubMqttSensorValueMsg() {

    String topicSensorValue = "esp32SensorValue";
    char publishTopic[topicSensorValue.length() + 1];
    strcpy(publishTopic, topicSensorValue.c_str());

    String messageSensorValue = "sound: " + String(soundValue) + "; pir: " + String(pirValue);
    char publishMsg[messageSensorValue.length() + 1];
    strcpy(publishMsg, messageSensorValue.c_str());

    if(mqttClient.publish(publishTopic,publishMsg)){
        Serial.println("Topic: " + String(publishTopic));
        Serial.println("Message: " + String(publishMsg));
    }else{
        Serial.println("Publish Failed!");
    }
}

void sendSensor(){
    pirValue = digitalRead(PIRPIN);
    soundValue = analogRead(SOUNDPIN);
    Serial.println(soundValue);
    Serial.println(pirValue);

    Blynk.virtualWrite(V3,soundValue);
    Blynk.virtualWrite(V4,pirValue);

    if(mqttClient.connected()){
        if(soundValue >= 2000 || pirValue == 1){
            pubMqttSensorValueMsg();
        }
    }

}


BLYNK_WRITE(V0){
    int value = param.asInt();
    Serial.println(value);

    if(value == 1){
        digitalWrite(LEDPIN,HIGH);
        Serial.println("led on! ");
        timerID = timer.setInterval(200L, sendSensor);
        timer.enable(timerID);

    } else {
        digitalWrite(LEDPIN, LOW);
        timer.disable(timerID);
    }
};


void setup() {
    Serial.begin(115200);

    pinMode(LEDPIN,OUTPUT);
    digitalWrite(LEDPIN,LOW);
    

    wifiConnect();

    Blynk.begin(BLYNK_AUTH_TOKEN,ssid,pass);
    mqttClient.setServer(mqttService,1883);

    connectMQTTServer();



}


void loop() {
    if(mqttClient.connected()){
        mqttClient.loop();
    }else{
        connectMQTTServer();
    }

    Blynk.run();
    timer.run();
}