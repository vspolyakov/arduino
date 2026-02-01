/*
  Vitaliy Polyakov (c), polyakov.vs@mail.ru, 2024.
  Transmitter.
*/

// Left joystick
#define LJOY_VRX A0     // A0
#define LJOY_VRY A1     // A1
#define LJOY_SW 8       // D8

// Right joystick
#define RJOY_VRX A2     // A2
#define RJOY_VRY A3     // A3
#define RJOY_SW 7       // D7

// Buttons
#define BUTTON_A 5      // D5
#define BUTTON_B 4      // D4
#define BUTTON_C 3      // D3
#define BUTTON_D 2      // D2

void setup() {
  Serial.begin(9600);

  pinMode(LJOY_VRX, INPUT);
  pinMode(LJOY_VRY, INPUT);
  pinMode(LJOY_SW, INPUT_PULLUP);

  pinMode(RJOY_VRX, INPUT);
  pinMode(RJOY_VRY, INPUT);
  pinMode(RJOY_SW, INPUT_PULLUP);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP);
  pinMode(BUTTON_D, INPUT_PULLUP);      
}

void loop() {
  Serial.print("LJOY(");  
  Serial.print(map(analogRead(LJOY_VRX), 0, 1023, 0, 255));
  Serial.print(", ");
  Serial.print(map(analogRead(LJOY_VRY), 0, 1023, 0, 255));
  Serial.print(", ");
  Serial.print(!digitalRead(LJOY_SW));
  Serial.print(") ");
  Serial.print("\t");

  Serial.print("RJOY(");
  Serial.print(map(analogRead(RJOY_VRX), 0, 1023, 0, 255));
  Serial.print(", ");
  Serial.print(map(analogRead(RJOY_VRY), 0, 1023, 0, 255));
  Serial.print(", ");
  Serial.print(!digitalRead(RJOY_SW));
  Serial.print(") ");
  Serial.print("\t");

  Serial.print("Buttons(");
  Serial.print(!digitalRead(BUTTON_A));
  Serial.print(", ");
  Serial.print(!digitalRead(BUTTON_B));
  Serial.print(", ");
  Serial.print(!digitalRead(BUTTON_C));
  Serial.print(", ");
  Serial.print(!digitalRead(BUTTON_D));
  Serial.print(")");

  Serial.print("\n");
}
