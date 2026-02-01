/*
  Vitaliy Polyakov (C), polyakov.vs@mail.ru, 2024.
*/

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// motor A
#define ENA 5
#define IN1 3
#define IN2 4

// motor B
#define ENB 6
#define IN3 7
#define IN4 8

// motor speed (0..255)
byte speedA = 0;
byte speedB = 0;

// Создание и настройка радиомодуля nRF24L01 на пинах 9 и 10
RF24 radio(9, 10);
// Объявление массива принятых данных
byte recieved_data[3];
// Перечень возможных номеров труб
byte address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };

// Настройка джойстика
// Среднее положение джойстика по осям X и Y
byte joyMidX = 127;
byte joyMidY = 127;
// Текущее положение джойстика по осям X и Y
byte joyCurX;
byte joyCurY;
// Разброс среднего положения джойстика mid +/- 5
byte joySpread = 10;
// состояние кнопки джойстика (0, 1)
byte joySwitch;

void setup() {
  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

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

}
void loop() {
  // Получаем данные с джойстика через радиомодуль nRF24L01
  byte pipeNo;
  while (radio.available(&pipeNo)) {  // есть входящие данные
    // чиатем входящий сигнал
    radio.read(&recieved_data, sizeof(recieved_data));
  } 

  // Извлечение данных из полученного пакета
  joyCurX = recieved_data[0];
  Serial.print(recieved_data[0]);
  Serial.print("\t");

  joyCurY = recieved_data[1];
  Serial.print(recieved_data[1]);
  Serial.print("\t");

  joySwitch = recieved_data[2];
  Serial.print(recieved_data[2]);
  Serial.print("\n");

     if ((joyCurX > joyMidX - joySpread) && (joyCurX < joyMidX + joySpread) && (joyCurY > joyMidX - joySpread) && (joyCurY < joyMidX + joySpread)) {
      // Стоп двигателей
      motorStop();
    } else if ((joyCurX > joyMidX - joySpread) && (joyCurX < joyMidX + joySpread) && (joyCurY < joyMidX - joySpread)) {
      // Движение вперед
      motorFwd();
    } else if ((joyCurX > joyMidX - joySpread) && (joyCurX < joyMidX + joySpread) && (joyCurY > joyMidY + joySpread)) {
      // Движение назад 
      motorBwd();
    } else if ((joyCurY > joyMidY - joySpread) && (joyCurY < joyMidY + joySpread) && (joyCurX < joyMidX - joySpread)) {
      // Вращение влево
      motorRotateLeft();
    } else if ((joyCurY > joyMidY - joySpread) && (joyCurY < joyMidY + joySpread) && (joyCurX > joyMidX + joySpread)) {
      // Вращение вправо
      motorRotateRight();
    } else if ((joyCurX < joyMidX - joySpread) && (joyCurY < joyMidY - joySpread)) {
      // Движение влево вперед
      motorLeftFwd();
    } else if ((joyCurX > joyMidX + joySpread) && (joyCurY < joyMidY - joySpread)) {
      // Движение вправо вперед
      motorRightFwd();
    } else if ((joyCurX < joyMidX - joySpread) && (joyCurY > joyMidY + joySpread)) {
      // Движение влево назад
      motorLeftBwd();
    } else if ((joyCurX > joyMidX + joySpread) && (joyCurY > joyMidY + joySpread)) {
      // Движение вправо назад
      motorRightBwd();
    }
  }


void motorStop() {
  byte speedA = 0;
  byte speedB = 0;
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void motorFwd() {
  byte speedA = 255;
  byte speedB = 255;
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);  
}

void motorBwd() {
  byte speedA = 255;
  byte speedB = 255;
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);  
}

void motorRotateLeft() {
  byte speedA = 255;
  byte speedB = 255;
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);    
}

void motorRotateRight() {
  byte speedA = 255;
  byte speedB = 255;
  analogWrite(ENA, speedA);
  analogWrite(ENB, speedB);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);    
}

void motorLeftFwd() {

}

void motorRightFwd() {

}

void motorLeftBwd() {

}

void motorRightBwd() {

}