#include <Wire.h>
#include <PVision.h>
#include <Servo.h>
#include "Header.h"

void setup() { 
  Serial.begin(9600);
  
  Throttle.attach(3); Pitch.attach(5);  
  Roll.attach(6); Yaw.attach(9);
  Aux1.attach(10);
} 

//Main loop
void loop() 
{ 

 // Make an interrupt to throw safemode on switch flick 
  
 //CalibrateAllChannels();
  
 SetInitialValues();
 
 if(true) EnableSafemode();   
 
 if(true) DisableSafemode();
 
 //LiftOff();

 //Landing();
 
 if(true) EnableSafemode();
 
 while(true)
 {
 }

} 

//  delay(500);
//  //Read in the array of data sent from Xbee controller
//  while(Serial.available()){
//    input = Serial.read();
//  }
//  // Disable SAFE MODE on the quad when the button is pressed
//  if ((input == 1) && (flag == 1)){
//    // Throttle at 0 and Yaw all the way to the right
//    Throttle.writeMicroseconds(1000);
//    Yaw.writeMicroseconds(2000);
//    // Hold this PWM signal for 3 sec
//    delay(3000);
//    // Return to initial positions with no input
//    Throttle.writeMicroseconds(1000);
//    Yaw.writeMicroseconds(1500); 
//    flag = 0;     
//    x = 1000;
//    // Delay for 10 sec to make sure SAFE MODE is off and ready
//    delay(10000);
//  }
//  // Enable SAFE MODE when button is not pressed
//  else if ((input == 0) && (flag == 0)){
//    // Throttle at 0 and Yaw all the way to the left
//    Throttle.writeMicroseconds(1000);
//    Yaw.writeMicroseconds(1000);
//    // Hold this PWM signal for 3 sec
//    delay(3000);
//    // Return to initial positions with no input
//    Throttle.writeMicroseconds(1000);
//    Yaw.writeMicroseconds(1500);
//    flag = 1;  
//  }
//    Takeoff();
//    getCameraData(3);
//    checkLogical();
//
//
// Function to simulate Take Off. The quad will increase throttle
// until a specific height is reached and then hold.
//void Takeoff(){
//  if (x < 1150){
//    Throttle.writeMicroseconds(x);
//    x += 10; 
//    //delay(50);     
//  } 
//  else{
//    Throttle.writeMicroseconds(1150);
//  }
//}

