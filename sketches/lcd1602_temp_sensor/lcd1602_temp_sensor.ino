/* 
  A simple sketch to display LCD 1602 + I2C.

  Teacher Inventor DIY : Как настроить LCD ДИСПЛЕЙ и I2C на Arduino ?!
  https://www.youtube.com/watch?v=hdW-cLaQ9WE
  
  Vitaliy Polyakov (c), polyakov.vs@mail.ru, 2024.  

  I2C connection:
  I2C         Arduino
  -------------------
  GND         GND
  VCC         +5V
  SDA         A4
  SCL         A5
*/

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>

#define DHTPIN 2        // Выбираем цифровой pin, к которому подключается информационный выход датчика 
#define DHTTYPE DHT22   // Указываем тип датчика
 
DHT dht(DHTPIN, DHTTYPE);
 
LiquidCrystal_I2C lcd(0x27, 20, 4); // запускаем ЖК-дисплей

void setup() {
  Serial.begin(9600);                 // 
  dht.begin();                        // запускаем датчик

  
  lcd.init();                         // инициализируем ЖК-дисплей
  lcd.backlight();                    // включаем подсветку
}

void loop() {
  // Переменная для считывания значения влажности
  float h = dht.readHumidity();
  //  Переменная для считывания значения температуры (в градусах Цельсия)
  float t = dht.readTemperature();
  // Переменная для считывания значения температуры (в Фаренгейтах)
  float f = dht.readTemperature(true);
 
  // Проверка подключения датчика.
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
 
  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);
 
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.setCursor(6, 0);
  lcd.print(hic);
  delay(2000);

}