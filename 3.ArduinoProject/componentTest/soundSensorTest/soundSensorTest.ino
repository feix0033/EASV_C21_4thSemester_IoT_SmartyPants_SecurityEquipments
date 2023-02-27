/***********************************************
* name:Sound Sensor
* function: you can see the value of sound intensity on Serial Monitor.
* When the volume reaches to a certain value, the LED attached to pin 13 on the SunFounder Uno board will light up. 
**************************************************/
//Email:support@sunfounder.com
//Website:www.sunfounder.comconst int ledPin = 13; //pin 13 built-in led

const int soundPin = A0; //sound sensor attach to A0void setup()
int ledPin = 13;

void setup(){
  pinMode(ledPin,OUTPUT);//set pin13 as OUTPUT
  Serial.begin(9600); //initialize serial
}

void loop()
{
  int value = analogRead(soundPin);//read the value of A0
  Serial.println(value);//print the value
  if(value > 600) //if the value is greater than 600
  {
    digitalWrite(ledPin,HIGH);//turn on the led
    delay(200);//delay 200ms
  }
  else
  {
    digitalWrite(ledPin,LOW);//turn off the led
  }
}
