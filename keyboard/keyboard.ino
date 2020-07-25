// test code for sending keystrokes from Arduino
// to computer via HID Bluetooth module
// This version has the Arduino and Bluetooth module acting like a
//wireless keyboard.
#include <avr/io.h>
#include<SoftwareSerial.h>
SoftwareSerial bt(3,2);
uint8_t upInput = 8; //Pin numbers for the Cardinal Directions
uint8_t leftInput = 9;
uint8_t rightInput = 10;
uint8_t downInput = 11;
#define upKey 0x01 //Used for Bit Masking each direction
#define leftKey 0x02
#define rightKey 0x04
#define downKey 0x08
uint8_t button1Input = 9; //Pin numbers corresponding to each button
uint8_t button2Input = 10;
uint8_t button3Input = 4;
uint8_t button4Input = 5;
uint8_t button5Input = 6;
uint8_t button6Input = 7;


uint8_t button7Input = 12;
uint8_t button8Input = 11;
//Bit Masks for each button, along with HID scan code values
#define button1 0x04 //Y = 0x1C; T = 0x17;
#define button2 0x08 //U = 0x18; F = 0x09;
#define button3 0x10 //I = 0x0C; M = 0x10;
#define button4 0x20 //O = 0x12; G = 0x0A;
#define button5 0x40 //H = 0x0B; R = 0x15;
#define button6 0x80 //J = 0x0D; N = 0x11;
#define button7 0x10 //K = 0x0E; P = 0x13;
#define button8 0x01 //L = 0x0F; V = 0x19;
//LED pin used for debugging. It should not be used as a button input.
uint8_t led = 13;
//Variables tracking which buttons are pressed and which direction the
uint8_t stickState = 0;
uint8_t buttonState = 0;
//Arrays tracking which buttons are currently active
uint8_t activeKeys[6] = {
0};
uint8_t listOfPortDButtons[7] = {
button8, button1, button2, button3, button4, button5, button6};
uint8_t listOfPortBButtons[5] = {
upKey, leftKey, rightKey, downKey, button7};
uint8_t numberOfBButtons = 5;
uint8_t numberOfDButtons = 7;
//DirectionTransmissions
uint8_t keyPress = 0x00;
uint8_t keyTwoPress = 0x00;
uint8_t upRelease = 0x00;
uint8_t leftRelease = 0x00;
uint8_t rightRelease = 0x00;
uint8_t downRelease = 0x00;
//Initialize the button states to 0
uint8_t button1Press = 0x00;
uint8_t button2Press = 0x00;
uint8_t button3Press = 0x00;
uint8_t button4Press = 0x00;
uint8_t button5Press = 0x00;
uint8_t button6Press = 0x00;
uint8_t button7Press = 0x00;
uint8_t button8Press = 0x00;
uint8_t button1Release = 0x00;
uint8_t button2Release = 0x00;
uint8_t button3Release = 0x00;
uint8_t button4Release = 0x00;
uint8_t button5Release = 0x00;
uint8_t button6Release = 0x00;
uint8_t button7Release = 0x00;
uint8_t button8Release = 0x00;

