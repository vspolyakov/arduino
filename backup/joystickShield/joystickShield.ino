/*
  Vitaliy Polyakov (c), polyakov.vs@mail.ru, 2024
  Joystick Shield.
*/

#define VRX A0    // A0
#define VRY A1    // A1
#define SW 8      // D8

#define BTN_A 2   // D2
#define BTN_B 3   // D3
#define BTN_C 4   // D4
#define BTN_D 5   // D5
#define BTN_E 6   // D6
#define BTN_F 7   // D7

void setup() {
  Serial.begin(9600);

  pinMode(VRX, INPUT);
  pinMode(VRY, INPUT);
  pinMode(SW, INPUT_PULLUP);

  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);
  pinMode(BTN_C, INPUT_PULLUP);
  pinMode(BTN_D, INPUT_PULLUP);
  pinMode(BTN_E, INPUT_PULLUP);
  pinMode(BTN_F, INPUT_PULLUP);
}

void loop() {
  printData();
}

void printData() {

  Serial.print("JOYSTICK (");
  Serial.print(analogRead(VRX));
  Serial.print(", ");
  Serial.print(analogRead(VRY));
  Serial.print(", ");
  Serial.print(digitalRead(SW));
  Serial.print(").\t");

  Serial.print("BUTTONS(");
  Serial.print("A");
  Serial.print(digitalRead(BTN_A));
  Serial.print(", ");

  Serial.print("B");
  Serial.print(digitalRead(BTN_B));
  Serial.print(", ");

  Serial.print("C");
  Serial.print(digitalRead(BTN_C));
  Serial.print(", ");

  Serial.print("D");
  Serial.print(digitalRead(BTN_D));
  Serial.print(", ");

  Serial.print("E");
  Serial.print(digitalRead(BTN_E));
  Serial.print(", ");

  Serial.print("F");
  Serial.print(digitalRead(BTN_F));
  Serial.print(")");

  Serial.print("\n");
}


