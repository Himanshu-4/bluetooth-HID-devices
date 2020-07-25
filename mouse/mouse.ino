/*MIT License

Copyright (c) 2020 Himanshu jangra

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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
