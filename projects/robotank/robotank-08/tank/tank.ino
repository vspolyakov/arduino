/*
  Vitaliy Polyakov (C), polyakov.vs@mail.ru, 2024.

  Подключенные модули:
    MX1508          - 1 шт
    PCA9685         - 1 шт
    LCD1602         - 1 шт
    nRF24L01        - 1 шт
    LED (белый)     - 2 шт
    LED (красный)   - 2 шт
    LED (оранжевый) - 1 шт

  Схема подключения:
                          TX1 |       | VIN   +5V (input)
                          RXO |       | GND   GND (-5V input, nRF24L01, LCD1602)
                          RST |       | RST
GND (LED A7, 3, 2, 1, 0)  GND |       | 5V    5V (nRF24L01, LCD1602)
5V (LED ORANGE)           D2  |       | A7    
IN0 (MX1508, A, BWD)      D3  |       | A6
IN0 (MX1508, A, FWD)      D4  |Arduino| A5    SCL (LCD1602, PCA9685)
IN0 (MX1508, B, BWD)      D5  |  Nano | A4    SDA (LCD1602, PCA9685)
IN0 (MX1508, B, FWD)      D6  |       | A3    5V (LED WHITE, RIGHT)
                          D7  |       | A2    5V (LED WHITE, LEFT)
                          D8  |       | A1    5V (LED RED, RIGHT)
CSN (nRF24L01)            D9  |       | A0    5V (LED RED, LEFT)
CE (nRF24L01)             D10 |       | REF
MOSI (nRF24L01)           D11 |       | 3V3
MISO (nRF24L01)           D12 |       | D13   SCK (nRF24L01)
                              |  USB  |
*/


// Настройка 4-х сервоприводов через I2C плату расширения PCA9685
#define SERVOMIN 150  // Минимальная длительность импульса для сервопривода
#define SERVOMAX 600  // Максимальная длина импульса для сервопривода
#define SERVO_1 0     // Номер порта (0 - 15)
#define SERVO_2 1     // Номер порта (0 - 15)
#define SERVO_3 2     // Номер порта (0 - 15)
#define SERVO_4 3     // Номер порта (0 - 15)

// Настройка светодиодов
#define LED_ORANGE          2
#define LED_WHITE_RIGHT     A3
#define LED_WHITE_LEFT      A2
#define LED_RED_RIGHT       A1
#define LED_RED_LEFT        A0

// Подключение 4-х сервоприводов через I2C плату расширения PCA9685
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Подключение модуля радиоуправления nRF24L01
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

// Подключение ЖК-дисплея LCD1602
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Настройка радиомодуля nRF24L01
// "создать" модуль на пинах 9 и 10 Для Уно
RF24 radio(9, 10);
// массив принятых данных
byte recieved_data[9];
// возможные номера труб
byte address[][6] = { "1Node", "2Node", "3Node", "4Node", "5Node", "6Node" };

// Настройка портов плат расширения LCD1602 и PCA9685
// Установка адреса I2C 0x40 для платы расширения PCA9685
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x40);
// Установка адреса I2C 0x27 для платы расширения LCD1602
LiquidCrystal_I2C lcd(0x27, 20, 4);

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

    pinMode(LED_ORANGE, OUTPUT);
    pinMode(LED_WHITE_RIGHT, OUTPUT);
    pinMode(LED_WHITE_LEFT, OUTPUT);
    pinMode(LED_RED_RIGHT, OUTPUT);
    pinMode(LED_RED_LEFT, OUTPUT);

    // серво PCA9685
    pwm.begin();         // Инициализация
    pwm.setPWMFreq(60);  // Частота следования импульсов 60 Гц
    delay(10);           // Пауза

    motorR.init();
    motorL.init();

    lcd.init();
    lcd.backlight();

    // nrf24L01
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
    radio.setPALevel(RF24_PA_MAX);
    // скорость обмена. На выбор RF24_2MBPS, RF24_1MBPS, RF24_250KBPS
    // должна быть одинакова на приёмнике и передатчике!
    // при самой низкой скорости имеем самую высокую чувствительность и дальность
    radio.setDataRate(RF24_250KBPS);
    // начать работу
    radio.powerUp();
    // начинаем слушать эфир, мы приёмный модуль
    radio.startListening();
}

void loop() {
    // Тест LCD1602
    lcd.setCursor(0, 0);
    lcd.print("TESTING...      ");

    lcd.setCursor(0, 1);
    //         ----------------
    lcd.print("nRF24L01        ");
    // Тест радиомодуля
    byte pipeNo;
    while (radio.available(&pipeNo)) {  // есть входящие данные
        // чиатем входящий сигнал
        radio.read(&recieved_data, sizeof(recieved_data));
    }
    delay(2000);

    lcd.setCursor(0, 1);
    //         ----------------
    lcd.print("LEDS            ");
    // Тест светодиодов
    digitalWrite(LED_WHITE_LEFT, HIGH);
    delay(1000);
    digitalWrite(LED_WHITE_LEFT, LOW);
    
    digitalWrite(LED_WHITE_RIGHT, HIGH);
    delay(1000);
    digitalWrite(LED_WHITE_RIGHT, LOW);

    digitalWrite(LED_RED_LEFT, HIGH);
    delay(1000);
    digitalWrite(LED_RED_LEFT, LOW);

    digitalWrite(LED_RED_RIGHT, HIGH);
    delay(1000);
    digitalWrite(LED_RED_RIGHT, LOW);

    // Тест двигателей
    lcd.setCursor(0, 1);
    //         ----------------
    lcd.print("MOTORS          ");
    motorR.moveFwd(speed);
    motorL.moveFwd(speed);
    delay(2000);

    motorR.moveStop();
    motorL.moveStop();
    delay(2000);

    motorR.moveBwd(speed);
    motorL.moveBwd(speed);
    delay(2000);

    motorR.moveStop();
    motorL.moveStop();
    delay(2000);

    // Тест сервоприводов
    lcd.setCursor(0, 1);
    //         ----------------
    lcd.print("SERVO           ");
    // Включение оранжевого светодиода
    digitalWrite(LED_ORANGE, HIGH);
    // Вращение сервопривода 1 влево
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(SERVO_1, 0, pulselen);
    }
    delay(250);
    // Вращение сервопривода 1 вправо
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(SERVO_1, 0, pulselen);
    }
    delay(250);

    // Вращение сервопривода 2 влево
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(SERVO_2, 0, pulselen);
    }
    delay(250);
    // Вращение сервопривода 2 вправо
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(SERVO_2, 0, pulselen);
    }
    delay(250);

    // Вращение сервопривода 3 влево
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(SERVO_3, 0, pulselen);
    }
    delay(250);
    // Вращение сервопривода 3 вправо
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(SERVO_3, 0, pulselen);
    }
    delay(250);

    // Вращение сервопривода 4 влево
    for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
        pwm.setPWM(SERVO_4, 0, pulselen);
    }
    delay(250);
    // Вращение сервопривода 4 вправо
    for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
        pwm.setPWM(SERVO_4, 0, pulselen);
    }
    delay(250);
    // выключение оранжевого светодиода
    digitalWrite(LED_ORANGE, LOW);

    lcd.setCursor(0, 0);
    //         ----------------
    lcd.print("TEST DONE       ");
    lcd.setCursor(0, 1);
    //         ----------------
    lcd.print("                ");
    delay(2000);
}
