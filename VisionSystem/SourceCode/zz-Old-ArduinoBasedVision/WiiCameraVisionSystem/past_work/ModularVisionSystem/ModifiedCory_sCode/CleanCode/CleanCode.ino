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


void setup() { 
  Serial.begin(9600);
  ircam.init();
} 

//Main loop
void loop() { 
  
  int readingsToAverage = 1;
  
  GetAverageBlobXYCoordinates(readingsToAverage);

  if (IsLogical()) SendMovementCommand(); 


}


