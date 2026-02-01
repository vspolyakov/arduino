int buzzer=11; //переменной buzzer присваиваем номер выходного пина (11)

void setup()
{
  pinMode(buzzer,OUTPUT);//11 пин будет работает как выходной 
}

void loop() 
{
  unsigned char i,j;//задаем переменные
  while(1) //цикл while
  {
    for(i=1;i<80;i++)//выходная частота звука 
    {
      digitalWrite(buzzer,HIGH);//зуммер издает звук, за счет HIGH
      delay(1);//задержка 1 ms 
      digitalWrite(buzzer,LOW);// зуммер не издает звук, за счет LOW
      delay(1);// задержка 1 ms
  }
  for(i=0;i<100;i++)//другая выходная частота звука 
    {
    digitalWrite(buzzer,HIGH);// зуммер издает звук, за счет HIGH 
    delay(2);// задержка 2 ms 
    digitalWrite(buzzer,LOW);// зуммер не издает звук, за счет LOW
    delay(2);// задержка 2ms
    }
  }
}
