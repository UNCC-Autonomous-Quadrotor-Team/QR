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
#include "Average.h"

//Global variables               
PVision ircam;
byte result;
int LEDFront = 2;
int LEDBack = 4;
int LEDRight = 3;
int LEDLeft = 5;
double length;

void setup() { 
  Serial.begin(9600);
  ircam.init();
  pinMode(LEDFront, OUTPUT);
  pinMode(LEDRight, OUTPUT);
  pinMode(LEDBack, OUTPUT);
  pinMode(LEDLeft, OUTPUT);
} 

//Main loop
void loop() { 
  getCameraData(3);
  checkLogical();
  //get distance is called if it is logical
  moveQuad();

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
    Serial.print("X1 Average: ");
    Serial.println(Blob1Avg.X);
    Blob1Avg.Y = Y1 / n;
    Serial.print("Y1 Average: ");
    Serial.println(Blob1Avg.Y);
  }
  if ((X2 != 0) && (Y2 != 0)){
    Blob2Avg.X = X2 / n;
    Serial.print("X2 Average: ");
    Serial.println(Blob2Avg.X);
    Blob2Avg.Y = Y2 / n;
    Serial.print("Y2 Average: ");
    Serial.println(Blob2Avg.Y);
  }
  if ((X3 != 0) && (Y3 != 0)){
    Blob3Avg.X = X3 / n;
    Serial.print("X3 Average: ");
    Serial.println(Blob3Avg.X);
    Blob3Avg.Y = Y3 / n;
    Serial.print("Y3 Average: ");
    Serial.println(Blob3Avg.Y);
  }
  if ((X4 != 0) && (Y4 != 0)){  
    Blob4Avg.X = X4 / n;
    Serial.print("X4 Average: ");
    Serial.println(Blob4Avg.X);
    Blob4Avg.Y = Y4 / n;
    Serial.print("Y4 Average: ");
    Serial.println(Blob4Avg.Y);
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
    Serial.println("Blobs 1,2, and 3 are on an axis");
    getDistance();
  }
  else if(comp12 && comp14){
    compareX(Blob1Avg,Blob2Avg,Blob4Avg);
    Serial.println("Blobs 1,2, and 4 are on an axis");
    getDistance();
  }
  else if(comp13 && comp14){
    compareX(Blob1Avg,Blob3Avg,Blob4Avg);
    Serial.println("Blobs 1,3, and 4 are on an axis");
    getDistance();
  }
  else if(comp23 && comp24){
    compareX(Blob2Avg,Blob3Avg,Blob4Avg);
    Serial.println("Blobs 2,3, and 4 are on an axis");
    getDistance();
  }
  else
    Serial.println("No 3 points exist on the same axis!");
  }
}

void getDistance(){
  int distOb = TBlob3.X - TBlob1.X;
  Serial.println(distOb);
 
  //from 20" to greater than or equal to 36"
  if((distOb >= 381) && (distOb < 697)){
    //This formula produced by best fit linear line from observed data (2-3 feet)
    length = (1097 - distOb) / 20;
    Serial.print("Distance from camera: ");
    Serial.print(length);
    Serial.println(" in.");
    Serial.println("--------------------------------------------------");
  }
  //from 36" to greater than or equal to 60"
  if((distOb < 381) && (distOb >= 222)){
    //This formula produced by best fit linear line from observed data (3-5 feet)
    length = (623 - distOb) / 7;
    Serial.print("Distance from camera: ");
    Serial.print(length);
    Serial.println(" in.");
    Serial.println("--------------------------------------------------");
  }
  //from 60" to greater than or equal to 84"
  if((distOb < 222) && (distOb >= 143)){
    //This formula produced by best fit linear line from observed data (3-5 feet)
    length = (342 - distOb) / 2;
    Serial.print("Distance from camera: ");
    Serial.print(length);
    Serial.println(" in.");
    Serial.println("--------------------------------------------------");
  }
}

//Determines which direction to move based on where the 3 usable BLOBS are located
void moveQuad(){
  int midpoint = 512,tolerance = 50;
  int dOb23 = (TBlob3.X - TBlob2.X);
  int dOb12 = (TBlob2.X - TBlob1.X);
  int scaling = 1; // scaling is to be determined experimentally
  int actual_depth = length;
//  Serial.print("Actual = ");
//  Serial.println(actual_depth);
  int ideal_depth = 48;
  int dOb13 = (TBlob3.X - TBlob1.X);
//  actual_depth = scaling * dOb13;
//  
//  //Adjusting altitude
//  if (Blob1Avg.Y > (midpoint + tolerance)){  //Change variable for Y to allow for actual 3 points being used
//    //Move Down
//  }
//  else if(Blob1Avg.Y < (midpoint - tolerance)){
//    //Move Up
//  }
//  //Rotate about the Yaw
//  else if(Blob2Avg.X > (midpoint + tolerance)){
//    //Rotate Counter-Clockwise
//  }
//  else if(Blob2Avg.X < (midpoint - tolerance)){
//    //Rotate Clockwise
//  }
//
//  //Adjusting Laterally (Roll)
//  else if(dOb12 > dOb23){
//    //Move Left
//    digitalWrite(LEDLeft, HIGH);
//    digitalWrite(LEDRight, LOW);
//    digitalWrite(LEDFront, LOW);
//    digitalWrite(LEDBack, LOW);
//  }
//  else if(dOb12 < dOb23){
//    //Move Right
//    digitalWrite(LEDRight, HIGH);
//    digitalWrite(LEDLeft, LOW);
//    digitalWrite(LEDFront, LOW);
//    digitalWrite(LEDBack, LOW);
//  }
  //Determine distane from beacon (Pitch)
  Serial.print("Actual = ");
  Serial.print(actual_depth);
  Serial.print(", Ideal= ");
  Serial.println( ideal_depth);
  
  if (actual_depth > (ideal_depth)){
    //Move towards beacon
  Serial.println("Toward");
  }
  if (actual_depth < (ideal_depth)){
    //Move away from beacon
    Serial.println("Back");

  }
//  else{
//    // Hover
//  }
}

//Compare function to determine if two points are on the same X axis
boolean compareY(int one,int two,int tolerance){
  if(abs(one - two) <= tolerance)
    return true;
  else
    return false;
}

boolean compareX(Average one,Average two, Average three){
  int array_x[3] = {one.X,two.X,three.X};
  int array_y[3] = {one.Y,two.Y,three.Y};
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
  Serial.println("The ordered points are:");
  for(i = 0; i < 3; i++){
    Serial.print(array_x[i]);
    Serial.print(", ");
    Serial.println(array_y[i]);
  }
  TBlob1.X = array_x[0];
  TBlob2.X = array_x[1];
  TBlob3.X = array_x[2];
  TBlob1.Y = array_y[0];
  TBlob2.Y = array_y[1];
  TBlob3.Y = array_y[2];
}
