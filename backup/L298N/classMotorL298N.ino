/*
  Vitaliy Polyakov (c), polyakov.vs@mail.ru, 2024.
*/

class Motor { // драйвер L298N

private:
  byte _pinEn;    // пин EN
  byte _pinIn1;   // пин IN1
  byte _pinIn2;   // пин IN2
                  // speed: скорость вращения (0-255)
                  // reverse: false - направление вращения вперед, true - назад
public:
  Motor(byte pinEn, byte pinIn1, byte pinIn2) { // конструктор, инициализация
    _pinEn = pinEn;
    _pinIn1 = pinIn1;
    _pinIn2 = pinIn2;

    pinMode(_pinEn, OUTPUT);
    pinMode(_pinIn1, OUTPUT);
    pinMode(_pinIn2, OUTPUT);

    // остановка мотора (при инициализации)
    analogWrite(_pinEn, 0);
    digitalWrite(_pinIn1, LOW);
    digitalWrite(_pinIn1, LOW);
  }

  void move(byte speed, bool reverse) {
    analogWrite(_pinEn, speed);
    if (!reverse) { // вращение вперед
      digitalWrite(_pinIn1, HIGH);
      digitalWrite(_pinIn2, LOW);
    } else { // вращение назад
      digitalWrite(_pinIn1, LOW);
      digitalWrite(_pinIn2, HIGH);
    }

    void stop() {
      analogWrite(_pinEn, 0);
      digitalWrite(_pinIn1, LOW);
      digitalWrite(_pinIn2, LOW);
    }
  }
};

Motor motorA(9, 8, 7);
Motor motorB(3, 5, 4);

void setup() {

}

void loop() {
  // движение обоих двигателей вперед с половиной скорости
  motorA.move(125, false);
  motorB.move(125, false);
  delay(3000);

  // движение обоих двигателей назад с половиной скорости
  motorA.move(125, true);
  motorB.move(125, true);
  delay(3000);

  // двигатель A вперед, двигатель B назад
  motorA.move(125, false);
  motorB.move(125, true);
  delay(3000);

  // наоборот: двигатель A назад, двигатель B вперед
  motorA.move(125, true);
  motorB.move(125, false);
  delay(3000);

  // остановка двигателей
  motorA.stop();
  motorB.stop();
}