int8_t xValue = 0;
int8_t yValue = 0;
int8_t zValue = 0;
int8_t rotValue = 0;
void setup(){
//set input states
pinMode(upInput, INPUT);
pinMode(downInput, INPUT);
pinMode(rightInput, INPUT);
pinMode(leftInput, INPUT);
pinMode(button1Input, INPUT);
pinMode(button2Input, INPUT);
pinMode(button3Input, INPUT);
pinMode(button4Input, INPUT);
pinMode(button5Input, INPUT);
pinMode(button6Input, INPUT);
pinMode(button7Input, INPUT);
pinMode(button8Input, INPUT);
pinMode(led, OUTPUT);
//set pull-up resistors
//digitalWrite(upInput, HIGH);
//digitalWrite(downInput, HIGH);
//digitalWrite(leftInput, HIGH);
//digitalWrite(rightInput, HIGH);
//digitalWrite(button1Input, HIGH);
//digitalWrite(button2Input, HIGH);
//digitalWrite(button3Input, HIGH);
//digitalWrite(button4Input, HIGH);
//digitalWrite(button5Input, HIGH);
//digitalWrite(button6Input, HIGH);
//digitalWrite(button7Input, HIGH);
//digitalWrite(button8Input, HIGH);
digitalWrite(led,LOW);
//the joystick switches are only present on PORTB
//the buttons are used in PORTD, but PIN1 of portD has no button
//The button state variable is bit masked to account for this
stickState = PINB;
buttonState = PIND & 0xFD;
bt.begin(9600);

}
void loop()
{

if( (PINB != stickState) || ((PIND & 0xFD) != buttonState) )
{
delay(4);
stickState = PINB;
buttonState = PIND & 0xFD;
updateActiveKeys();
}
}
///////////////////////////////////////////////////////////////////////
uint8_t getState(uint8_t REG, uint8_t KEY)
{
return (REG & KEY) ^ KEY;
}
///////////////////////////////////////////////////////////////////////
void updateActiveKeys(){
// release unpressed keys
for (uint8_t i = 0; i < 6; i++) {
if (activeKeys[i]) {
uint8_t key = getKeyMask(activeKeys[i]);
uint8_t port = getPortForKeyValue(activeKeys[i]);
//if the key is not being pressed, it's spot in the HID report is
//replaced with a release value of 0x00.
if(!getState(port, key)) {
activeKeys[i] = 0x00;
}
}
}
// send release message
sendButtons();
// add newly pressed keys
for (uint8_t i = 0; i < numberOfDButtons; i++) {
if(getState(buttonState, listOfPortDButtons[i])){
uint8_t keyValue = getPortDKeyValue(listOfPortDButtons[i]);
uint8_t firstEmptySlot = 10;
uint8_t found = 0;
for (uint8_t j = 0; j < 6; j++) {
if ( (activeKeys[j] == 0x00) && (firstEmptySlot > 5) ) {
firstEmptySlot = j;
}
else if (activeKeys[j] == keyValue) {
found = 1;
break;
}
}
if (!found && firstEmptySlot < 6) {
activeKeys[firstEmptySlot] = keyValue;
}
}
}
for (uint8_t i = 0; i < numberOfBButtons; i++) {

if(getState(stickState, listOfPortBButtons[i])){
uint8_t keyValue = getPortBKeyValue(listOfPortBButtons[i]);
uint8_t firstEmptySlot = 10;
uint8_t found = 0;
for (uint8_t j = 0; j < 6; j++) {
if ( (activeKeys[j] == 0x00) && (firstEmptySlot > 5) ) {
firstEmptySlot = j;
}
else if (activeKeys[j] == keyValue) {
found = 1;
break;
}
}
if (!found && firstEmptySlot < 6) {
activeKeys[firstEmptySlot] = keyValue;
}
}
}
// send new HID report with buttons added
sendButtons();
}
///////////////////////////////////////////////////////////////////////
uint8_t getPortBKeyValue(uint8_t KEY){
uint8_t value;
switch (KEY) {
case upKey :
value = 0x1A;
break;
case downKey :
value = 0x16;
break;
case leftKey :
value = 0x04;
break;
case rightKey :
value = 0x07;
break;
case button7 :
value = 0x0E;
break;
}
return value;
}
///////////////////////////////////////////////////////////////////////
uint8_t getKeyMask(uint8_t VALUE){
uint8_t mask;
switch (VALUE) {
case 0x1A :
mask = upKey;
break;
case 0x16 :
mask = downKey;
break;
case 0x04 :
mask = leftKey;
break;
case 0x07 :
mask = rightKey;
break;
case 0x0E :
mask = button7;
break;
case 0x1C:
mask = button1;
break;
case 0x18:
mask = button2;
break;
case 0x0C :
mask = button3;
break;
case 0x12 :
mask = button4;
break;
case 0x0B :
mask = button5;
break;
case 0x0D :
mask = button6;
break;
case 0x0F :
mask = button8;
break;
}
return mask;
}
///////////////////////////////////////////////////////////////////////
uint8_t getPortDKeyValue(uint8_t KEY){
uint8_t value;
switch (KEY) {
case button1:
value = 0x1C;
break;
case button2:
value = 0x18;
break;
case button3 :
value = 0x0C;
break;
case button4 :
value = 0x12;
break;
case button5 :
value = 0x0B;
break;
case button6 :
value = 0x0D;
break;
case button8 :
value = 0x0F;
break;
}
return value;
}
///////////////////////////////////////////////////////////////////////
uint8_t getPortForKeyValue(uint8_t VALUE) {
switch (VALUE) {
case 0x1A:
case 0x16:
case 0x04:
case 0x07:
case 0x0E:
return stickState;
break;
case 0x1C:
case 0x18:
case 0x0C:
case 0x12:
case 0x0B:
case 0x0D:
case 0x0F:
return buttonState;
break;
}
}
///////////////////////////////////////////////////////////////////////
void sendButtons() {
bt.write(uint8_t(0xFD));
bt.write(uint8_t(0x09));
bt.write(uint8_t(0x01));
bt.write(uint8_t(0x00));
bt.write(uint8_t(0x00));
for (uint8_t i = 0; i < 6; i++) {
bt.write(activeKeys[i]);
}
}

