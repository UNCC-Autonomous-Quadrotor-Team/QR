//----------------------------------------------------------------
//Vision System for Wii IR Camera to be integrated with quadrotor
//Description:
//          This code.....
//
//Last Update: 10/10/2013
//Programmed by: Cory Engel and Audrow Nash
//----------------------------------------------------------------
#include <Wire.h>
#include <PVision.h>
#include <Servo.h>
#include "Average.h"

//Global variables               
PVision ircam;
byte result;
int LEDFront = 2;
int LEDBack = 4;
int LEDRight = 3;
int LEDLeft = 5;
int LEDin = 6;
double length;
byte input;
int flag = 1;
int x = 1000;

//Define Servo objects to be used for PWM
Servo Throttle;
Servo Pitch;
Servo Roll;
Servo Yaw;
Servo Aux1;

void setup() { 
  Serial.begin(9600);
  
  ircam.init();
  pinMode(2, OUTPUT);
  
  //Define the PWM signal pins on the RedBoard
  Throttle.attach(3);
  Pitch.attach(5);
  Roll.attach(6);
  Yaw.attach(9);
  Aux1.attach(10);
  
  //Set the initial values to replicate the controller
  Throttle.writeMicroseconds(1000);
  Pitch.writeMicroseconds(1500);
  Roll.writeMicroseconds(1500);
  Yaw.writeMicroseconds(1500);
  Aux1.writeMicroseconds(1200);
} 

//Main loop
void loop() { 
  //Read in the array of data sent from Xbee controller
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
    getCameraData(3);
    checkLogical();
}

// Function to simulate Take Off. The quad will increase throttle
// until a specific height is reached and then hold.
void Takeoff(){
  if (x < 1150){
    Throttle.writeMicroseconds(x);
    x += 10; 
    //delay(50);     
  } 
  else{
    Throttle.writeMicroseconds(1150);
  }
}

//Gets data from Wii IR Camera and computes an average of the detected X and Y coordinates
void getCameraData(int n){

  //Initialize all variables to 0 to make sure no garbage was saved from previous usage
  int X1 = 0,Y1 = 0,X2 = 0,Y2 = 0,X3 = 0,Y3 = 0,X4 = 0,Y4 = 0;
  int n1 = 0,n2 = 0,n3 = 0,n4 = 0;
  Blob1Avg.X = 0,Blob1Avg.Y = 0,Blob2Avg.X = 0,Blob2Avg.Y = 0,Blob3Avg.X = 0,Blob3Avg.Y = 0,Blob4Avg.X = 0,Blob4Avg.Y = 0;

  for (int i = 0; i < n; i++){
    result = ircam.read();  
    if (result & BLOB1){
      X1 += ircam.Blob1.X;
      Y1 += ircam.Blob1.Y;
      n1++;
    }
    if (result & BLOB2){
      X2 += ircam.Blob2.X;
      Y2 += ircam.Blob2.Y;
      n2++;
    }
    if (result & BLOB3){
      X3 += ircam.Blob3.X;
      Y3 += ircam.Blob3.Y;
      n3++;
    }
    if (result & BLOB4){
      X4 += ircam.Blob4.X;
      Y4 += ircam.Blob4.Y;
      n4++;
    }
  }
  //Save average values into the average structure created in the header file
  if ((X1 != 0) && (Y1 != 0)){
    Blob1Avg.X = X1 / n;
    Blob1Avg.Y = Y1 / n;
  }
  if ((X2 != 0) && (Y2 != 0)){
    Blob2Avg.X = X2 / n;
    Blob2Avg.Y = Y2 / n;
  }
  if ((X3 != 0) && (Y3 != 0)){
    Blob3Avg.X = X3 / n;
    Blob3Avg.Y = Y3 / n;
  }
  if ((X4 != 0) && (Y4 != 0)){  
    Blob4Avg.X = X4 / n;
    Blob4Avg.Y = Y4 / n;
  }
  //Short delay for camera
  delay(50);
}

