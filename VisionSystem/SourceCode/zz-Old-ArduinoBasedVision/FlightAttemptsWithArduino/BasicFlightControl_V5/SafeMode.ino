void CheckForKillSwitch()
{
  while(xbee.available())
  {
    xbeeMessage = xbee.read();
  }
  
  if(xbeeMessage == 'X')
  {
    EnableSafemode();
    while(true)
    {
    }  
  }
}

  void EnableSafemode()
  { 
    // Throttle at 0 and Yaw all the way to the left
    Throttle.writeMicroseconds(1000);
    Yaw.writeMicroseconds(1000);
    // Hold this PWM signal for 3 sec
    delay(3000); // Change to 3000
    // Return to initial positions with no input
    Throttle.writeMicroseconds(1000);
    Yaw.writeMicroseconds(1500); 
    
    delay(3000); 
  }

void ArmForFlight()
{
  // Throttle at 0 and Yaw all the way to the right
  Throttle.writeMicroseconds(1000);
  Yaw.writeMicroseconds(2000);
  // Hold this PWM signal for 3 sec
  delay(3000);
  // Return to initial positions with no input
  Throttle.writeMicroseconds(1000);
  Yaw.writeMicroseconds(1500); 
  
  delay(3000);
}
