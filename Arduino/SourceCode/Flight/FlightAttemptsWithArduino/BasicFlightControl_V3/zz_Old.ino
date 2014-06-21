// Before I realized how to calibrate through mission planner
static void ScaleRoll(Servo servo)
{
  int x = 1531;
  
  if ((x >= 1125) && (x<=1531))
  {
    servo.writeMicroseconds(x+125);
    return; 
  }
  if ((x > 1531) && (x<=1962))
  {
    servo.writeMicroseconds(x+0.174*x-141);
    return;
  }  
}

