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
const char* host = "maker.ifttt.com";
const char* apiKey = "c3th-_yog0xgoqzfaHnomU";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

BlynkTimer timer;
int timerID;

int cameraTrigger = 0;
int buzzerTrigger = 0;
boolean emailTrigger = false;

void pubMqttBuzzerTriggerMsg(){
    String topicSensorValue = "buzzerTrigger";
    char publishTopic[topicSensorValue.length() + 1];
    strcpy(publishTopic, topicSensorValue.c_str());

    String messageSensorValue = String(buzzerTrigger);
    char publishMsg[messageSensorValue.length() + 1];
    strcpy(publishMsg, messageSensorValue.c_str());

    if(mqttClient.publish(publishTopic,publishMsg)){
        Serial.println("Topic: " + String(publishTopic));
        Serial.println("Message: " + String(publishMsg));
    }else{
        Serial.println("Publish Failed!");
    }
}

void pubMqttCameraTriggerMsg(){
    String topicSensorValue = "cameraTrigger";
    char publishTopic[topicSensorValue.length() + 1];
    strcpy(publishTopic, topicSensorValue.c_str());

    String messageSensorValue = String(cameraTrigger);
    char publishMsg[messageSensorValue.length() + 1];
    strcpy(publishMsg, messageSensorValue.c_str());

    if(mqttClient.publish(publishTopic,publishMsg)){
        Serial.println("Topic: " + String(publishTopic));
        Serial.println("Message: " + String(publishMsg));
    }else{
        Serial.println("Publish Failed!");
    }
}

void pubMqttSensorValueMsg() {

    String topicSensorValue = "sensorValue";
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

void emailService(){
    Serial.print("connecting to ");
    Serial.println(host);

    const int httpPort = 80;


    if (!wifiClient.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    String url = "trigger/sensor_has_been_triggered/with/key/";
    url += apiKey;

    Serial.print("Requesting URL: ");
    Serial.println(url);
    wifiClient.print(String("POST ") + url + " HTTP/1.1\r\n" +
                     "Host: " + host + "\r\n" +
                     "Content-Type: application/x-www-form-urlencoded\r\n" +
                     "Content-Length: 13\r\n\r\n" +
                     "value1=" + "1" + "\r\n");
}

void sendSensor(){
    pirValue = digitalRead(PIRPIN);
    soundValue = analogRead(SOUNDPIN);
    Serial.println(soundValue);
    Serial.println(pirValue);

    Blynk.virtualWrite(V3,soundValue);
    Blynk.virtualWrite(V4,pirValue);

    if(mqttClient.connected()) {
        pubMqttSensorValueMsg();
        if (soundValue >= 2000 || pirValue == 1) {
            buzzerTrigger = 1;
            cameraTrigger = 1;
            pubMqttCameraTriggerMsg();
            pubMqttBuzzerTriggerMsg();
            emailTrigger = true;
        }
    }
}



void connectMQTTServer() {
    if(mqttClient.connect("ggg", "ibSS5uuhGY1D19pC4twZClvYWmCW0978aPP4979DV04MZkdSBzF3K0Puafh0ecZc","")){
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

    mqttClient.setServer(mqttService,1883);
    connectMQTTServer();

    Blynk.begin(BLYNK_AUTH_TOKEN,ssid,pass);

}


void loop() {
    if(mqttClient.connected()){
        mqttClient.loop();
    }else{
        connectMQTTServer();
    }

    if(emailTrigger){
        emailService();
        emailTrigger=false;
    }


    Blynk.run();
    timer.run();
}