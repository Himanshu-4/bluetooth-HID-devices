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
SOFTWARE.*/

#include<SoftwareSerial.h>
SoftwareSerial b(3,2);

/* defination of input pins in the keyboard buttons*/
#define channel1 4
#define channel2 5
#define channel3 6

// defination of input pins 
#define in1 7
#define in2 8
#define in3 9
#define in4 10

// bit mask value 
#define up_key     0x52
#define left_key   0x50
#define right_key  0x4F
#define down_key   0x51
#define space_key  0x2C
#define tab_key    0x2B
#define enter_key  0x28
#define w_key      0x1A
#define a_key      0x04
#define s_key      0x16
#define d_key      0x07
#define q_key      0x14
#define e_key      0x08
#define z_key      0x1D
#define x_key      0x1B


// defination of modifiers 
#define shift_key  0x20
#define ctr_key    0x01

// pins for enter and tab key 
#define enter_key_pin 11
#define tab_key_pin 12


// defination of input analog pins 
#define updown_analog A3
#define lefright_analog A4
#define spacekey_analog A5

#define xmouse_analog A0
#define ymouse_analog A1
#define lclk_analog A2     //left click of the mouse button


uint8_t modifier_Key;
uint8_t  left_rightchannel =0, hat_middlechannel=0, modifiers =0;
uint8_t prev_left_rightchannel, prev_hat_middlechannel, prev_modifiers;

volatile uint8_t  activeKeys[6] ={0};
// these are the bool values for keyboard 
bool up,left,down,right,w,a,s,d,q,e,z,x,tab,enter,shift,ctr,space;
// bool values for mouse 
bool lclk, rclk;
int mousex, mousey;

#define ar(a) analogRead(a)



void setup()
{
   // keypad matrix row pin 
pinMode(channel1,OUTPUT);
pinMode(channel2,OUTPUT);
pinMode(channel3,OUTPUT);
// keypad matrix column pins
pinMode(in1,INPUT);
pinMode(in2,INPUT);
pinMode(in3,INPUT);
pinMode(in4,INPUT);

// now we make the assignment of previous values to the current values 
prev_left_rightchannel = left_rightchannel;
prev_hat_middlechannel = hat_middlechannel;
prev_modifiers = modifiers;

b.begin(9600);
}
void loop()
{ 
    mousex = ar(xmouse_analog);
    mousey = ar(ymouse_analog);
    

digitalWrite(channel1,1);digitalWrite(channel2,0);digitalWrite(channel3,0);
delay(50);
w = digitalRead(in1); a= digitalRead(in2); s = digitalRead(in3); d = digitalRead(in4);
// switch to the next row

digitalWrite(channel1,0);digitalWrite(channel2,1);digitalWrite(channel3,0);
delay(50);
q = digitalRead(in1); e = digitalRead(in2); z = digitalRead(in3); x = digitalRead(in4);
// switch to the third row

digitalWrite(channel1,1);digitalWrite(channel2,0);digitalWrite(channel3,1);
delay(50);
shift = digitalRead(in1); ctr = digitalRead(in2); lclk = digitalRead(in3); rclk = digitalRead(in4);

// read the enter and shift key 
enter= digitalRead(enter_key_pin);
tab = digitalRead(tab_key_pin);

// read the space key 
if((analogRead(spacekey_analog)>800))space =1;
else space =0;

// now we got our values from the keys its time to send them through bluetooth
//first send the mouse directions 

if((mousex<300)) //left 
{
     mousesend(0,-30,0,0);
}
if(mousex >700) // right 
{
    
    mousesend(0,30, 0,0);

}
if(mousey > 700)// down 
{
    mousesend(0,0,30,0);

}
if(mousey<300)// up
{
  
    mousesend(0,0,-30,0);
}
// send the moouse clicks
if(((lclk ==1) || (ar(lclk_analog)>800))){
    mouseclear();
    mousesend(1,0,0,0);
    delay(100);
    mouseclear();

}
if(rclk ==1){
    mouseclear();
    mousesend(2,0,0,0);
    delay(100);
    mouseclear();
}

    // the left_right channel and the hat_middle channel are used to keep track of change in key pressed 
left_rightchannel = ( w<<7 | a<< 6 | s <<5 | d << 4 | q<< 3 | e << 2 | z << 1 | x);
hat_middlechannel = ( up<< 7 | left << 6 | right << 5 | down << 4 | tab<< 3 | enter <<2 | space<<1 | 0 );
modifiers = ( shift <<1 | ctr );
// now its time to check whether the keys are been modified or not

if((left_rightchannel !=  prev_left_rightchannel) ||  (hat_middlechannel  != prev_hat_middlechannel ) 
|| (modifiers != prev_modifiers ) )
{
left_rightchannel =  prev_left_rightchannel;
hat_middlechannel  = prev_hat_middlechannel;
updateActiveKeys();
    
}


}


// this is the main functions of this program because it builds  the activeKeys[i] arrays and then send it through sendbuttons()

