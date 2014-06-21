//------------------------------------------------------------
// Code Developed by:     Cory Engel
// Date Last Modified:    1/27/2014
//
// Description:
//      This code is used to calibrate the radio, except in
// this case, for autonomous calibration to mimic the radio. This 
// code is used inside of Mission Planner under the Radio 
// Configuration tab. Click complete when desired values are reached.
// 
// References:
//     https://sites.google.com/site/buildyourownquadcoptercamp/construction/configuring-the-crius-in-mission-planner
//------------------------------------------------------------
#include <Servo.h>

// Define Servo object to be used for PWM signal
Servo Roll;
Servo Pitch;
Servo Yaw;
Servo Throttle;
Servo FlightMode;

// Initial values
int x = 1000;

// Attach each Servo to a respective PWM Pin on the Arduino Uno
void setup(){
  Roll.attach(3);
  Pitch.attach(5);
  Yaw.attach(6);
  Throttle.attach(9);
  FlightMode.attach(10);
}

void loop(){
  // Write the initial value of 1000 to the PWM output
  Roll.writeMicroseconds(x);
  Pitch.writeMicroseconds(x);
  Yaw.writeMicroseconds(x);
  Throttle.writeMicroseconds(x);
  FlightMode.writeMicroseconds(x);
  
  // Increment the pulse by 20 until 2000 is reached and 
  // then start over (process repeats until complete)
  x += 20;
  if (x > 2000){
    x = 1000;
  }
  
  // Delay used to slow down the signal due to the increased 
  // sample rate from Mission Planner and the AIOP V2
  delay(150);
}

