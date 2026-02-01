/*
  Vitaliy Polyakov (C), polyakov.vs@mail.ru, 2024.
*/

class Motor {
private:
  byte _fwdPin;
  byte _bwdPin;
public:
  Motor(byte fwdPin, byte bwdPin) {
    _fwdPin = fwdPin;
    _bwdPin = bwdPin;
  }

  void init() {
    pinMode(_fwdPin, OUTPUT);
    pinMode(_bwdPin, OUTPUT);
  }

  void moveFwd(byte speed) {
    analogWrite(_fwdPin, speed);
    analogWrite(_bwdPin, LOW);  
  }

  void moveBwd(byte speed) {
    analogWrite(_fwdPin, LOW); 
    analogWrite(_bwdPin, speed);
  }

  void moveStop() {
    analogWrite(_fwdPin, LOW); 
    analogWrite(_bwdPin, LOW);
  }
};

Motor motorR(4, 3); // двигатель A, правый
Motor motorL(6, 5); // двигатель B, левый

byte maxSpeed = 200;

void setup() {
  motorR.init();
  motorL.init();
}

void loop() {
  motorR.moveFwd(maxSpeed);
  motorL.moveFwd(maxSpeed);
  delay(2000);

  motorR.moveStop();
  motorL.moveStop();
  delay(2000);

  motorR.moveBwd(maxSpeed);
  motorL.moveBwd(maxSpeed);
  delay(2000);

  motorR.moveStop();
  motorL.moveStop();
  delay(2000);
}
