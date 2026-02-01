/*
	Vitaliy Polyakov (C), polyakov.vs@mail.ru, 2024.
*/

#define LED_ORANGE A1

#define JOY_X A3
#define JOY_Y A5
#define JOY_SW 3

// Переменная manipulator: false - джойстик управляет двигателями,
// true - джойстик управляет манипулятором
bool mnpEnable = false;

byte x;
byte y;
bool sw;

byte midX;          // среднее положение джойстика по оси X
byte midY;          // среднее положение джойстика по оси Y
byte spread = 15;   // разброс средних значений джойстика 

unsigned long lcdDisplayTimer;
unsigned long switchTimer;
unsigned long mnpLedTimer;
bool mnpLed = false;

void setup() {
	Serial.begin(9600);
	pinMode(LED_ORANGE, OUTPUT);
    pinMode(JOY_X, INPUT);
    pinMode(JOY_Y, INPUT);
    pinMode(JOY_SW, INPUT_PULLUP);

    midX = map(analogRead(JOY_X), 0, 1023, 0, 255);
    midY = map(analogRead(JOY_Y), 0, 1023, 0, 255);
}

void loop() {
    x = map(analogRead(JOY_X), 0, 1023, 0, 255);
    y = map(analogRead(JOY_Y), 0, 1023, 0, 255);
    sw = !digitalRead(JOY_SW);

    if ((sw) && (millis() - switchTimer > 250)) {
        switchTimer = millis();
        mnpEnable = !mnpEnable;
    }

    if (mnpEnable) {
        // Работает манипулятор
        if          ((x > midX - spread) && (x < midX + spread) &&
                     (y > midY - spread) && (y < midY + spread)) {
            showMsg("MNP: STOP");
        } else if   ((x > midX - spread) && (x < midX + spread) &&
                     (y < midY - spread)                       ) {
            showMsg("MNP: MOVE FORWARD");
        } else if   ((x > midX - spread) && (x < midX + spread) &&
                     (y > midY + spread)                       ) {
            showMsg("MNP: MOVE BACKWARD");
        } else if   ((y > midY - spread) && (y < midY + spread) && 
                     (x < midX - spread)                       ) {
            showMsg("MNP: ROTATE LEFT");
        } else if   ((y > midY - spread) && (y < midY + spread) && 
                     (x > midX + spread)                       ) {
            showMsg("MNP: ROTATE RIGHT");
        } else if ((x < midX - spread) && (y < midY - spread))   {
            showMsg("MNP: MOVE LEFT FORWARD");
        } else if ((x > midX + spread) && (y < midY - spread))   {
            showMsg("MNP: MOVE RIGHT FORWARD");
        } else if ((x < midX - spread) && (y > midY + spread))   {
            showMsg("MNP: MOVE LEFT BACKWARD");
        } else if ((x > midX + spread) && (y > midY + spread))   {
            showMsg("MNP: MOVE RIGHT BACKWARD");
        }

        // Манипулятор мигаем оранжевым светодиодом
        if (millis() - mnpLedTimer > 250) {
            mnpLed = !mnpLed;
            digitalWrite(LED_ORANGE, mnpLed);
            mnpLedTimer = millis(); 
        } 
    } else {
        digitalWrite(LED_ORANGE, LOW);
        // Работают двигатели шасси
        if          ((x > midX - spread) && (x < midX + spread) &&
                     (y > midY - spread) && (y < midY + spread)) {
            showMsg("MOTORS: STOP");
        } else if   ((x > midX - spread) && (x < midX + spread) &&
                     (y < midY - spread)                       ) {
            showMsg("MOTORS: MOVE FORWARD");
        } else if   ((x > midX - spread) && (x < midX + spread) &&
                     (y > midY + spread)                       ) {
            showMsg("MOTORS: MOVE BACKWARD");
        } else if   ((y > midY - spread) && (y < midY + spread) && 
                     (x < midX - spread)                       ) {
            showMsg("MOTORS: ROTATE LEFT");
        } else if   ((y > midY - spread) && (y < midY + spread) && 
                     (x > midX + spread)                       ) {
            showMsg("MOTORS: ROTATE RIGHT");
        } else if ((x < midX - spread) && (y < midY - spread))   {
            showMsg("MOTORS: MOVE LEFT FORWARD");
        } else if ((x > midX + spread) && (y < midY - spread))   {
            showMsg("MOTORS: MOVE RIGHT FORWARD");
        } else if ((x < midX - spread) && (y > midY + spread))   {
            showMsg("MOTORS: MOVE LEFT BACKWARD");
        } else if ((x > midX + spread) && (y > midY + spread))   {
            showMsg("MOTORS: MOVE RIGHT BACKWARD");
        }
    }
}

void showMsg(String str) {
    if (millis() - lcdDisplayTimer > 200) {
        Serial.print("midX = ");
        Serial.print(midX);
        Serial.print(", midY = ");
        Serial.print(midY);
        Serial.print(", X = ");
        if (x < 10) {
            Serial.print("  ");
        } else if ((x > 10) && (x < 100)) {
            Serial.print(" ");
        }
        Serial.print(x);
        Serial.print(", Y = ");
        if (y < 10) {
            Serial.print("  ");
        } else if ((y > 10) && (y < 100)) {
            Serial.print(" ");
        }
        Serial.print(y);
        Serial.print(", SW = ");
        Serial.print(sw);
        Serial.print(", MNP = ");
        Serial.print(mnpEnable);
        Serial.print("   - ");
        Serial.println(str);

        lcdDisplayTimer = millis();
    }
}