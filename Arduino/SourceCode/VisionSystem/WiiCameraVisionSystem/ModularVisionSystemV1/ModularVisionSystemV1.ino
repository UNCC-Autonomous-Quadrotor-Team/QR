/* 
 *  Note: 
 *    - Tools/Board -> Arudino Uno   // To program Redboard
 *    - PVision must be added to libraries before compiling,
 *      and is included in the folder of this code
 *    - To connect camera to RedBoard, make all connections:
 *      GND, 3.3V, 5V, SDA, SCL (SDA to A4, SCL to A5)
 *    - Test camera operation with the function 
 *      'TroubleShooting_PrintPointCoordinates();' which will
 *      give information about upto 4 observed light sources
 *
 *  For questions, contact Audrow Nash
 *      anash13@uncc.edu
 *      860 818 1026
 */

#include <Wire.h>
#include <PVision.h>
#include "ModularVisionSystem.h"

void setup() 
{ 
  Serial.begin(9600);
  ircam.init();
} 

//Main loop
void loop() 
{ 
  //TroubleShooting_PrintPointCoordinates();

  GetAverageBlobXYCoordinates();

  if (IsLogical()) SendMovementCommand();
  
  //PrintNowDistanceFromBeacon();

}


