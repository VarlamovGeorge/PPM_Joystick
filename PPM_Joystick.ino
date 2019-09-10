// PPM_Joystick.ino - Converter of PPM sygnal to USB joystick controls. 
// 
// PPM pulse train:
//   __   ___________________________________   _____   ____   _______   ___ //___   __________   __ HIGH
//     |_|      Sync pulse > 3000 us         |_|     |_|    |_|       |_|   //    |_|          |_|
// Channel pulse 1000 to 2000 us                                                                     LOW
// Pulse start and end on falling edge. The end of one pulse is the begining of next.

// The code uses Jystick library from https://github.com/MHeironimus/ArduinoJoystickLibrary/
#define PPM_PIN 2 //Arduino pin for PPM input
#include <Joystick.h>
Joystick_ Joystick;

unsigned long curTime = 0;
unsigned long oldTime = 1;
volatile int pulseLength = 0;
volatile int channel = 0;

// Pulse timing interupt routine.
void ISR1()
{
  curTime = micros();
  pulseLength = curTime - oldTime;  
  oldTime = curTime;  

  if (pulseLength > 3000) {
    channel = 0;
    Joystick.sendState();
  } else {
    channel++;
    setChannel(channel, pulseLength);  
  }  
}

// The function converts length of the pulse 
// to USB joystick control value.
void setChannel(int ch, int pulse)
{
  // Processing pulses with length 800 - 2200 mks. 
  //float value = (pulse - 800) / 1400.0;
  float value = (pulse - 500) / 1400.0;
  
  if (value < 0) 
    value = 0;
  else if (value > 1.0)
    value = 1.0;

  switch(ch) {
    case 2:
    Joystick.setYAxis(value*255 - 127);
    //Joystick.setXAxis(value*255 - 127);
    break;

    case 3:
    Joystick.setThrottle(value*255);
    //Joystick.setYAxis(value*255 - 127);
    break;

    case 1:
    Joystick.setXAxis(value*255 - 127);
    //Joystick.setZAxis(value*255 - 127);
    break;

    case 4:
    Joystick.setRzAxis(value*360);
    break;

    case 5:
    Joystick.setRxAxis(value*360);
    //Joystick.setThrottle(value*255);    
    break;

    case 6:
    Joystick.setRyAxis(value*360);
    //Joystick.setRudder(value*255);    
    break;

  }
}

void setup()
{
  pinMode(PPM_PIN, INPUT_PULLUP);
  Joystick.begin(false);
  attachInterrupt(digitalPinToInterrupt(PPM_PIN), ISR1, FALLING);
  //attachInterrupt(1, ISR1, FALLING); 
}

void loop()
{
  // Place your debug code here...
  delay(100);
}
