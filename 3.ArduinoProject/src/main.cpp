#include "Arduino.h"
#include "../projectArduino.ino"


int buzzerPin = 3;
flag = false;

void setup(){
//    movement sensor

    pinMode(buzzerPin,OUTPUT);
}

void loop(){
    /*
     * if the sensor have a sense. then the flag will be ture. and then the camera will be turned on
     * and the LED will blink
     * and the buzzer will be make noise.
     */
    buzzer(flag);




    for (int i = 200; i <= 800; i++) {
        tone(buzzerPin,i);
        delay(5);
    }
//    delay(4000);
    for (int i = 800; i >=200; i--) {
        tone(buzzerPin,i);
        delay(10);
    }
}

void pirSonser(){

    pirValue = digitalRead(pirPin);
    digitalWrite(ledPin, pirValue);
}

void buzzer(bool flag){
    while(flag){
        for (int i = 200; i <= 800; i++) {
            tone(buzzerPin,i);
            delay(5);
        }
//    delay(4000);
        for (int i = 800; i >=200; i--) {
            tone(buzzerPin,i);
            delay(10);
        }
    }
}

void led(){
    digitalWrite(ledPin, HIGH);  // turn the LED on (HIGH is the voltage level)
    delay(1000);                      // wait for a second
    digitalWrite(ledPin, LOW);   // turn the LED off by making the voltage LOW
    delay(1000);

}