/*
MIT License

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

#define row1 6
#define row2 7              // these are rows pin of keypad
#define row3 8

#define column1 9
#define column2 10          // these are column pins for keypad 
#define column3 11 
#define column4 12


#define selectpin 4
#define startpin 3
#define analogpin 2

uint16_t lastBtnState = 0;
int8_t lastX1 = 0;
int8_t lastY1 = 0;
int8_t lastX2 = 0;
int8_t lastY2 = 0;

uint16_t btnstate = 0;
int8_t x1 = 0;
int8_t y1 = 0;
int8_t x2 = 0;
int8_t y2 = 0;

uint16_t btn_state;

//These are our button constants
#define PSB_SELECT      0x0001
#define PSB_L3          0x0002
#define PSB_R3          0x0004
#define PSB_START       0x0008
#define PSB_PAD_UP      0x0010
#define PSB_PAD_RIGHT   0x0020
#define PSB_PAD_DOWN    0x0040
#define PSB_PAD_LEFT    0x0080                // these are hex values for joystick buttons 
#define PSB_L2          0x0100
#define PSB_R2          0x0200
#define PSB_L1          0x0400
#define PSB_R1          0x0800
#define PSB_TRIANGLE    0x1000
#define PSB_CIRCLE      0x2000
#define PSB_CROSS       0x4000
#define PSB_SQUARE      0x8000


// defination of array with all the hex code 
uint16_t arr[] = { PSB_L3, PSB_SELECT,PSB_R3,PSB_START,PSB_PAD_UP ,PSB_PAD_RIGHT ,PSB_PAD_DOWN ,PSB_PAD_LEFT ,
                  PSB_L2  ,PSB_R2  ,PSB_L1 ,PSB_R1 ,PSB_TRIANGLE , PSB_CIRCLE ,PSB_CROSS, PSB_SQUARE };

boolean select, start,l1,l2,r1,r2, l3, r3, up,left,right,down,triangle,circle ,cross, squar;

void setup(){
pinMode(row1,OUTPUT);
pinMode(row2,OUTPUT);
pinMode(row3,OUTPUT);
pinMode(column1,INPUT_PULLUP);
pinMode(column2,INPUT_PULLUP);
pinMode(column3,INPUT_PULLUP);
pinMode(column4,INPUT_PULLUP);
pinMode(selectpin,INPUT_PULLUP);
pinMode(startpin,INPUT_PULLUP);

  Serial.begin(9600);
}
void loop(){
	
    // now the first thing we have to do is to scan for the columns by multiplexing and find the buttons press
    digitalWrite(row1,LOW); digitalWrite(row2,HIGH);digitalWrite(row3,HIGH);
    delay(50);
    up = !digitalRead(column1); right = !digitalRead(column2); left = !digitalRead(column3); down = !digitalRead(column4);
    // now switch to the next row 
    digitalWrite(row1,HIGH); digitalWrite(row2,LOW); digitalWrite(row3,HIGH);
    delay(50);
    cross = !digitalRead(column1); circle = !digitalRead(column2); triangle = !digitalRead(column3); squar = !digitalRead(column4);
    // now switch to the last row 
    digitalWrite(row1,HIGH); digitalWrite(row2,HIGH); digitalWrite(row3,LOW);
    delay(50);
    r2 = !digitalRead(column1); l2 = !digitalRead(column2); l1= !digitalRead(column3); r1 = !digitalRead(column4);

   select = !digitalRead(selectpin);
   start = !digitalRead(startpin);

   if(analogRead(A0)>800)l3= 1;else l3 =0;
   if (analogRead(A5)>800)r3 =1;else r3 =0;
x1 = (analogRead(A2) - 512 )/4;
y1 = (analogRead(A1) - 512 )/4;
x2 = (analogRead(A4) - 512 )/4;
y2 = (analogRead(A3) - 512 )/4;

if(x1>=-25 && x1<=25) x1=0;
if(x2>=-25 && x2<=25) x2=0;
if(y1>=-25 && y1<=25) y1=0;
if(y2>=-25 && y2<=25) y2=0;

btnstate = l3<<15 | select<<14 | r3<<13 | start<<12 | up<<11 | right<<10 | down<<9 | left<<8 | l2<< 7 | r2<<6 
            | l1<<5 | r1<< 4 | triangle<<3 | circle<<2 | cross<<1 | squar;

    
		if (lastBtnState != btnstate || lastX1 != x1 || lastY1 != y1 || lastX2 != x2 || lastY2 != y2 )
		{
			for(int i =0; i<=15; i++){
				if(btnstate>>i & 1) btn_state |= arr[15-i];
			}
     if (btnstate ==0)btn_state =0;

			sendGamepadState(btn_state, x1, y1, x2 ,y2 );
			lastBtnState = btnstate;
			lastX1 = x1;
			lastY1 = y1;
			lastX2 = x2;
			lastY2 = y2;
		}

	}

// if you don't want to use arrays then you can use this function for saving some ram
//uint8_t return_keyState(uint8_t i){
//	uint8_t keyvalue =0;
//	switch (i)
//	{
//	  case 15:
//		keyvalue = PSB_SELECT;
//		break;
//		case 14:
//		keyvalue = PSB_L3;
//		break;
//		case 13:
//		keyvalue = PSB_R3;
//		break;
//		case 12:
//		keyvalue = PSB_START;
//		break;
//		case 11:
//		keyvalue =PSB_PAD_UP;
//		break;
//		case 10:
//		keyvalue =  PSB_PAD_RIGHT;
//		case 9:
//		keyvalue = PSB_PAD_DOWN;
//		break;
//		case 8:
//		keyvalue = PSB_PAD_LEFT;
//		break;
//		case 7:
//		keyvalue = PSB_L2;
//		break;
//		case 6:
//		keyvalue = PSB_R2;
//		break;
//		case 5:
//		keyvalue = PSB_L1;
//		break;
//		case 4:
//		keyvalue = PSB_R1;
//		break;
//		case 3:
//		keyvalue = PSB_TRIANGLE;
//		break;
//		case 2:
//		keyvalue = PSB_CIRCLE;
//		break;
//		case 1:
//		keyvalue =PSB_CROSS;
//		break;
//		case 0:
//		keyvalue = PSB_SQUARE;
//
//	default:
//	keyvalue =0;
//		break;
//    
//	};
// return keyvalue;
//} 

// now this function will send the hex code to bluetooth device 
void sendGamepadState(uint16_t btn_state, int8_t x1, int8_t y1, int8_t x2, int8_t y2)
{
	Serial.write((uint8_t)0xFD);
	Serial.write((uint8_t)0x06);
	Serial.write((uint8_t)x1);
	Serial.write((uint8_t)y1);
	Serial.write((uint8_t)x2);
	Serial.write((uint8_t)y2);
	uint8_t btnState1 = btn_state & 0xFF;
	uint8_t btnState2 = (btn_state >> 8) & 0xFF;
	Serial.write((uint8_t)btnState1);
	Serial.write((uint8_t)btnState2);

}
