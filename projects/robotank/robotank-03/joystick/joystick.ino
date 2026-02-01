/*
  Vitaliy Polyakov (c), polyakov.vs@mail.ru, 2024.
  Transmitter (передатчик, TX).
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Left joystick
#define LJOY_VRX A0     // A0
#define LJOY_VRY A1     // A1
#define LJOY_SW 8       // D8

// Right joystick
#define RJOY_VRX A2     // A2
#define RJOY_VRY A3     // A3
#define RJOY_SW 7       // D7

// Buttons
#define BUTTON_A 5      // D5
#define BUTTON_B 4      // D4
#define BUTTON_C 3      // D3
#define BUTTON_D 2      // D2

// nRF24L01
// "создать" модуль на пинах 9 и 10 Для Уно
RF24 radio(9, 10);

//возможные номера труб
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};

// массив, хранящий передаваемые данные
byte transmit_data[10];
// массив, хранящий последние переданные данные
byte latest_data[10];
// флажок отправки данных
boolean flag;

void setup() {
  Serial.begin(9600);

  pinMode(LJOY_VRX, INPUT);
  pinMode(LJOY_VRY, INPUT);
  pinMode(LJOY_SW, INPUT_PULLUP);

  pinMode(RJOY_VRX, INPUT);
  pinMode(RJOY_VRY, INPUT);
  pinMode(RJOY_SW, INPUT_PULLUP);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  pinMode(BUTTON_D, INPUT_PULLUP);

  // активировать модуль
  radio.begin();
  // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setAutoAck(1);
  // время между попыткой достучаться, число попыток
  radio.setRetries(0, 15);
  // разрешить отсылку данных в ответ на входящий сигнал
  radio.enableAckPayload();
  // размер пакета, в байтах
  radio.setPayloadSize(32);
  // мы - труба 0, открываем канал для передачи данных
  radio.openWritingPipe(address[0]);
  // выбираем канал (в котором нет шумов!)
  radio.setChannel(0x60);
  // уровень мощности передатчика. На выбор RF24_PA_MIN, RF24_PA_LOW,
  // RF24_PA_HIGH, RF24_PA_MAX
  radio.setPALevel (RF24_PA_MAX);

  // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  // должна быть одинакова на приёмнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность!
  radio.setDataRate (RF24_250KBPS);
  // начать работу
  radio.powerUp();
  // не слушаем радиоэфир, мы передатчик
  radio.stopListening();
}

void loop() {
  // Left joystick data
  transmit_data[0] = map(analogRead(LJOY_VRX), 0, 1023, 0, 255);
  transmit_data[1] = map(analogRead(LJOY_VRY), 0, 1023, 0, 255);
  transmit_data[2] = !digitalRead(LJOY_SW);

  // Right joystick data
  transmit_data[3] = map(analogRead(RJOY_VRX), 0, 1023, 0, 255);
  transmit_data[4] = map(analogRead(RJOY_VRY), 0, 1023, 0, 255);
  transmit_data[5] = !digitalRead(RJOY_SW);

  // Buttons A..D data
  transmit_data[6] = !digitalRead(BUTTON_A);
  transmit_data[7] = !digitalRead(BUTTON_B);
  transmit_data[8] = !digitalRead(BUTTON_C);
  transmit_data[9] = !digitalRead(BUTTON_D);

  // в цикле от 0 до числа каналов
  for (int i = 0; i < 3; i++) {
    // если есть изменения в transmit_data
    if (transmit_data[i] != latest_data[i]) {
      // поднять флаг отправки по радио
      flag = 1;
      // запомнить последнее изменение
      latest_data[i] = transmit_data[i];
    }
  }

  if (flag == 1) {
    // включить передатчик
    radio.powerUp();
    // отправить по радио
    radio.write(&transmit_data, sizeof(transmit_data));
    // опустить флаг
    flag = 0;
    // выключить передатчик
    radio.powerDown();
  }
}
