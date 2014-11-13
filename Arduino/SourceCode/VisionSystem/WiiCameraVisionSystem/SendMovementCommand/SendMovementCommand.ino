//*************************************************************************************

void SendMovementCommand()
{
  InitializeParameters();

  if(hasNotMoved) CheckIfMoveAboutYaw();

  if(hasNotMoved) CheckIfMoveLaterally();

  if(hasNotMoved) CheckIfMoveDistanceFrom(); 
  
  if(hasNotMoved) Hover();
  
  //See TroubleShooting file
  //TroubleShooting_CheckYaw();
}

//*************************************************************************************
//*************************************************************************************

void InitializeParameters()
{
  hasNotMoved = true;
  
  pixelsFromB1ToB3 = TBlob3.X - TBlob1.X;
  pixelsFromB1ToB2 = TBlob2.X - TBlob1.X;
  pixelsFromB2ToB3 = TBlob3.X - TBlob2.X;
  
  yawBeaconDifference = pixelsFromB1ToB2-pixelsFromB2ToB3;
  
  middleBeaconX = TBlob2.X;
  middleBeaconY = TBlob2.Y;  
}

//*************************************************************************************
//*************************************************************************************
// // Should be taken care of by altitude hold mode
//void CheckIfAdjustAltitude()
//{
//  
//  distanceFromBeacon = scaling * dOb13;
//  
//  //Adjusting altitude
//  if (Blob1Avg.Y > (middleBeaconY + tolerance)){  //Change variable for Y to allow for actual 3 points being used
//    //Move Down
//  }
//  else if(Blob1Avg.Y < (middleBeaconY - tolerance)){
//    //Move Up
//  }
//}

//*************************************************************************************
//*************************************************************************************

void CheckIfMoveAboutYaw()
{  
  if(yawBeaconDifference < -yawTolerance)
  {
    Serial.println("Move Quadrotor CW");
    hasNotMoved = false;
  }
  
  else if(yawBeaconDifference > yawTolerance)
  {
    Serial.println("Move Quadrotor CCW");
    hasNotMoved = false;
  }  
}

//*************************************************************************************
//*************************************************************************************

void CheckIfMoveLaterally()
{
  if(middleBeaconX > (idealMiddleOfXAxis + lateralMoveTolerance))
  {
    Serial.println("Move Quadrotor Left");
    hasNotMoved = false;
  }
  
  else if(middleBeaconX < (idealMiddleOfXAxis - lateralMoveTolerance))
  {
    Serial.println("Move Quadrotor Right");
    hasNotMoved = false;
  } 
}

//*************************************************************************************
//*************************************************************************************

void CheckIfMoveDistanceFrom()
{
  GetDistanceFromBeacon();
  
   if (distanceFromBeacon > (idealDistanceFromBeacon + distanceFromBeaconTolerance))
  {
    Serial.println("Move Quadrotor Forward");
    hasNotMoved = false;
  }
  
  else if (distanceFromBeacon < (idealDistanceFromBeacon - distanceFromBeaconTolerance))
  {
    Serial.println("Move Quadrotor Back");
    hasNotMoved = false;
  }  

  PrintDistanceFromBeacon();

}


//*************************************************************************************
//*************************************************************************************

void GetDistanceFromBeacon()
{  
  //from 20" to greater than or equal to 36"
  if((pixelsFromB1ToB3 >= 381) && (pixelsFromB1ToB3 < 697))
    distanceFromBeacon = (1097 - pixelsFromB1ToB3) / 20;

  //from 36" to greater than or equal to 60"
  if((pixelsFromB1ToB3 < 381) && (pixelsFromB1ToB3 >= 222))
    distanceFromBeacon = (623 - pixelsFromB1ToB3) / 7;

  //from 60" to greater than or equal to 84"
  if((pixelsFromB1ToB3 < 222) && (pixelsFromB1ToB3 >= 143)) 
    distanceFromBeacon = (342 - pixelsFromB1ToB3) / 2;
}  

//*************************************************************************************
//*************************************************************************************

void PrintDistanceFromBeacon()
{
    Serial.print("Distance from camera: ");
    Serial.print(distanceFromBeacon);
    Serial.println(" in.");
}

//*************************************************************************************
void Hover()
{
    Serial.println("In position");
}   
