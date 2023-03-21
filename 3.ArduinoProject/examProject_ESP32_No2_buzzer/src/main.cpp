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

int buzzerPin = 16;

boolean trigger = false;

// Wi-Fi ssid and password
const char* ssid = "Evensnachi";
const char* pass = "12345678";
const char* mqttService = "mqtt.flespi.io";

WiFiClient wifiClient;
PubSubClient mqttClient(wifiClient);

BlynkTimer timer;
int timerID;


void buzzerTone(){
    for (int i = 0; i < 3; ++i) {
        tone(buzzerPin, 200); // should be 2000
        delay(200);
        noTone(buzzerPin);
        delay(200);
        tone(buzzerPin, 100); // should be 1000
        delay(200);
        noTone(buzzerPin);
        delay(200);
    }
}

void pubMqttBuzzerStatueMsg(){

    String topicSensorValue = "buzzerStatus"; // here insert the topic which you want to publish.
    char publishTopic[topicSensorValue.length() + 1];
    strcpy(publishTopic, topicSensorValue.c_str());

    String messageSensorValue = String(trigger); // here insert the message which you want to publish
    char publishMsg[messageSensorValue.length() + 1];
    strcpy(publishMsg, messageSensorValue.c_str());

    if(mqttClient.publish(publishTopic,publishMsg)){
        Serial.println("Topic: " + String(publishTopic));
        Serial.println("Message: " + String(publishMsg));
    }else{
        Serial.println("Publish Failed!");
    }
}

void receiveCallback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message Received [");
    Serial.print(topic);
    Serial.print("]");

    for (int i = 0; i < length; i++) {
        Serial.println((char) payload[i]);
    }

    Serial.println("");
    Serial.print("Message length(Bytes): ");
    Serial.println(length);

    if((char) payload[0] == '1'){
        Serial.println("start buzzer");
        buzzerTone();
        pubMqttBuzzerStatueMsg();
    } else if((char) payload[0] == '0'){
        noTone(buzzerPin);
        pubMqttBuzzerStatueMsg();
    }
}

void subscribeBuzzerTopic() {
    String topicString = "buzzerTrigger"; // here insert the topic which you want to subscribed.
    char subTopic[topicString.length() + 1];
    strcpy(subTopic,topicString.c_str());

    if(mqttClient.subscribe(subTopic)){
        Serial.println("Subscrib Topic: ");
        Serial.println(subTopic);
    }else{
        Serial.println("Sbuscribe Fail..");
    }
}

void connectMQTTServer() {
//    String clientId = "esp32-Sensor-" + WiFi.macAddress();
    if(mqttClient.connect("eee", "r8Skuyyk0E3SEbARQJPNJ2eQZcGrDRGUQp48LqiBU9jnbH2RgHsPxvtmxYD0F8Kj","")){
        Serial.println("MQTT Service connected!");
        Serial.println("Server address: ");
        Serial.println(mqttService);
        subscribeBuzzerTopic(); // subscribe the topic which this method have.
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

void setup() {
    Serial.begin(115200);
    pinMode(buzzerPin, OUTPUT);

    buzzerTone();

    wifiConnect();

    Blynk.begin(BLYNK_AUTH_TOKEN,ssid,pass);


    mqttClient.setServer(mqttService,1883);
    mqttClient.setCallback(receiveCallback);
    connectMQTTServer();

    delay(3000);
    buzzerTone();

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