//Checks to make sure that 3 of the observed BLOBS are in line on an "x-axis"
void checkLogical(){
  int tolerance = 15;
  //Compute variables to be used in algorithm
  if((Blob1Avg.Y != 0) && (Blob2Avg.Y !=0) && (Blob3Avg.Y != 0)){
    boolean comp12 = compareY(Blob1Avg.Y,Blob2Avg.Y,tolerance);
    boolean comp13 = compareY(Blob1Avg.Y,Blob3Avg.Y,tolerance);
    boolean comp14 = compareY(Blob1Avg.Y,Blob4Avg.Y,tolerance);
    boolean comp23 = compareY(Blob2Avg.Y,Blob3Avg.Y,tolerance);
    boolean comp24 = compareY(Blob2Avg.Y,Blob4Avg.Y,tolerance);

    //Compare the 5 relevant pairs of points to determine if an axis exists
    if(comp12 && comp13){
      compareX(Blob1Avg,Blob2Avg,Blob3Avg);
      getDistance();
    }
    else if(comp12 && comp14){
      compareX(Blob1Avg,Blob2Avg,Blob4Avg);
      getDistance();
    }
    else if(comp13 && comp14){
      compareX(Blob1Avg,Blob3Avg,Blob4Avg);
      getDistance();
    }
    else if(comp23 && comp24){
      compareX(Blob2Avg,Blob3Avg,Blob4Avg);
      getDistance();
    }
  }
  else{
    digitalWrite(2, LOW);
  }
}

void getDistance(){
  int distOb = TBlob3.X - TBlob1.X;

  //from 20" to greater than or equal to 36"
  if((distOb >= 381) && (distOb < 697)){
    //This formula produced by best fit linear line from observed data (2-3 feet)
    length = (1097 - distOb) / 20;
    Serial.print("Distance from camera: ");
    Serial.print(length);
    Serial.println(" in.");
    moveQuad();
    digitalWrite(2, HIGH);
  }
  //from 36" to greater than or equal to 60"
  if((distOb < 381) && (distOb >= 222)){
    //This formula produced by best fit linear line from observed data (3-5 feet)
    length = (623 - distOb) / 7;
    Serial.print("Distance from camera: ");
    Serial.print(length);
    Serial.println(" in.");
    moveQuad();
    digitalWrite(2, HIGH);
  }
  //from 60" to greater than or equal to 84"
  if((distOb < 222) && (distOb >= 143)){
    //This formula produced by best fit linear line from observed data (3-5 feet)
    length = (342 - distOb) / 2;
    Serial.print("Distance from camera: ");
    Serial.print(length);
    Serial.println(" in.");
    moveQuad();
    digitalWrite(2, HIGH);
  }
  
}

//Determines which direction to move based on where the 3 usable BLOBS are located
void moveQuad(){
  int dOb23          = (TBlob3.X - TBlob2.X);
  int dOb12          = (TBlob2.X - TBlob1.X);
  int act_midpoint   = 512;
  int midpoint       = TBlob2.X;
  int actual_depth   = length;
  int ideal_depth    = 48;
  int dOb13          = (TBlob3.X - TBlob1.X);
  int pitch_tolerance = 50;
  int roll_tolerance = 75;

  //Roll
  if(midpoint > (act_midpoint + roll_tolerance)){
    //Move to the right
    Roll.writeMicroseconds(1850);
    delay(800);
  }
  else if(midpoint < (act_midpoint - roll_tolerance)){
    //Move to the left
    Roll.writeMicroseconds(1150);
    delay(800);   
  }
  //Pitch
  else if (actual_depth > (ideal_depth + pitch_tolerance)){
    //Move Forward
    Pitch.writeMicroseconds(1800);
    delay(800);
  }
  else if (actual_depth < (ideal_depth - pitch_tolerance)){
    //Move Backward
    Pitch.writeMicroseconds(1200);
    delay(800);
  }
  else{
    Pitch.writeMicroseconds(1500);
    Roll.writeMicroseconds(1500);
    Yaw.writeMicroseconds(1500);
  }
}

//Compare function to determine if two points are on the same X axis
boolean compareY(int one,int two,int tolerance){
  if(abs(one - two) <= tolerance)
    return true;
  else
    return false;
}

boolean compareX(Average one,Average two, Average three){
  int array_x[3] = {
    one.X,two.X,three.X  };
  int array_y[3] = {
    one.Y,two.Y,three.Y  };
  int t,n,i,s;

  for(n = 0; n < 3; n++){
    for(i = 0; i < 2; i++){
      if(array_x[i] > array_x[i+1]){
        t = array_x[i];
        array_x[i] = array_x[i+1];
        array_x[i+1] = t;
        s = array_y[i];
        array_y[i] = array_y[i+1];
        array_y[i+1] = s;
      }
    }
  }
  TBlob1.X = array_x[0];
  TBlob2.X = array_x[1];
  TBlob3.X = array_x[2];
  TBlob1.Y = array_y[0];
  TBlob2.Y = array_y[1];
  TBlob3.Y = array_y[2];
}

