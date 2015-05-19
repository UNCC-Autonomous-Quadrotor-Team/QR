// Sweep
// by BARRAGAN <http://barraganstudio.com> 

#include <Servo.h> 
#include <Wire.h>
#include <PVision.h>

int ActPos[2]  = {-1, -1};  // {X, Y} Actual Position
int DesPos[2] = {0, 0};   // {X, Y} Desired Position
int Done = 0;


void setup() 
{ 
  Serial.begin(9600);
} 
 
 
void loop() 
{ 
  // Check if in the correct position
  if ((ActPos[0] == DesPos[0]) && (ActPos[1] == DesPos[1]) && (Done != 1))  {
    Serial.println("Stay");
    Done =1;
  }
  
  // Check Y value and move down
  if (ActPos[1] > DesPos[1]) {
    Serial.println("Down");
    ActPos[1] = ActPos[1] - 1;
  }
  
  // Check Y value and move up
  if (ActPos[1] < DesPos[1]) {
    Serial.println("Up");
    ActPos[1] = ActPos[1] + 1;
  }
  
  // Check X value and move left
  if (ActPos[0] > DesPos[0]) {
    Serial.println("Left");
    ActPos[0] = ActPos[0] - 1;
  }
  
  // Check X value and move right
  if (ActPos[0] < DesPos[0]) {
    Serial.println("Right");
    ActPos[0] = ActPos[0] + 1;
  }
}
