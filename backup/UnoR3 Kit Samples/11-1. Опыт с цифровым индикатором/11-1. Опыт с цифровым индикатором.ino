const int led2 =  2; 
const int led3 =  3; 
const int led4 =  4; 
const int led5 =  5; 
const int led6 =  6; 
const int led7 =  7; 
const int led8 =  8; 

void setup() {
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
}

void loop() {
 int i = 200;
   digitalWrite(led2, HIGH);
    delay(i);
    digitalWrite(led2, LOW);
    delay(i);
    digitalWrite(led3, HIGH);
    delay(i);
    digitalWrite(led3, LOW);
    delay(i);
    digitalWrite(led4, HIGH);
    delay(i);
    digitalWrite(led4, LOW);
    delay(i);
    digitalWrite(led5, HIGH);
    delay(i);
    digitalWrite(led5, LOW);
    delay(i);
    digitalWrite(led6, HIGH);
    delay(i);
    digitalWrite(led6, LOW);
    delay(i);
    digitalWrite(led7, HIGH);
    delay(i);
    digitalWrite(led7, LOW);
    delay(i);
    digitalWrite(led8, HIGH);
    delay(i);
    digitalWrite(led8, LOW);
    delay(i);
}