void updateActiveKeys()
{
    if(modifiers != prev_modifiers){
      if(shift) modifier_Key =0x20;
      else if(ctr) modifier_Key = 0x01;
    else modifier_Key = 0x00;

    }
// now we have to releases the key which are being machanically released by sending a value 0 to that particular key index in the array
for(uint8_t i =0; i<6 ; i++){
// scanned for non zero values in the array 
if(activeKeys[i]){
    uint8_t key = activeKeys[i];
    // if the keys are not being pressed , its spot in the HID report will replaced with 
    // a release value of 0x00
    if(!getState(key)){
        activeKeys[i]=0x00;
    }
}
}
sendbuttons();

// now we have to check for a new key pressed and sent it to through the updatekey array but dont change the 
// previous key that is active now
// now to scan the buttons for whic is pressed and which is not
//left_rightchannel
for(uint8_t i =0; i <= 7; i++){
    if (((left_rightchannel >>i) &1))
    {
        bool found =0;
        uint8_t firstemptyslot =10;
        uint8_t keyvalue =0;
        keyvalue  = getkey_forleft_right(i);
    for (uint8_t j = 0; j< 6; j++)
    {
        if( (activeKeys[j] == 0x00  && firstemptyslot > 5)){
            firstemptyslot = j;
        }
        else if (activeKeys[j] == keyvalue){
            found  =1;
            break;
        }
        }
        if((!found && firstemptyslot <6)){
            activeKeys[firstemptyslot] = keyvalue;
    }
}
    
}

// now its time to send the key value of hat_middlechannel 
for(uint8_t i =1; i <=7; i++){
    if((hat_middlechannel >> i & 1)){
        uint8_t keyvalue = getKey_forhat_middle(i);
        bool found =0;
        uint8_t firstemptyslot =10;
        for(uint8_t j =0; j <6 ;j++)
        {
            if((activeKeys[j] ==0x00 & firstemptyslot > 5)){

                firstemptyslot = j;
            }
            else if (activeKeys[j] == keyvalue){
                found =1;
                break;
            }
        }
        if((!found && firstemptyslot <6)){
            activeKeys[firstemptyslot] = keyvalue;
        }
    }
}
    sendbuttons();
}



bool getState(uint8_t value )
{
    bool x =0;
// scan here for the state by HID codes
    switch (value)
    {
    case up_key:
    if((hat_middlechannel>>7 & 1))x=1;
    break;

    case left_key:
    if((hat_middlechannel >>6 & 1)) x=1;
    break;

    case right_key:
    if((hat_middlechannel >> 5 &1))x=1;
    break;

    case down_key:
    if((hat_middlechannel >>4 & 1))x =1;
    break;

    case tab_key:
    if((hat_middlechannel >>3 & 1))x =1;
    break;
   
   case enter_key:
   if((hat_middlechannel >>2 &1))x=1;
   break;

   case space_key:
   if((hat_middlechannel >>1 & 1)) x=1;
   break;

   // now to get state for the left_rightchannel 
   case w_key:
   if((left_rightchannel >> 7 &1)) x=1;
   break;

   case a_key:
   if((left_rightchannel >> 6 &1)) x=1;
   break;

   case s_key:
   if((left_rightchannel >> 5 &1)) x=1;
   break;

   case d_key:
   if((left_rightchannel >> 4 & 1))x =1;
   break;

   case q_key:
   if((left_rightchannel >> 3 & 1))x=1;
   break;

   case e_key:
   if((left_rightchannel >>2 & 1))x=1;
   break;

   case z_key:
   if((left_rightchannel >>1 & 1)) x=1;
   break;

   case x_key:
   if((left_rightchannel & 1)) x=1;
   break;

   default:
   x=0;


    }
   return x;

}

uint8_t getKey_forhat_middle(uint8_t VALUE_ofi)
{
    uint8_t mask;
    switch (VALUE_ofi) {
case 7:
mask = up_key;
break;
case 6 :
mask = left_key;
break;
case 5 :
mask = right_key;
break;
case 4 :
mask = down_key;
break;
case 3:
mask = tab_key;
break;
case 2:
mask = enter_key;
break;
case 1:
mask = space_key;
break;
    }
    return mask;
}

uint8_t getkey_forleft_right(uint8_t value_ofi){
uint8_t mask =0;
switch(value_ofi){

case 7:
mask = w_key;
break;
case  6:
mask = a_key;
break;
case 5 :
mask = s_key;
break;
case 4 :
mask = d_key;
break;
case 3 :
mask = q_key;
break;
case 2:
mask = e_key;
break;
case 1 :
mask = z_key;
break;
case 0:
mask = x_key;
break;
}
return mask;
}

// function for the getportfor key value to make sure that it is actually a known key press 

// function to send the activeKeys[i] buffer to the connected device 
void sendbuttons()
{
  uint8_t value = 0x00;
  
  b.write((uint8_t)0xFD);
  b.write((uint8_t)0x09);
  b.write((uint8_t)0x01); 
  b.write(modifier_Key);
  b.write(value);
  for(uint8_t i =0 ; i<6; i++){
    b .write(activeKeys[i]);
}
}
void mouseclear(){ 
  uint8_t value = 0x00;
  
  b.write((uint8_t)0xFD);
  b.write((uint8_t)0x05);
  b.write((uint8_t)0x02);
  b.write(value);
  b.write(value);
  b.write(value);
  b.write(value);
  
  delay(20);
}
void mousesend(uint8_t button, uint8_t x, uint8_t y ,uint8_t wheel){
 
  b.write((uint8_t)0xFD);
  b.write((uint8_t)0x05);
  b.write((uint8_t)
  0x02);
  b.write(button);//buttons
  b.write(x);//(x)
  b.write(y);//(y)
  b.write(wheel);//(wheel)
  
  delay(20);
}
