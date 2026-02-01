/*
  Vitaliy Polyakov (C), polyakov.vs@mail.ru, 2024.

  Подключенные модули:
    L298N           - 1 шт
    PCA9685         - 1 шт
    LCD1602         - 1 шт
    nRF24L01        - 1 шт
    LED (белый)     - 2 шт
    LED (красный)   - 2 шт
    LED (оранжевый) - 1 шт

  Схема подключения:
                          TX1 |       | VIN   +5V (input)
                          RXO |       | GND   GND (-5V input, nRF24L01, LCD1602, LEDs D2, A0, A1, A2, A3)
                          RST |       | RST
                          GND |       | 5V
5V (LED ORANGE)           D2  |       | A7    
L298N (IN1)               D3  |       | A6
L298N (IN2)               D4  |Arduino| A5    SCL (LCD1602, PCA9685)
L298N (ENA)               D5  |  Nano | A4    SDA (LCD1602, PCA9685)
L298N (ENB)               D6  |       | A3    5V (LED WHITE, RIGHT)
L298N (IN3)               D7  |       | A2    5V (LED WHITE, LEFT)
L298N (IN4)               D8  |       | A1    5V (LED RED, RIGHT)
CSN (nRF24L01)            D9  |       | A0    5V (LED RED, LEFT)
CE (nRF24L01)             D10 |       | REF
MOSI (nRF24L01)           D11 |       | 3V3
MISO (nRF24L01)           D12 |       | D13   SCK (nRF24L01)
                              |  USB  |

                              Отдельно: питание +5V от DC-DC преобразователя: Arduino Nano, nRF24L01, LCD1602, PCA9685
*/

#define PIN_ENA 5 // Вывод управления скоростью вращения мотора №1
#define PIN_ENB 6 // Вывод управления скоростью вращения мотора №2
#define PIN_IN1 3 // Вывод управления направлением вращения мотора №1
#define PIN_IN2 4 // Вывод управления направлением вращения мотора №1
#define PIN_IN3 7 // Вывод управления направлением вращения мотора №2
#define PIN_IN4 8 // Вывод управления направлением вращения мотора №2

uint8_t power = 105; // Значение ШИМ (или скорости вращения)

void setup() {
  // Установка всех управляющих пинов в режим выхода
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  // Команда остановки двум моторам
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}

void loop() {
  // Вращаем моторы в одну сторону с разной скоростью
  while(power < 255) {
    analogWrite(PIN_ENA, power); // Устанавливаем скорость 1-го мотора
    analogWrite(PIN_ENB, power); // Устанавливаем скорость 2-го мотора
    // Задаём направление для 1-го мотора
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    // Задаём направление для 2-го мотора
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    delay(3000); // Пауза 3 секунды
    power += 30; // Увеличиваем скорость
  }
  
  power = 225;
  // Вращаем моторы в другую сторону с разной скоростью
  while(power > 105) {
    analogWrite(PIN_ENA, power); // Устанавливаем скорость 1-го мотора
    analogWrite(PIN_ENB, power); // Устанавливаем скорость 2-го мотора
    // Задаём направление для 1-го мотора
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    // Задаём направление для 2-го мотора
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
    delay(3000); // Пауза 3 секунды
    power -= 30; // Уменьшаем скорость
  }
  power = 105;
}