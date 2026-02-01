/*
	Vitaliy Polyakov (C), polyakov.vs@mail.ru, 2024.
*/

// Подключение модуля радиоуправления nRF24L01
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Подключение модуля LCD1602
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Объявление пинов светодиодов
#define LED_ORANGE 2
#define LED_WHITE_LEFT A2
#define LED_WHITE_RIGHT A3
#define LED_RED_LEFT A0
#define LED_RED_RIGHT A1

// Создание и настройка радиомодуля nRF24L01 на пинах 9 и 10
RF24 radio(9, 10);
// Объявление массива принятых данных
byte recieved_data[11];
// Перечень возможных номеров труб
byte address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };

// Объявление модуля lcd (LCD1602)
LiquidCrystal_I2C lcd(0x27, 20, 4);

// Настройка джойстика
// Среднее положение джойстика по осям X и Y
byte joyMidX;
byte joyMidY;
// Текущее положение джойстика по осям X и Y
byte joyCurX;
byte joyCurY;
// Разброс среднего положения джойстика mid +/- 5
byte joySpread = 5;
// состояние кнопки джойстика (0, 1)
byte joySwitch;
// Состояние кнопок A..D joystick shield (0, 1)
byte joyBtnA;
byte joyBtnB;
byte joyBtnC;
byte joyBtnD;
// Состояние кнопок E..F joystick shield (0, 1)
byte joyBtnE;
byte joyBtnF;

// Переменная mnpEnable (manipulator enable) управляет режимом работы робота:
// false - джойстик управляет двигателями, true - джойстик управляет
// манипулятором. Изменение режима происходит по нажатию на джойстик.
bool mnpEnable = false;
bool mnpLedFlag = false;

// Частота обновления дисплея LCD1602
// 250 микросекунд (10 раз в секунду)
unsigned long lcdTimer;
unsigned long lcdDelay = 100;

unsigned long joySwitchTimer;
unsigned long joySwitchDelay = 250;

unsigned long ledBlinkTimer;
unsigned long ledBlinkDelay = 250;

// Класс для работы с двигателями
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

Motor motorR(4, 3);  // двигатель A, правый
Motor motorL(6, 5);  // двигатель B, левый
byte speed = 255;  // скорость 100% (0-255)

