int pushButton = 2;
// проведем необходимые установки:
void setup() {
// инициализируем последовательную передачу данных со скоростью 9600 бит в секунду:
 Serial.begin(9600);
// назначим вывод 2 входом:
 pinMode(pushButton, INPUT);
}
// основной цикл:
void loop() {
// читаем значение на входе:
 int buttonState = digitalRead(pushButton);
// вывод и значение на монитор:
Serial.println(buttonState);
delay(1); // задержка для стабильного считывания
}