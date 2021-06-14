#include <Servo.h>
Servo servoMotor;
int servoPin = 9;
const int PIN = 3;
int buzzer = 6;
const int PRESS = HIGH;
const int NOPRESS = LOW;
bool isOpen = false;
unsigned int index = 0;
int storage[10];
int combinationUpper[10] = { 500, 500, 300, 300, 500, 500, 300, 300, 500, 300 };
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  servoMotor.attach(servoPin);
  pinMode(3, INPUT);
  pinMode(9, OUTPUT);
  pinMode(6, OUTPUT);
}
void loop() {
 unsigned int pressCount = 0;
 unsigned int noPressCount = 0;
  if (digitalRead(PIN) == PRESS) {
    digitalWrite(6, HIGH);
    while (digitalRead(PIN) == PRESS) {
      pressCount += 1;
      delay(10);
    }
  }
  if (digitalRead(PIN) == NOPRESS && pressCount > 5) {
    digitalWrite(buzzer,LOW);
    storage[index] = pressCount;
    Serial.print("index and value: ");
    Serial.println(index);
    Serial.println(pressCount);
    index += 1;
    if (check() == 10) {
      isOpen = true;
      servoMotor.write(90);
      Serial.println("open!!!!!!");
    }
    while (digitalRead(PIN) == NOPRESS) {
      noPressCount +=1;
      if (noPressCount >= 3000) {
        isOpen = false;
        wipe();
        servoMotor.write(0);
        Serial.println("wiping");
        check();
      }
      delay(10);
    }
    if (check () == 10 && pressCount >= 600){
     for (int i =0;i<10;i++){
       if (digitalRead(PIN) == PRESS){
        digitalWrite(buzzer,LOW);
        while (digitalRead (PIN) == PRESS){
          pressCount +=1;
          delay(10);
        }
      }
      Serial.print(i);
      Serial.print(": ");
      Serial.println(storage[i]);
      if (digitalRead(PIN) == NOPRESS && pressCount > 5 && pressCount <350){
        combinationUpper[i]=300;
      }
      if (digitalRead(PIN) == NOPRESS && pressCount > 5 && pressCount <550){
        combinationUpper[i]=500;
      }
      if (pressCount >= 550 && pressCount <1000){
        servoMotor.write(0);
        i--;
      }
     }
  }
  }
}
int check() {
      unsigned int check = 0;
      for(int i = 0; i <= 9; i++) {
      Serial.print(i);
      Serial.print(": ");
      Serial.println(storage[i]);
      if (storage[i] <= combinationUpper[i]+50 && storage[i] > (combinationUpper[i] - 150)) {
          check += 1;
        }
      }
      Serial.print("check: ");
      Serial.println(check);
      return check;
}
void wipe() {
  for(int i = 1; i <= 10; i++) {
    storage[i] = 0;
  }
}
