int x;
int y;

int speedMotorA = 0;
int speedMotorB = 0;

float tmp;

void setup() {
  Serial.begin(9600);

}

void loop() {
  y = 100;
  x = 100;

  // tmp = (y - (y * (float)x / 256));
  //tmp = (float)tmp * y;
  // tmp = y - tmp;

  speedMotorA = y;
  speedMotorB = (int)(y - (y * (float)x / 256));


  Serial.print("speedMotorA = ");
  Serial.print(speedMotorA);
  Serial.print("\t");

  Serial.print("speedMotorB = ");
  Serial.print(speedMotorB);
  Serial.print("\t");







  Serial.print("\n");
}
