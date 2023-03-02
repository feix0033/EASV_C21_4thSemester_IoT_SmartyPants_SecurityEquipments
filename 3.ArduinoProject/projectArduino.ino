
int buzzerPin = 3;
bool flag = false;

void setup(){
//    movement sensor

    pinMode(buzzerPin,OUTPUT);
}

void loop(){
    /*
     * if the sensor have a sense. the the flag will be ture. and then the carmar will be turn on
     * and the led will blinking
     * and the buzzer will be make noize.
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