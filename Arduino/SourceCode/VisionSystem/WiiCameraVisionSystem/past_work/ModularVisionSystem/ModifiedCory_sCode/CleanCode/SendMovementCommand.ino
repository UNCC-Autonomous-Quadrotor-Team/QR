//*************************************************************************************

void SendMovementCommand(){

  GetDistanceFromBeacon();

  int distanceObservedFromBeacon3To2 = (TBlob3.X - TBlob2.X);
  int distanceObservedFromBeacon2To1 = (TBlob2.X - TBlob1.X);
  int act_midpoint   = 512;
  int midpoint       = TBlob2.X;
  int actual_depth   = length;
  int ideal_depth    = 48;
  int dOb13          = (TBlob3.X - TBlob1.X);

  int lat_tolerance = 30;   // Observed resolution
  int len_tolerance = 1;    // inches

//  Serial.print("Actual = ");
//  Serial.print(actual_depth);
//  Serial.print(", Ideal= ");
//  Serial.println( ideal_depth);

//  actual_depth = scaling * distanceObservedFromBeacon3To2;
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
//  Serial.print("DOb12 = ");
//  Serial.print("Midpoint");
//  Serial.println(midpoint);
  
  //adjusting laterally
  if(midpoint > (act_midpoint + lat_tolerance)){
    Serial.println("Left");
  }
  else if(midpoint < (act_midpoint - lat_tolerance)){
    Serial.println("Right");
  }
  else if (actual_depth > (ideal_depth + len_tolerance)){
    //move towards beacon
    Serial.println("Forward");
  }
  else if (actual_depth < (ideal_depth - len_tolerance)){
    //move away from beacon
    Serial.println("Back");
  }
  else{
    //hover
    Serial.println("In position");
  }   
  
  Serial.println("--------------------------------------");
}

//*************************************************************************************
//*************************************************************************************

void GetDistanceFromBeacon()
{
  int distObser3To1 = TBlob3.X - TBlob1.X;

  //from 20" to greater than or equal to 36"
  if((distObser3To1 >= 381) && (distObser3To1 < 697)){
    length = (1097 - distObser3To1) / 20;
    Serial.print("Distance from camera: ");
    Serial.print(length);
    Serial.println(" in.");
  }
  //from 36" to greater than or equal to 60"
  if((distObser3To1 < 381) && (distObser3To1 >= 222)){
    length = (623 - distObser3To1) / 7;
    Serial.print("Distance from camera: ");
    Serial.print(length);
    Serial.println(" in.");
  }
  //from 60" to greater than or equal to 84"
  if((distObser3To1 < 222) && (distObser3To1 >= 143)){
    length = (342 - distObser3To1) / 2;
    Serial.print("Distance from camera: ");
    Serial.print(length);
    Serial.println(" in.");
  }
}
