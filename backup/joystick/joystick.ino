/*
  Vitaliy Polyakov (C), polyakov.vs@mail.ru, 2024.
*/

#define JOY_VRX A0 // pin A0
#define JOY_VRY A1 // pin A1
#define JOY_SW 13  // pin D13

int midx; // middle x
int midy; // middle y
int variability;
int x; // 0..1023
int y; // 0..1023

byte forward;  // motor direction (1, 0)
byte backward; // motor direction (1, 0)
byte left;  // motors rotate left
byte right; // motors rotate right 
byte speedMotorA; // 0..255
byte speedMotorB; // 0..255


void setup() {
  Serial.begin(9600);
  pinMode(JOY_VRX, INPUT);
  pinMode(JOY_VRY, INPUT);  
  pinMode(JOY_SW, INPUT_PULLUP);

  // default values
  variability = 25;
  forward = 0;
  backward = 0;
  left = 0;
  right = 0;
  speedMotorA = 0;
  speedMotorB = 0;
  midx = analogRead(JOY_VRX);
  midy = analogRead(JOY_VRY);
}

void loop() {
  // current x, y values
  x = analogRead(JOY_VRX);
  y = analogRead(JOY_VRY);

  if (x <= midx - variability) {
    // left rotation
    left = 1;
    right = 0;
  } else if (x > midx + variability) {
    // right rotation
    left = 0;
    right = 1;    
  } else {
    // stop rotation
    left = 0;
    right = 0;     
  }
  x = convert(x);

  // motor direction
  if (y <= midy - variability) {
    // forward motors direction
    forward = 1;
    backward = 0;
    // speed motors
    speedMotorA = convert(y);
    speedMotorB = convert(y);
  } else if (y > midy + variability) {
    // backward motors direction
    forward = 0;
    backward = 1;
    // speed motors
    
    speedMotorA = convert(y);
    speedMotorB = convert(y);
  } else {
    // stop motors
    forward = 0;
    backward = 0;
    // speed motors

    speedMotorA = 0;
    speedMotorB = 0;
  }    













  Serial.print("midx = ");
  Serial.print(midx);
  Serial.print("\t");

  Serial.print("midy = ");
  Serial.print(midy);
  Serial.print("\t");

  Serial.print("x = ");
  Serial.print(x);
  Serial.print("\t");

  Serial.print("y = ");
  Serial.print(y);
  Serial.print("\t");

  Serial.print("forward = ");
  Serial.print(forward);
  Serial.print("\t");

  Serial.print("backward = ");
  Serial.print(backward);
  Serial.print("\t");  

  Serial.print("left = ");
  Serial.print(left);
  Serial.print("\t");

  Serial.print("right = ");
  Serial.print(right);
  Serial.print("\t");  

  Serial.print("speedMotorA = ");
  Serial.print(speedMotorA);
  Serial.print("\t");

  Serial.print("speedMotorB = ");
  Serial.print(speedMotorB);
  Serial.print("\t");

  Serial.print("\n");
}

int convert(int tmp_val) {
  if (tmp_val >= 511) {
    tmp_val = tmp_val - 511;
  } else {
    tmp_val = 511 - tmp_val;
  }
  return map(tmp_val, 0, 511, 0, 255);
}
