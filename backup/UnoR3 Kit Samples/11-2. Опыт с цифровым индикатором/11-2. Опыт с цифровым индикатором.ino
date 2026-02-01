const int led2 =  2; 
const int led3 =  3; 
const int led4 =  4; 
const int led5 =  5; 
const int led6 =  6; 
const int led7 =  7; 
const int led8 =  8; 

void setup() {
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);
  pinMode(led6, OUTPUT);
  pinMode(led7, OUTPUT);
  pinMode(led8, OUTPUT);
}

void loop() {
 int i = 200;
 // 0  
    digitalWrite(led6, HIGH);
    digitalWrite(led7, HIGH);
    digitalWrite(led8, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);

delay(i);

//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);

// 1  
    digitalWrite(led8, HIGH);
    digitalWrite(led2, HIGH);

delay(i);

//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);

// 2  
    digitalWrite(led7, HIGH);
    digitalWrite(led8, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led3, HIGH);

delay(i);

//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);

// 3  
    digitalWrite(led7, HIGH);
    digitalWrite(led8, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);

delay(i);

//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);


// 4  
    digitalWrite(led6, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led8, HIGH);
    digitalWrite(led2, HIGH);

delay(i);

//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);

// 5  
    digitalWrite(led7, HIGH);
    digitalWrite(led6, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);

delay(i);
    
//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);

// 6  
    digitalWrite(led7, HIGH);
    digitalWrite(led6, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led5, HIGH);

delay(i);

//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);

// 7  
    digitalWrite(led7, HIGH);
    digitalWrite(led8, HIGH);
    digitalWrite(led2, HIGH);

delay(i);

//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);

// 8  
    digitalWrite(led6, HIGH);
    digitalWrite(led7, HIGH);
    digitalWrite(led8, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);

delay(i);

//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);    

// 9  
    digitalWrite(led6, HIGH);
    digitalWrite(led7, HIGH);
    digitalWrite(led8, HIGH);
    digitalWrite(led5, HIGH);
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);

delay(i);

//  Очистка всех сегментов  
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, LOW);
    digitalWrite(led6, LOW);
    digitalWrite(led7, LOW);
    digitalWrite(led8, LOW);

}

