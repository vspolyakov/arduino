/*
  Vitaliy Polyakov (c), polyakov.vs@mail.ru, 2024.
*/

class Joystick {
private:
  byte _pinVRX;   // пин VRX
  byte _pinVRY;   // пин VRY
  byte _pinSW;    // пин SW (кнопка)

  int _x;         // текущее положение по оси X
  int _y;         // текущее положение по оси Y
  int _deadZone;  // "мертвая зона" джойстика
  int _middleX;   // среднее положение по оси X
  int _middleY;   // среднее положение по оси Y
public:
  Joystick(byte pinVRX, byte pinVRY, byte pinSW, int deadZone) { // конструктор
    _pinVRX = pinVRX;
    _pinVRY = pinVRY;
    _pinSW = pinSW;
    _deadZone = deadZone;

    pinMode(_pinVRX, INPUT);
    pinMode(_pinVRY, INPUT);
    pinMode(_pinSW, INPUT_PULLUP);
  }

  int getx() {
    analogRead(_pinVRX);
  }

  int gety() {
    analogRead(_pinVRY);
  }

  bool switchPressed() {
    return digitalRead(_pinSW);
  }

  bool inDeadZoneX() {
    if ((analogRead(_pinVRX) >= _middleX - _deadZone) &&
        (analogRead(_pinVRX) <  _middleX + _deadZone)) {
          return true;
        } else {
          return false;
        }
  }

  bool inDeadZoneY() {
    if ((analogRead(_pinVRY) >= _middleY - _deadZone) &&
        (analogRead(_pinVRY) <  _middleY + _deadZone)) {
          return true;
        } else {
          return false;
        }
  }

};

Joystick lJoystick(A0, A1, 11);
Joystick rJoystick(A2, A3, 12);

void setup() {

}

void loop() {
  
}
