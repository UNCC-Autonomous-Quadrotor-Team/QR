//------------------------------------------------------------
// Code Developed by:       Cory Engel
// Date Last Modified:      1/27/2014
//
// Description:
//      This code enables the quadrotor to fly autonomously. It 
// is combined with "AutonomousController" code to communicate
// over Xbee network. On a button press the quad will gain 
// altitude until a heght of 100cm is reached and enter Alt Hold
// mode and wait for the beacon. The beacon will be kept is the 
// middle of the camera and PWM is used to adjust the quad to 
// move in the desired direction.
// 
// References:
//       N/A - Original Code
//------------------------------------------------------------
#include <Servo.h>

// Global variables
byte input;
int flag = 1;
int x = 1000;

// Define Servo objects to be used for PWM signal 
Servo Throttle;
Servo Pitch;
Servo Roll;
Servo Yaw;
Servo Aux1;

void setup(){
  // Set up the Xbee on the Serial COM port (Pin 0,1)
  Serial.begin(9600);
  
  // Attach PWM signal to respective pins on the RedBoard
  Throttle.attach(3);
  Pitch.attach(5);
  Roll.attach(6);
  Yaw.attach(9);
  Aux1.attach(10);
  
  // Set the initial values to replicate the controller 
  // with no input
  Throttle.writeMicroseconds(1000);
  Pitch.writeMicroseconds(1500);
  Roll.writeMicroseconds(1500);
  Yaw.writeMicroseconds(1500);
  Aux1.writeMicroseconds(1200);
}

void loop(){
  // Read in the data sent from the Xbee network
  while(Serial.available()){
    input = Serial.read();
  }
  // Disable SAFE MODE on the quad when the button is pressed
  if ((input == 1) && (flag == 1)){
    // Throttle at 0 and Yaw all the way to the right
    Throttle.writeMicroseconds(1000);
    Yaw.writeMicroseconds(2000);
    // Hold this PWM signal for 3 sec
    delay(3000);
    // Return to initial positions with no input
    Throttle.writeMicroseconds(1000);
    Yaw.writeMicroseconds(1500); 
    flag = 0;     
    x = 1000;
    // Delay for 10 sec to make sure SAFE MODE is off and ready
    delay(10000);
  }
  // Enable SAFE MODE when button is not pressed
  else if ((input == 0) && (flag == 0)){
    // Throttle at 0 and Yaw all the way to the left
    Throttle.writeMicroseconds(1000);
    Yaw.writeMicroseconds(1000);
    // Hold this PWM signal for 3 sec
    delay(3000);
    // Return to initial positions with no input
    Throttle.writeMicroseconds(1000);
    Yaw.writeMicroseconds(1500);
    flag = 1;  
  }
    Takeoff();
}

// Function to simulate Take Off
void Takeoff(){
  // Slowly increase the throttle until a MAX is reached
  if (x < 1300){
    Throttle.writeMicroseconds(x);
    x += 10;
    delay(100);
  } 
  else{
    Throttle.writeMicroseconds(1300);
  }
}

// Future Work:
//      Use the "SoftwareSerial" Library to enable the ability to
//      program the RedBoard while the Xbee is still connected.
