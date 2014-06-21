void CalibrateAllChannels()
{
  for(int i = 0; i < 3; i++)
  {
    CalibrateSingleChannel(Throttle);
    CalibrateSingleChannel(Pitch);
    CalibrateSingleChannel(Roll);
    CalibrateSingleChannel(Yaw);
    CalibrateSingleChannel(Aux1);
  }
}

void CalibrateSingleChannel(Servo servo)
{
  servo.writeMicroseconds(1000);
  delay(100);
  servo.writeMicroseconds(2000);
  delay(100);
}

void SetInitialValues()
{
  Throttle.writeMicroseconds(throttleInitialValue);
  Pitch.writeMicroseconds(pitchInitialValue);
  Roll.writeMicroseconds(rollInitialValue);
  Yaw.writeMicroseconds(yawInitialValue);
  Aux1.writeMicroseconds(aux1StableFlight);  
}
