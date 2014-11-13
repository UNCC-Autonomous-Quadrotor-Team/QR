//----------------------------------------------------------------
//----------------------------------------------------------------
#include <Wire.h>
#include <PVision.h>
#include "Header.h"

void setup() 
{ 
  Serial.begin(9600);
  ircam.init();
} 

//Main loop
void loop() 
{ 
  GetAverageBlobXYCoordinates();

  if (IsLogical()) SendMovementCommand();
}


