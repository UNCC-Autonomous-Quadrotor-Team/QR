//------------------------------------------------------------
// Code Developed by:  Cory Engel
// Date Last Modified:      1/27/2014
//
// Description:
//      This code is used to calibrate the ESC's of the quad
// for autonomous operation. The signal from the receiver does 
// not have the full swing of 1000us, so this code enables a 
// full swing of the PWM. Before the battery is plugged in, set
// the switch to high (1), and after you hear the beeps, 
// switch position to low (0).
// 
// References:
//      https://code.google.com/p/arducopter/wiki/AC2_ESC
//------------------------------------------------------------
#include <Servo.h>

// Define Servo object to be used for PWM signal
Servo ESC;

void setup(){
  // Enable Digital Pin 8 as an input for a Switch
  pinMode(8, INPUT);
  
  // Attach PWM signal to Digital Pin 5
  ESC.attach(5);
}

void loop(){
  // Read the state of the switch (0 or 1)
  int Switch = digitalRead(8);
  
  // If switch is pressed (1), send out max PWM signal
  if(Switch == 1){
    ESC.writeMicroseconds(2000);
  }
  // If switch is not pressed (0), send out min PWM signal
  if (Switch == 0){
    ESC.writeMicroseconds(1000);
  }
}

// Future Work:
//      Determine if this code can be enhanced to allow for all
//      4 ESC's to be calibrated at once.
//
//      Note: sometimes all 4 ESCs give errors when programmed 
//      at the same time.
