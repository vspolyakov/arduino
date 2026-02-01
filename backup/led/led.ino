int LED8 = 8;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED8, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(LED8, HIGH);
  delay(500);
  digitalWrite(LED8, LOW);
  delay(500);
}
