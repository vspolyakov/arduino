/*
	Vitaliy Polyakov (C), polyakov.vs@mail.ru, 2024.
*/

#include <Servo.h>

#define LED_ORANGE 2
#define LED_WHITE_LEFT A2
#define LED_WHITE_RIGHT A3
#define LED_RED_LEFT A0
#define LED_RED_RIGHT A1

#define JOY_X A6
#define JOY_Y A7
#define JOY_SW 3

// Переменная mnpEnable (manipulator enable) управляет режимом работы робота:
// false - джойстик управляет двигателями, true - джойстик управляет 
// манипулятором. Изменение режима происходит по нажатию на джойстик.
bool mnpEnable = false;

// Координаты и состояние кнопки, полученные с джойстика
byte x;
byte y;
bool sw;

byte midX;         // среднее положение джойстика по оси X
byte midY;         // среднее положение джойстика по оси Y
byte spread = 15;  // разброс средних значений джойстика

// Таймеры для millis()
unsigned long lcdDisplayTimer;  // отображение данных на LCD1602
unsigned long switchTimer;
unsigned long mnpLedTimer;  // мигание светодиодов
bool mnpLed = false;
bool ledOn = false;

Servo testServo;
byte testServoAngle;
byte joyServoAngle;

void setup() {
  Serial.begin(9600);
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_WHITE_LEFT, OUTPUT);
  pinMode(LED_WHITE_RIGHT, OUTPUT);
  pinMode(LED_RED_LEFT, OUTPUT);
  pinMode(LED_RED_RIGHT, OUTPUT);

  pinMode(JOY_X, INPUT);
  pinMode(JOY_Y, INPUT);
  pinMode(JOY_SW, INPUT_PULLUP);

  midX = map(analogRead(JOY_X), 0, 1023, 0, 255);
  midY = map(analogRead(JOY_Y), 0, 1023, 0, 255);

  testServo.attach(10);  // тестовый серво-привод подключен к цифровому пину 10
  testServo.write(90);   // начальное положение сервопривода
}

void loop() {
  x = map(analogRead(JOY_X), 0, 1023, 0, 255);
  y = map(analogRead(JOY_Y), 0, 1023, 0, 255);
  sw = !digitalRead(JOY_SW);

  if ((sw) && (millis() - switchTimer > 250)) {
	switchTimer = millis();
	mnpEnable = !mnpEnable;
  }

  if (mnpEnable) {
	// Работает манипулятор
	testServoAngle = testServo.read();
	joyServoAngle = map(x, 0, 255, 0, 180);

	if ((joyServoAngle > 85) && (joyServoAngle < 95)) {
	  // Ничего не делаем
	} else {
	  if ((joyServoAngle > 95) && (testServo.read()) < 180) {
		// Вращаем серво вправо
		testServo.write(testServo.read() + 1);
		delay(10);
	  } else if ((joyServoAngle < 85) && (testServo.read() > 0)) {
		// Вращаем серво влево
		testServo.write(testServo.read() - 1);
		delay(10);
	  }
	}

	// Манипулятор мигаем оранжевым светодиодом
	if (millis() - mnpLedTimer > 250) {
	  mnpLed = !mnpLed;
	  digitalWrite(LED_ORANGE, mnpLed);
	  mnpLedTimer = millis();
	}
  } else {
	digitalWrite(LED_ORANGE, LOW);
	// Работают двигатели шасси
	if ((x > midX - spread) && (x < midX + spread) && (y > midY - spread) && (y < midY + spread)) {
	  // Стоп двигателей
	  showMsg("MOTORS: STOP");
	  motorStop();
	} else if ((x > midX - spread) && (x < midX + spread) && (y < midY - spread)) {
	  // Движение вперед
	  showMsg("MOTORS: MOVE FORWARD");
	  motorFwd();
	} else if ((x > midX - spread) && (x < midX + spread) && (y > midY + spread)) {
	  showMsg("MOTORS: MOVE BACKWARD");
	  motorBwd();
	} else if ((y > midY - spread) && (y < midY + spread) && (x < midX - spread)) {
	  showMsg("MOTORS: ROTATE LEFT");
	  motorRotateLeft();
	} else if ((y > midY - spread) && (y < midY + spread) && (x > midX + spread)) {
	  showMsg("MOTORS: ROTATE RIGHT");
	  motorRotateRight();
	} else if ((x < midX - spread) && (y < midY - spread)) {
	  showMsg("MOTORS: MOVE LEFT FORWARD");
	  motorLeftFwd();
	} else if ((x > midX + spread) && (y < midY - spread)) {
	  showMsg("MOTORS: MOVE RIGHT FORWARD");
	  motorRightFwd();
	} else if ((x < midX - spread) && (y > midY + spread)) {
	  showMsg("MOTORS: MOVE LEFT BACKWARD");
	  motorLeftBwd();
	} else if ((x > midX + spread) && (y > midY + spread)) {
	  showMsg("MOTORS: MOVE RIGHT BACKWARD");
	  motorRightBwd();
	}
  }
}

