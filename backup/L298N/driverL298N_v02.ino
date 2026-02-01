/*
  Технические параметры драйвера L298N
  ------------------------------------
  Напряжение питания логики модуля: 5 В
  Потребляемый ток встроенной логики:  36 мА
  Напряжение питания драйвера: 5 В – 35 В
  Рабочий ток драйвера: 2 А (пиковый ток 3 А)


  вывод     Vss         — питание двигателей, от 5 до 35 В;
  вывод     GND         — общий вывод заземления;
  вывод     Vs          — питание логической схемы, 4,5 - 5,5 В;
  перемычка ENA         — управление скоростью двигателя A;
  выводы    IN1 и IN2   — управление направлением вращения двигателя A;
  выводы    IN3 и IN4   — управление направлением вращения двигателя B;
  перемычка ENB         — управление скоростью двигателя B;
  выходы    OUT1 и OUT2 — разъем для двигателя A;
  выходы    OUT3 и OUT4 — разъем для двигателя B;

  Управления скоростью. Разъемы управления скоростью ENA и ENB используются для
  включения и выключения управления скоростью двигателей. Когда перемычка
  установлена, двигатель вращается с максимальной скоростью. Если необходимо
  управлять скоростью двигателей, необходимо убрать перемычку и подключить
  выводы к контактам с поддержкой PWM на Arduino.

  Падение напряжения драйвера L298N составляет около 2 В. Это связано с
  внутренним падением напряжения в транзисторах в цепи H-мосте. Таким образом,
  если подключим 12 В к источнику питания двигателя, то двигатели получат
  напряжение около 10 В. Это означает, что двигатель на 12 В не будет работать
  с максимальной скоростью. Для получения максимальной скорости, напряжение
  поданное на двигатели должно быть выше на 2 В, чем потребность в фактическом
  напряжении двигателя.
*/

// motor A
#define ENA 9 // pin D9, PWM
#define IN1 8 // pin D8
#define IN2 7 // pin D7

// motor B
#define ENB 3 // pin D3, PWM
#define IN3 5 // pin D5
#define IN4 4 // pin D4

// joystick
#define VRX A1 // pin A1
#define VRY A2 // pin A2
#define SW 12  // pin D12

// motor speed (0..255)
int speedA = 0;
int speedB = 0;

// parameter downgrades driven motor speed
float downgradeA = 1.0;
float downgradeB = 1.0;

// joystick x, y values (0..1024)
int x = 0;
int y = 0;

// joystick middle
int xMiddle = 511;
int yMiddle = 511;

// joystick "dead zone"
int deadZone = 25;

void setup() {
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  pinMode(SW, INPUT_PULLUP);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  xMiddle = analogRead(VRX);
  yMiddle = analogRead(VRY);
}

void loop() {
  Serial.begin(9600);

  // current joystick values
  x = analogRead(VRX);
  y = analogRead(VRY);

  downgradeA = 0.5; // full speed - 1.0, half speed - 0.5
  downgradeB = 0.5;

  if (y > (yMiddle + deadZone)) {
    // forward rotation
    // set speed form 0...1023 to 0..255
    speedA = ((y - 511) / 511 * 255) * downgradeA;
    speedB = ((y - 511) / 511 * 255) * downgradeB;

    analogWrite(ENA, speedA);
    analogWrite(ENB, speedB);

    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
  } else if (y < yMiddle - deadZone) {
    // backward rotation
    // set speed form 0...1023 to 0..255
    speedA = ((511 - y) / 511 * 255) * downgradeA;
    speedB = ((511 - y) / 511 * 255) * downgradeB;

    analogWrite(ENA, speedA);
    analogWrite(ENB, speedB);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
  } else {
    // stop rotation
    speedA = 0;
    speedB = 0;
    analogWrite(ENA, speedA);
    analogWrite(ENB, speedB);

    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }


}
