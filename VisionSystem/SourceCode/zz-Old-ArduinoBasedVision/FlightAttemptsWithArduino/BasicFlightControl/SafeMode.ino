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

void EnableSafemode()
{
    // Throttle at 0 and Yaw all the way to the left
    Throttle.writeMicroseconds(1000);
    Yaw.writeMicroseconds(1000);
    // Hold this PWM signal for 3 sec
    delay(3000);
    // Return to initial positions with no input
    Throttle.writeMicroseconds(throttleInitialValue);
    Yaw.writeMicroseconds(yawInitialValue); 
    
    delay(3000);
}