void setup() {
  Serial.begin(9600);

  motorR.init();
  motorL.init();

  // Настройка работы пинов светодиодов
  pinMode(LED_ORANGE, OUTPUT);
  pinMode(LED_WHITE_LEFT, OUTPUT);
  pinMode(LED_WHITE_RIGHT, OUTPUT);
  pinMode(LED_RED_LEFT, OUTPUT);
  pinMode(LED_RED_RIGHT, OUTPUT);

  // Настройка радиомодуля nRF24L01
  //Активировать модуль
  radio.begin();
  // Режим подтверждения приёма: 1 вкл, 0 выкл
  radio.setAutoAck(1);
  // Время между попыткой достучаться, число попыток
  radio.setRetries(0, 15);
  // Разрешить отсылку данных в ответ на входящий сигнал
  radio.enableAckPayload();
  // Размер пакета, в байтах
  radio.setPayloadSize(32);
  // Слушаем трубу 0
  radio.openReadingPipe(1, address[0]);
  // Выбираем канал (в котором нет шумов!)
  radio.setChannel(0x60);
  // Уровень мощности передатчика (RF24_PA_MIN, RF24_PA_LOW, RF24_PA_HIGH,
  // RF24_PA_MAX)
  radio.setPALevel(RF24_PA_MAX);
  // Скорость обмена (RF24_2MBPS, RF24_1MBPS, RF24_250KBPS) должна быть
  // одинакова на приёмнике и передатчике. При самой низкой скорости имеем
  // самую высокую чувствительность (дальность)
  radio.setDataRate(RF24_250KBPS);
  // Начать работу
  radio.powerUp();
  // Начинаем слушать эфир (мы приёмный модуль)
  radio.startListening();

  // Инициализация модуля LCD1602
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Получаем данные с джойстика через радиомодуль nRF24L01
  byte pipeNo;
  while (radio.available(&pipeNo)) {  // есть входящие данные
    // чиатем входящий сигнал
    radio.read(&recieved_data, sizeof(recieved_data));
  } 

  // Извлечение данных из полученного пакета
  joyMidX = recieved_data[0];
  joyMidY = recieved_data[1];
  joyCurX = recieved_data[2];
  joyCurY = recieved_data[3];
  joySwitch = recieved_data[4];
  joyBtnA = recieved_data[5];
  joyBtnB = recieved_data[6];
  joyBtnC = recieved_data[7];
  joyBtnD = recieved_data[8];
  joyBtnE = recieved_data[9];
  joyBtnF = recieved_data[10];

  // Отображение на дисплее LCD1602 полученных данных
  displayJoyData();

  // Переключение режима работы манипулятора по нажатию джойстика
  if ((joySwitch) && (millis() - joySwitchTimer > joySwitchDelay)) {
    joySwitchTimer = millis();
    mnpEnable = !mnpEnable;
  }

  if (mnpEnable) {
    // Работает манипулятор




    // Манипулятор мигаем оранжевым светодиодом
    if (millis() - ledBlinkTimer > ledBlinkDelay) {
      mnpLedFlag = !mnpLedFlag;
      digitalWrite(LED_ORANGE, mnpLedFlag);
      ledBlinkTimer = millis();
    }
  } else {
    // Работают двигатели шасси
    digitalWrite(LED_ORANGE, LOW);
    if ((joyCurX > joyMidX - joySpread) && (joyCurX < joyMidX + joySpread) && (joyCurY > joyMidX - joySpread) && (joyCurY < joyMidX + joySpread)) {
      // Стоп двигателей
      // motorStop();
    } else if ((joyCurX > joyMidX - joySpread) && (joyCurX < joyMidX + joySpread) && (joyCurY < joyMidX - joySpread)) {
      // Движение вперед
      // motorFwd();
    } else if ((joyCurX > joyMidX - joySpread) && (joyCurX < joyMidX + joySpread) && (joyCurY > joyMidY + joySpread)) {
      // Движение назад 
      // motorBwd();
    } else if ((joyCurY > joyMidY - joySpread) && (joyCurY < joyMidY + joySpread) && (joyCurX < joyMidX - joySpread)) {
      // Вращение влево
      // motorRotateLeft();
    } else if ((joyCurY > joyMidY - joySpread) && (joyCurY < joyMidY + joySpread) && (joyCurX > joyMidX + joySpread)) {
      // Вращение вправо
      // motorRotateRight();
    } else if ((joyCurX < joyMidX - joySpread) && (joyCurY < joyMidY - joySpread)) {
      // Движение влево вперед
      // motorLeftFwd();
    } else if ((joyCurX > joyMidX + joySpread) && (joyCurY < joyMidY - joySpread)) {
      // Движение вправо вперед
      // motorRightFwd();
    } else if ((joyCurX < joyMidX - joySpread) && (joyCurY > joyMidY + joySpread)) {
      // Движение влево назад
      // motorLeftBwd();
    } else if ((joyCurX > joyMidX + joySpread) && (joyCurY > joyMidY + joySpread)) {
      // Движение вправо назад
      // motorRightBwd();
    }
  }
}

