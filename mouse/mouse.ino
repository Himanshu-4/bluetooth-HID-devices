#include<SoftwareSerial.h>
SoftwareSerial b(3,2);

int bt1 = 4;//Button 1
int bt2 = 5;//Button 2

int bt3 = 6;//Wheel Down
int bt4 = 7;//Wheel Up

int bt5 = 8;//Up
int bt6 = 9;//Down
int bt7 = 10;//Left
int bt8 = 11;//Right

void readData(void);
void mouseSend(void);
void mouseClear(void);


void setup() {                

b.begin(9600);
Serial.begin(9600);
}


void loop() {
   
  if(analogRead(A2)>900)//Button 1
    {
    
    mouseClear();//necessary for button activation  
    mouseSend(1,0,0,0);
    delay(50);
    mouseClear();
    }
//  if(digitalRead(bt2)==0)//Button 2
//    {
//    digitalWrite(led, LOW);
//    mouseClear();//necessary for button activation
//    mouseSend(2,0,0,0);
//    delay(200);
//    mouseClear();
//    }
//  if(digitalRead(bt3)==0)//Wheel Down
//    {
//    digitalWrite(led, HIGH);
//    mouseSend(0,0,0,-1);   
//    }
//  if(digitalRead(bt4)==0)//Wheel Up
//    {
//    digitalWrite(led, LOW);
//    mouseSend(0,0,0,1);    
//    }
  if(analogRead(A1)<200)//Up
    {
   
    mouseSend(0,0,-10,0);    
    }
  if(analogRead(A1)>700)//Down
    {
   
    mouseSend(0,0,10,0);   
    }
  if(analogRead(A0)<200)//Left
    {
    
    mouseSend(0,-10,0,0);   
    }
  if(analogRead(A0)>800)//Right
    {
     
    mouseSend(0,10,0,0);  
    }



} 
   
void mouseSend(int button,int x, int y,int wheel)
  {
  int value=0x00;
 
  b.write(0xFD);
  b.write(0x05);
  b.write(0x02);
  b.write(button);//botão
  b.write(x);//(x);
  b.write(y);//(y);
  b.write(wheel);//(wheel);
  
  delay(20);
  }
  
void mouseClear(void)
  {
  int value=0x00;
 
  b.write(0xFD);
  b.write(0x05);
  b.write(0x02);
  b.write(value);//botão
  b.write(value);//(x);
  b.write(value);//(y);
  b.write(value);//(wheel);
  
  delay(20);
  }
