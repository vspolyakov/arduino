/*
  Vitaliy Polyakov (c), polyakov.vs@mail.ru, 2024.
  Joystick Shield Transmitter (передатчик, TX)
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Joystick
#define JOY_VRX A0     // A0
#define JOY_VRY A1     // A1
#define JOY_SW 8       // D8

// Buttons
#define BUTTON_A 2      // D2
#define BUTTON_B 3      // D3
#define BUTTON_C 4      // D4
#define BUTTON_D 5      // D5
#define BUTTON_E 6      // D6
#define BUTTON_F 7      // D7

// nRF24L01
// "создать" модуль на пинах 9 и 10 Для Уно
RF24 radio(9, 10);

//возможные номера труб
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};

// массив, хранящий передаваемые данные
byte transmit_data[9];
// массив, хранящий последние переданные данные
byte latest_data[9];
// флажок отправки данных
boolean flag;

void setup() {
  Serial.begin(9600);

  pinMode(JOY_VRX, INPUT);
  pinMode(JOY_VRY, INPUT);
  pinMode(JOY_SW, INPUT_PULLUP);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  pinMode(BUTTON_D, INPUT_PULLUP);
  pinMode(BUTTON_E, INPUT_PULLUP);
  pinMode(BUTTON_F, INPUT_PULLUP);

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
  // Joystick data
  transmit_data[0] = map(analogRead(JOY_VRX), 0, 1023, 0, 255);
  transmit_data[1] = map(analogRead(JOY_VRY), 0, 1023, 0, 255);
  transmit_data[2] = !digitalRead(JOY_SW);

  // Buttons A..F data
  transmit_data[3] = !digitalRead(BUTTON_A);
  transmit_data[4] = !digitalRead(BUTTON_B);
  transmit_data[5] = !digitalRead(BUTTON_C);
  transmit_data[6] = !digitalRead(BUTTON_D);
  transmit_data[7] = !digitalRead(BUTTON_E);
  transmit_data[8] = !digitalRead(BUTTON_F);

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
    printData();
    radio.powerDown();
  }
}

void printData() {
    Serial.print("JOY(");
    Serial.print(transmit_data[0]);
    Serial.print(", ");
    Serial.print(transmit_data[1]);
    Serial.print(", ");
    Serial.print(transmit_data[2]);
    Serial.print(") ");
    Serial.print("\t");

    Serial.print("Buttons(");
    Serial.print(transmit_data[3]);
    Serial.print(", ");
    Serial.print(transmit_data[4]);
    Serial.print(", ");
    Serial.print(transmit_data[5]);
    Serial.print(", ");
    Serial.print(transmit_data[6]);
    Serial.print(", ");
    Serial.print(transmit_data[7]);
    Serial.print(", ");
    Serial.print(transmit_data[8]);    
    Serial.print(")");

    Serial.print("\n");
}
