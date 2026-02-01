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
#include <LiquidCrystal_I2C.h>  // библиотека

void setup() {
  LiquidCrystal_I2C lcd(0x27, 20, 4); // запускаем
  lcd.init();
  lcd.backlight();                    // включаем подсветку
}

void loop() {
  LiquidCrystal_I2C lcd(0x27, 20, 4); // запускаем
  lcd.init();
  lcd.backlight();                    // включаем подсветку

  for (int i = 0; i < 16; i++) {
    lcd.setCursor(i, 0);
    lcd.print("*");
    lcd.setCursor(0, 1);
    lcd.print(i);
    delay(500);
  }
}