void displayJoyData() {
  // Отображение данных, полученных с джойстика через радиомодуль nRF24L01
  // на дисплее LCD1602. Формат вывода данных (две строки по 16 символов):
  // первая строка: joyMidX, joyMidY, joySwitch, наименования кнопок A..D, E..F
  // вторая строка: joyCurX, joyCurY, значения (положения кнопок), 0 или 1

  if (millis() - lcdTimer > lcdDelay) {
    // Формирование первой строки (16 символов) для вывода на LCD1602
    String str1 = "";
    if (joyMidX < 10) {
      str1 = "..";
    } else if ((joyMidX > 10) && (joyMidX < 100)) {
      str1 = ".";
    }
    str1 = str1 + joyMidX;
    str1 = str1 + " ";
    if (joyMidY < 10) {
      str1 = str1 + "..";
    } else if ((joyMidY > 10) && (joyMidY < 100)) {
      str1 = str1 + ".";
    }
    str1 = str1 + joyMidY;
    str1 = str1 + " JABCD EF";

    // Формирование второй строки (16 символов) для вывода на LCD1602
    String str2 = "";
    if (joyCurX < 10) {
      str2 = "..";
    } else if ((joyCurX > 10) && (joyCurX < 100)) {
      str2 = ".";
    }
    str2 = str2 + joyCurX;
    str2 = str2 + " ";
    if (joyCurY < 10) {
      str2 = str2 + "..";
    } else if ((joyCurY > 10) && (joyCurY < 100)) {
      str2 = str2 + ".";
    }
    str2 = str2 + joyCurY;
    str2 = str2 + " ";

    if (joySwitch > 0) {
      str2 = str2 + "*";
    } else {
      str2 = str2 + " ";
    }

    if (joyBtnA > 0) {
      str2 = str2 + "*";
    } else {
      str2 = str2 + " ";
    }

    if (joyBtnB > 0) {
      str2 = str2 + "*";
    } else {
      str2 = str2 + " ";
    }

    if (joyBtnC > 0) {
      str2 = str2 + "*";
    } else {
      str2 = str2 + " ";
    }

    if (joyBtnD > 0) {
      str2 = str2 + "*";
    } else {
      str2 = str2 + " ";
    }

    str2 = str2 + " ";

    if (joyBtnE > 0) {
      str2 = str2 + "*";
    } else {
      str2 = str2 + " ";
    }

    if (joyBtnF > 0) {
      str2 = str2 + "*";
    } else {
      str2 = str2 + " ";
    }

    Serial.println(str1);
    Serial.println(str2);

    // Вывод на LCD1602 первой строки
    lcd.setCursor(0, 0);
    lcd.print(str1);
    // Вывод на LCD1602 второй строки
    lcd.setCursor(0, 1);
    lcd.print(str2);

    lcdTimer = millis();
  }
}

void motorStop() {
  digitalWrite(LED_WHITE_LEFT, LOW);
  digitalWrite(LED_WHITE_RIGHT, LOW);
  digitalWrite(LED_RED_LEFT, LOW);
  digitalWrite(LED_RED_RIGHT, LOW);

  motorR.moveStop();
  motorL.moveStop();
}

void motorFwd() {
  digitalWrite(LED_WHITE_LEFT, HIGH);
  digitalWrite(LED_WHITE_RIGHT, HIGH);

  motorR.moveFwd(speed);
  motorL.moveFwd(speed);
}

void motorBwd() {
  digitalWrite(LED_RED_LEFT, HIGH);
  digitalWrite(LED_RED_RIGHT, HIGH);

  motorR.moveBwd(speed);
  motorL.moveBwd(speed);
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
  if (millis() - ledBlinkTimer > ledBlinkDelay) {
    mnpLedFlag = !mnpLedFlag;
    digitalWrite(LED_WHITE_LEFT, mnpLedFlag);
    ledBlinkTimer = millis();
  }
  digitalWrite(LED_WHITE_RIGHT, LOW);
  digitalWrite(LED_RED_LEFT, LOW);
  digitalWrite(LED_RED_RIGHT, LOW);
}

void motorRightFwd() {
  if (millis() - ledBlinkTimer > ledBlinkDelay) {
    mnpLedFlag = !mnpLedFlag;
    digitalWrite(LED_WHITE_RIGHT, mnpLedFlag);
    ledBlinkTimer = millis();
  }
  digitalWrite(LED_WHITE_LEFT, LOW);
  digitalWrite(LED_RED_LEFT, LOW);
  digitalWrite(LED_RED_RIGHT, LOW);
}

void motorLeftBwd() {
  if (millis() - ledBlinkTimer > ledBlinkDelay) {
    mnpLedFlag = !mnpLedFlag;
    digitalWrite(LED_RED_LEFT, mnpLedFlag);
    ledBlinkTimer = millis();
  }
  digitalWrite(LED_RED_RIGHT, LOW);
  digitalWrite(LED_WHITE_LEFT, LOW);
  digitalWrite(LED_WHITE_RIGHT, LOW);
}

void motorRightBwd() {
  if (millis() - ledBlinkTimer > ledBlinkDelay) {
    mnpLedFlag = !mnpLedFlag;
    digitalWrite(LED_RED_RIGHT, mnpLedFlag);
    ledBlinkTimer = millis();
  }
  digitalWrite(LED_RED_LEFT, LOW);
  digitalWrite(LED_WHITE_LEFT, LOW);
  digitalWrite(LED_WHITE_RIGHT, LOW);
}