/*
  Vitaliy Polyakov (C), polyakov.vs@mail.ru, 2024.
  Driver mx1508.
*/

class Motor {

private:
  byte _fwdPin;   // IN1
  byte _bwdPin;   // IN2
public:
  Motor(byte fwdPin, byte bwdPin) {
    _fwdPin = fwdPin;
    _bwdPin = bwdPin;

    pinMode(fwdPin, OUTPUT);
    pinMode(bwdPin, OUTPUT);

    // stop motor by default
    analogWrite(_fwdPin, LOW);
    analogWrite(_bwdPin, LOW);
  }

  void move(byte reverse, bool speed) {
    if (!reverse) {
      // forward
      analogWrite(_bwdPin, LOW);  
      analogWrite(_fwdPin, speed); 
    } else {
      // bacward
      analogWrite(_bwdPin, speed);
      analogWrite(_fwdPin, LOW); 
    }
  }

  void stop() {
    analogWrite(_fwdPin, LOW);
    analogWrite(_bwdPin, LOW);
  }
};

Motor motorA(4, 3);
Motor motorB(6, 5);

void setup() {

}

void loop() {
  // move forward motors A and B with speed 125 (50%)
  motorA.move(255, false);
  motorB.move(255, false);
  delay(3000);

  // move backward motors A and B
  motorA.move(255, true);
  motorB.move(255, true);
  delay(3000);

  // move motor A forward, motor B backward
  motorA.move(255, false);
  motorB.move(255, true);
  delay(3000);

  // move motor A backward, motor B forward
  motorA.move(255, true);
  motorB.move(255, false);
  delay(3000);

  // stop motors A and B
  motorA.stop();
  motorB.stop();
  delay(3000);
}