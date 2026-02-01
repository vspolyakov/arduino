#include <iostream>

using namespace std;

int getSlaveMotorSpeedS(int x, int y);

int main() {
  int _x = 0;
  int _y = 0;



  cout << "Enter x [0-255]: ";
  cin >> _x;
  cout << "Enter y [0-255]: ";
  cin >> _y;
  cout << "Slave motor speed = " << speedSlaveMotor(_x, _y);

  return 0;
}

int getSlaveMotorSpeedS(int x, int y) {
  return (int)(y - (y * (float)x / 256));
}
