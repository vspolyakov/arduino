/*
  Vitaliy Polyakov (c), polyakov.vs@mail.ru, 2024.
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// right joystick
#define RJOY_VRX A0
#define RJOY_VRY A1
#define RJOY_SW 8

// nRF24L01
// "создать" модуль на пинах 9 и 10 Для Уно
RF24 radio(9, 10);

//возможные номера труб
byte address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };

// массив, хранящий передаваемые данные
byte transmit_data[3];
// массив, хранящий последние переданные данные
byte latest_data[3];
// флажок отправки данных
boolean flag;

void setup() {
  Serial.begin(9600);

  pinMode(RJOY_VRX, INPUT);
  pinMode(RJOY_VRY, INPUT);
  pinMode(RJOY_SW, INPUT_PULLUP);

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
  radio.setPALevel(RF24_PA_MAX);

  // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
  // должна быть одинакова на приёмнике и передатчике!
  // при самой низкой скорости имеем самую высокую чувствительность и дальность!
  radio.setDataRate(RF24_250KBPS);
  // начать работу
  radio.powerUp();
  // не слушаем радиоэфир, мы передатчик
  radio.stopListening();
}

void loop() {
  // right joystick data
  Serial.print("RJOY: ");

  transmit_data[0] = map(analogRead(RJOY_VRX), 0, 1023, 0, 255);
  Serial.print(transmit_data[0]);
  Serial.print("\t");

  transmit_data[1] = map(analogRead(RJOY_VRY), 0, 1023, 0, 255);
  Serial.print(transmit_data[1]);
  Serial.print("\t");

  transmit_data[2] = !digitalRead(RJOY_SW);
  Serial.print(transmit_data[2]);
  Serial.print("\n");

  radio.powerUp();
  // отправить по радио
  radio.write(&transmit_data, sizeof(transmit_data));
  // выключить передатчик
  radio.powerDown();
  delay(25);
}