void motorStop() {
  digitalWrite(LED_WHITE_LEFT, LOW);
  digitalWrite(LED_WHITE_RIGHT, LOW);
  digitalWrite(LED_RED_LEFT, LOW);
  digitalWrite(LED_RED_RIGHT, LOW);
}

void motorFwd() {
  digitalWrite(LED_WHITE_LEFT, HIGH);
  digitalWrite(LED_WHITE_RIGHT, HIGH);
}

void motorBwd() {
  digitalWrite(LED_RED_LEFT, HIGH);
  digitalWrite(LED_RED_RIGHT, HIGH);
}

void motorRotateLeft() {
  digitalWrite(LED_RED_LEFT, HIGH);
  digitalWrite(LED_RED_RIGHT, LOW);
  digitalWrite(LED_WHITE_LEFT, HIGH);
  digitalWrite(LED_WHITE_RIGHT, LOW);
}

void motorRotateRight() {
  digitalWrite(LED_RED_LEFT, LOW);
  digitalWrite(LED_RED_RIGHT, HIGH);
  digitalWrite(LED_WHITE_LEFT, LOW);
  digitalWrite(LED_WHITE_RIGHT, HIGH);
}

void motorLeftFwd() {
  if (millis() - mnpLedTimer > 250) {
	ledOn = !ledOn;
	digitalWrite(LED_WHITE_LEFT, ledOn);
	mnpLedTimer = millis();
  }
  digitalWrite(LED_WHITE_RIGHT, LOW);
  digitalWrite(LED_RED_LEFT, LOW);
  digitalWrite(LED_RED_RIGHT, LOW);
}

void motorRightFwd() {
  if (millis() - mnpLedTimer > 250) {
	ledOn = !ledOn;
	digitalWrite(LED_WHITE_RIGHT, ledOn);
	mnpLedTimer = millis();
  }
  digitalWrite(LED_WHITE_LEFT, LOW);
  digitalWrite(LED_RED_LEFT, LOW);
  digitalWrite(LED_RED_RIGHT, LOW);
}

void motorLeftBwd() {
  if (millis() - mnpLedTimer > 250) {
	ledOn = !ledOn;
	digitalWrite(LED_RED_LEFT, ledOn);
	mnpLedTimer = millis();
  }
  digitalWrite(LED_RED_RIGHT, LOW);
  digitalWrite(LED_WHITE_LEFT, LOW);
  digitalWrite(LED_WHITE_RIGHT, LOW);
}

void motorRightBwd() {
  if (millis() - mnpLedTimer > 250) {
	ledOn = !ledOn;
	digitalWrite(LED_RED_RIGHT, ledOn);
	mnpLedTimer = millis();
  }
  digitalWrite(LED_RED_LEFT, LOW);
  digitalWrite(LED_WHITE_LEFT, LOW);
  digitalWrite(LED_WHITE_RIGHT, LOW);
}


void showMsg(String str) {
  if (millis() - lcdDisplayTimer > 200) {
	Serial.print("midX = ");
	Serial.print(midX);
	Serial.print(", midY = ");
	Serial.print(midY);
	Serial.print(", X = ");
	if (x < 10) {
	  Serial.print("  ");
	} else if ((x > 10) && (x < 100)) {
	  Serial.print(" ");
	}
	Serial.print(x);
	Serial.print(", Y = ");
	if (y < 10) {
	  Serial.print("  ");
	} else if ((y > 10) && (y < 100)) {
	  Serial.print(" ");
	}
	Serial.print(y);
	Serial.print(", SW = ");
	Serial.print(sw);
	Serial.print(", MNP = ");
	Serial.print(mnpEnable);
	Serial.print("   - ");
	Serial.println(str);

	lcdDisplayTimer = millis();
  }
}