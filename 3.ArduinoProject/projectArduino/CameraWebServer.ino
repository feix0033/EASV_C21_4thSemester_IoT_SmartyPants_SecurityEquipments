
int soundPin = A0;



void setup() {
    Serial.begin(9600);
    pinMode(soundPin,INPUT)
}

void loop() {
    int soundValue = analogRead(soundPin);
    Serial.println("the sound sensor value = " + soundValue);
}
