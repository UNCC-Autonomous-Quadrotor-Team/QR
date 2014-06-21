void WarmUp()
{
 throttleValue = 1000;
 int IterationsUntilKill= 1;
 
 while(throttleValue < 1300 && IterationsUntilKill < 20)
 {
   throttleValue += 50;
   Throttle.writeMicroseconds(throttleValue);
   delay(100);
    
   IterationsUntilKill++;  
 }
 
  delay(3000); 
}

void LiftOff()
{
 int IterationsUntilKill= 1;
 
 while(throttleValue < 1400 && IterationsUntilKill < 20)
 {
   throttleValue += 50;
   Throttle.writeMicroseconds(throttleValue);
   delay(100);
    
   IterationsUntilKill++;  
 }
 
  delay(1000); 
//  
//  while(throttleValue > 1400)
//  {
//    throttleValue -= 50;
//    Throttle.writeMicroseconds(throttleValue);
//    delay(100);
//  }



}
void Landing()
{
// while(throttleValue > 1400)
//  {
//   throttleValue -= 50;
//   Throttle.writeMicroseconds(throttleValue);
//   delay(100);
// }
  while(throttleValue > 1000)
  {
   throttleValue -= 100;
   Throttle.writeMicroseconds(throttleValue);
   delay(100);
  }

 Throttle.writeMicroseconds(1000);
  
}

