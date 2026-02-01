/*
  Vitaliy Polyakov (c), polyakov.vs@mail.ru, 2024.
  Reciever (приемник, RX).
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// "создать" модуль на пинах 9 и 10 Для Уно
RF24 radio(9, 10);

// массив принятых данных
byte recieved_data[10];

// возможные номера труб
byte address[][6] = {"1Node", "2Node", "3Node", "4Node", "5Node", "6Node"};

// переменные для принятых данных
byte lx;    // x левого джойстика
byte ly;    // y левого джойстика
byte lsw;   // кнопка левого джойстика

byte rx;    // x правого джойстика
byte ry;    // y правого джойстика
byte rsw;   // кнопка правого джойстика

byte btnA;  // состояние кнопки A
byte btnB;  // состояние кнопки B
byte btnC;  // состояние кнопки C
byte btnD;  // состояние кнопки D

void setup() {
  Serial.begin(9600);

  //активировать модуль
  radio.begin();
  // режим подтверждения приёма, 1 вкл 0 выкл
  radio.setAutoAck(1);
  // время между попыткой достучаться, число попыток
  radio.setRetries(0, 15);
  // разрешить отсылку данных в ответ на входящий сигнал
  radio.enableAckPayload();
  // размер пакета, в байтах
  radio.setPayloadSize(32);
  // хотим слушать трубу 0
  radio.openReadingPipe(1, address[0]);
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
  // начинаем слушать эфир, мы приёмный модуль
  radio.startListening();
}

void loop() {
  byte pipeNo;
  while (radio.available(&pipeNo)) { // есть входящие данные
    // чиатем входящий сигнал
    radio.read(&recieved_data, sizeof(recieved_data));

    // TODO: для отладки (выводим данные в Serial port)
    printData();

    // Left joystick
    byte lx =  recieved_data[0];
    byte ly =  recieved_data[1];
    byte lsw = recieved_data[2];
    // Right joystick
    byte rx  = recieved_data[3];
    byte ry  = recieved_data[4];
    byte rsw = recieved_data[5];
    // Buttons A..D
    byte btnA = recieved_data[6];
    byte btnB = recieved_data[7];
    byte btnC = recieved_data[8];
    byte btnD = recieved_data[9];
  }
}


void printData() {
  Serial.print("LJOY(");
  Serial.print(recieved_data[0]);
  Serial.print(", ");
  Serial.print(recieved_data[1]);
  Serial.print(", ");
  Serial.print(recieved_data[2]);
  Serial.print(") ");
  Serial.print("\t");

  Serial.print("RJOY(");
  Serial.print(recieved_data[3]);
  Serial.print(", ");
  Serial.print(recieved_data[4]);
  Serial.print(", ");
  Serial.print(recieved_data[5]);
  Serial.print(") ");
  Serial.print("\t");

  Serial.print("Buttons(");
  Serial.print(recieved_data[6]);
  Serial.print(", ");
  Serial.print(recieved_data[7]);
  Serial.print(", ");
  Serial.print(recieved_data[8]);
  Serial.print(", ");
  Serial.print(recieved_data[9]);
  Serial.print(")");

  Serial.print("\n");
}
