//*************************************************************************************

/*
 *  Contents
 *    - TroubleShooting_PrintPointCoordinates
 *    - TroubleShooting_CheckYaw
 *
 */

//*************************************************************************************
//*************************************************************************************

void TroubleShooting_PrintPointCoordinates()
{
  result = ircam.read();
  if (result & BLOB1)
  {
    Serial.print("BLOB1 detected. X:");
    Serial.print(ircam.Blob1.X);
    Serial.print(" Y:");
    Serial.print(ircam.Blob1.Y);
    Serial.print(" Size:");
    Serial.println(ircam.Blob1.Size);
  }
  
  if (result & BLOB2)
  {
    Serial.print("BLOB2 detected. X:");
    Serial.print(ircam.Blob2.X);
    Serial.print(" Y:");
    Serial.print(ircam.Blob2.Y);
    Serial.print(" Size:");
    Serial.println(ircam.Blob2.Size);
  }
  if (result & BLOB3)
  {
    Serial.print("BLOB3 detected. X:");
    Serial.print(ircam.Blob3.X);
    Serial.print(" Y:");
    Serial.print(ircam.Blob3.Y);
    Serial.print(" Size:");
    Serial.println(ircam.Blob3.Size);
  }
  if (result & BLOB4)
  {
    Serial.print("BLOB4 detected. X:");
    Serial.print(ircam.Blob4.X);
    Serial.print(" Y:");
    Serial.print(ircam.Blob4.Y);
    Serial.print(" Size:");
    Serial.println(ircam.Blob4.Size);
  }
}

//*************************************************************************************
//*************************************************************************************

void TroubleShooting_CheckYaw()
{
//  //Check points
//  Serial.print("Blob1 is ");
//  Serial.println(TBlob1.X);
//  
//  Serial.print("Blob2 is ");
//  Serial.println(TBlob2.X);
//  
//  Serial.print("Blob3 is ");
//  Serial.println(TBlob3.X);
//  
//  // Determine relative distances
//  Serial.print("Blob2-1 is ");
//  Serial.println(TBlob2.X-TBlob1.X);
//  
//  Serial.print("Blob3-2 is ");
//  Serial.println(TBlob3.X-TBlob2.X);
  
  // Print difference  
  Serial.print("Midpoint difference is ");
  Serial.println((TBlob2.X-TBlob1.X)-(TBlob3.X-TBlob2.X));
  
  Serial.println("--------------------------------------"); 
}

//*************************************************************************************
//*************************************************************************************

void PrintNowDistanceFromBeacon()
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

  Serial.print("Distance from camera: ");
  Serial.print(distanceFromBeacon);
  Serial.println(" in.");
}  

//*************************************************************************************
