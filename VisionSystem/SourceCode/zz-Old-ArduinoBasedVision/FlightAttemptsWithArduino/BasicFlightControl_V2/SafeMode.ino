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
    Serial.println("SafeModeEnabled");  
    
    // Throttle at 0 and Yaw all the way to the left
    Throttle.writeMicroseconds(1000);
    Yaw.writeMicroseconds(1000);
    // Hold this PWM signal for 3 sec
    delay(3000); // Change to 3000
    // Return to initial positions with no input
    Throttle.writeMicroseconds(throttleInitialValue);
    Yaw.writeMicroseconds(yawInitialValue); 
    
    delay(3000); 
    Serial.println("DoneWithSafeMode"); 
  }

void DisableSafemode()
{
  // Throttle at 0 and Yaw all the way to the right
  Throttle.writeMicroseconds(1000);
  Yaw.writeMicroseconds(2000);
  // Hold this PWM signal for 3 sec
  delay(3000);
  // Return to initial positions with no input
  Throttle.writeMicroseconds(throttleInitialValue);
  Yaw.writeMicroseconds(yawInitialValue); 
  
  delay(3000);
}
