#include <DHT.h>     // Подключение библиотеки
#define DHTPIN 2     // Выбираем цифровой pin, к которому подключается информационный выход датчика 
#define DHTTYPE DHT22   // Указываем тип датчика
 
DHT dht(DHTPIN, DHTTYPE);
 
void setup() {
  Serial.begin(9600);
  Serial.println(F("DHT test!"));
  dht.begin();
}
 
void loop() {
  // Обеспечиваем интервал в 2 секунды между измерениями
  delay(2000);
 
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
 
  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));
}