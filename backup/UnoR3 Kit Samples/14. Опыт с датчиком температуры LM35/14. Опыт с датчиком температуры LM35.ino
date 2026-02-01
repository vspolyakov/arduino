int raw = 0;
float temp = 0;

void setup() {
    Serial.begin(9600);
    pinMode( A0, INPUT );
}

void loop() {
    raw = analogRead(A0);
    temp = (((raw*5.0)/1024.0)-0.5)*100; //эта формалу нужна для правильного преобразования аналогового сигнала в градусы Цельсия
    Serial.println(temp);
    delay(1000); 
}