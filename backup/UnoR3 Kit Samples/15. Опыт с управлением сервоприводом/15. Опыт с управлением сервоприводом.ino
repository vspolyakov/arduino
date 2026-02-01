int servoPin=9;
int servoPosition=4000;

void setup() {
pinMode(servoPin, OUTPUT);
}
void loop() {
digitalWrite(servoPin, HIGH); 
delayMicroseconds(servoPosition); 
digitalWrite(servoPin, LOW); 
delay(20);
}
