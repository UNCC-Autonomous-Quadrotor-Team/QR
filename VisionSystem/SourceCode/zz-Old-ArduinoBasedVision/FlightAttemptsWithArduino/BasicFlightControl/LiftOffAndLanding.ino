void LiftOff()
{
 throttleValue = 1000;
 
 while(throttleValue < 1500)
 {
   throttleValue += 20;
   Throttle.writeMicroseconds(throttleValue);
   delay(100);
 }

 while(throttleValue < 1700)
 {
   throttleValue += 50;
   Throttle.writeMicroseconds(throttleValue);
   delay(100);
 }
 
  while(throttleValue > 1400)
 {
   throttleValue -= 50;
   Throttle.writeMicroseconds(throttleValue);
   delay(100);
 }

 delay(5000);

}

void Landing()
{
 while(throttleValue > 1400)
  {
   throttleValue -= 50;
   Throttle.writeMicroseconds(throttleValue);
   delay(100);
 }
  while(throttleValue > 1000)
  {
   throttleValue -= 100;
   Throttle.writeMicroseconds(throttleValue);
   delay(100);
 }
 
 Throttle.writeMicroseconds(1000);
  
}

