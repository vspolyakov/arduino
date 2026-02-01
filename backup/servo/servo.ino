#include <Servo.h>

Servo myservo;

void setup() {
  myservo.attach(5);



}

void loop() {
  myservo.write(0);
  delay(2000);

  myservo.write(90);
  delay(2000);

  myservo.write(180);
  delay(2000);

  myservo.write(90);
  delay(2000);
}