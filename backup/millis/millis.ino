unsigned long timer1;
unsigned long timer2;
unsigned long counter = 0;

#define LED 13
bool ledOn = false;

void setup() {
    Serial.begin(9600);
    pinMode(LED, OUTPUT);
}

void loop() {
    // Здесь выполняется код, который не зависит от таймера




    // Проверка таймеров
    if (millis() - timer1 > 1000) {
        timer1 = millis();
        Serial.println(counter);
        counter++;
    }

    if (millis() - timer2 > 500) {
        ledOn = !ledOn;
        digitalWrite(LED, ledOn);
        timer2 = millis();
    }



}
