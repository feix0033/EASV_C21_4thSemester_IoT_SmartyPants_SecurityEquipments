int buzzerPin = 3;

void setup(){
    pinMode(buzzerPin,OUTPUT);
}

void loop(){
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