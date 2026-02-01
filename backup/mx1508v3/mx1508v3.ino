const int FwdPin_A = 4; 
const int BwdPin_A = 3;  
const int FwdPin_B = 6; 
const int BwdPin_B = 5;        

int MaxSpd = 255;               // Скорость, значение 0-255
int delayVal = 2000;

void setup()
{
  pinMode(FwdPin_A, OUTPUT);    // Устанавливаем FwdPin_A как выход
  pinMode(BwdPin_A, OUTPUT);    // Устанавливаем BwdPin_A как выход
  pinMode(FwdPin_B, OUTPUT);    // Устанавливаем FwdPin_B как выход
  pinMode(BwdPin_B, OUTPUT);    // Устанавливаем BwdPin_B как выход
}

void loop()
{
  delay(5000);
  // Движение двигателями A и B вперед
  analogWrite(BwdPin_A, LOW);  
  analogWrite(BwdPin_B, LOW);
  analogWrite(FwdPin_A, MaxSpd);
  analogWrite(FwdPin_B, MaxSpd);
  delay(2000);

  // стоп двигателей
  analogWrite(BwdPin_A, 0);  
  analogWrite(BwdPin_B, 0);
  analogWrite(FwdPin_A, 0);
  analogWrite(FwdPin_B, 0);  
  delay(2000);

  // Движение двигателями A и B назад
  analogWrite(BwdPin_A, MaxSpd);  
  analogWrite(BwdPin_B, MaxSpd);
  analogWrite(FwdPin_A, LOW);
  analogWrite(FwdPin_B, LOW);
  delay(2000);

  // стоп двигателей
  analogWrite(BwdPin_A, 0);  
  analogWrite(BwdPin_B, 0);
  analogWrite(FwdPin_A, 0);
  analogWrite(FwdPin_B, 0);  
  delay(2000);

  // поворот направо
  analogWrite(FwdPin_A, LOW);
  analogWrite(BwdPin_A, LOW);  
  analogWrite(FwdPin_B, MaxSpd);
  analogWrite(BwdPin_B, LOW);
  delay(2000);

 // стоп двигателей
  analogWrite(BwdPin_A, 0);  
  analogWrite(BwdPin_B, 0);
  analogWrite(FwdPin_A, 0);
  analogWrite(FwdPin_B, 0);  
  delay(2000);

  // поворот налево
  analogWrite(FwdPin_A, MaxSpd);
  analogWrite(BwdPin_A, LOW);  
  analogWrite(FwdPin_B, LOW);
  analogWrite(BwdPin_B, LOW);
  delay(2000);  

  // Движение двигателями A и B назад
  analogWrite(BwdPin_A, MaxSpd);  
  analogWrite(BwdPin_B, MaxSpd);
  analogWrite(FwdPin_A, LOW);
  analogWrite(FwdPin_B, LOW);
  delay(2000);

  // вращение вправо
  analogWrite(FwdPin_A, MaxSpd);
  analogWrite(BwdPin_A, LOW);  
  analogWrite(FwdPin_B, LOW);
  analogWrite(BwdPin_B, MaxSpd);
  delay(2000);  

 // стоп двигателей
  analogWrite(BwdPin_A, 0);  
  analogWrite(BwdPin_B, 0);
  analogWrite(FwdPin_A, 0);
  analogWrite(FwdPin_B, 0);  
  delay(2000);



  // вращение влево
  // вращение вправо
  analogWrite(FwdPin_A, LOW);
  analogWrite(BwdPin_A, MaxSpd);  
  analogWrite(FwdPin_B, MaxSpd);
  analogWrite(BwdPin_B, LOW);
  delay(2000);  


 // стоп двигателей
  analogWrite(BwdPin_A, 0);  
  analogWrite(BwdPin_B, 0);
  analogWrite(FwdPin_A, 0);
  analogWrite(FwdPin_B, 0);  
  delay(2000);

/*


    delay(delayVal);
    analogWrite(FwdPin_A, LOW); 
    analogWrite(FwdPin_B, LOW);
// Движение двигателями A и B назад
    analogWrite(BwdPin_A, MaxSpd);  
    analogWrite(BwdPin_B, MaxSpd);
    analogWrite(FwdPin_A, LOW);
    analogWrite(FwdPin_B, LOW); 
    delay(delayVal);
    analogWrite(BwdPin_A, LOW); 
    analogWrite(BwdPin_B, LOW);
// Двигатель A вперед, B назад
    analogWrite(BwdPin_A, LOW);  
    analogWrite(BwdPin_B, MaxSpd);
    analogWrite(FwdPin_A, MaxSpd);
    analogWrite(FwdPin_B, LOW); 
    delay(delayVal);
    analogWrite(BwdPin_A, LOW); 
    analogWrite(BwdPin_B, LOW);
    analogWrite(FwdPin_A, LOW);
    analogWrite(FwdPin_A, LOW);
// Двигатель A назад, B вперед
    analogWrite(BwdPin_A, MaxSpd);  
    analogWrite(BwdPin_B, LOW);
    analogWrite(FwdPin_A, LOW);
    analogWrite(FwdPin_B, MaxSpd); 
    delay(delayVal);
    analogWrite(BwdPin_A, LOW); 
    analogWrite(BwdPin_B, LOW);
    analogWrite(FwdPin_A, LOW);
    analogWrite(FwdPin_A, LOW);
// Двигатели A и B холостой ход
    analogWrite(BwdPin_A, HIGH); 
    analogWrite(BwdPin_B, HIGH);
    analogWrite(FwdPin_A, HIGH);
    analogWrite(FwdPin_A, HIGH);
    delay(delayVal);

    */
}