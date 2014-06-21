// Sweep
// by BARRAGAN <http://barraganstudio.com> 

#include <Servo.h> 
#include <Wire.h>
#include <PVision.h>

Servo myservo2;  // create servo object to control a servo 
                 // a maximum of eight servo objects can be created 
                
PVision ircam;

byte result;
int ledPin = 13;   // LED connected to digital pin 13 
int pos = 0;       // variable to store the servo position 
int blob1X[3];
int blob1Y[3];
int blob1Xsum = 0;
int blob1Ysum = 0;
int blob1Xavg = 0;
int blob1Yavg = 0;
int n = 3;

int i;
 
void setup() 
{ 
  myservo2.attach(9);  // attaches the servo on pin 9 to the servo object 
  Serial.begin(9600);
  Serial.println("1");
  ircam.init();
  Serial.println("3");
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
} 
 
 
void loop() 
{ 
  //Serial.println("Begin For Loop");
  for (i = 0; i < n; i = i +1){
    
  result = ircam.read();
  
    
//    if (result & BLOB1)
//    {
//  
////      Serial.print("BLOB1 detected. X:");
////      Serial.print(ircam.Blob1.X);
////      Serial.print(" Y:");
////      Serial.print(ircam.Blob1.Y);
////      Serial.print(" Size:");
////      Serial.println(ircam.Blob1.Size);
////      pos = ircam.Blob1.X / 5;
////      myservo2.write(pos);
//    }
    
//    if (result & BLOB2)
//    {
//      digitalWrite(ledPin, HIGH);   // sets the LED on
//      Serial.print("BLOB2 detected. X:");
//      Serial.print(ircam.Blob2.X);
//      Serial.print(" Y:");
//      Serial.print(ircam.Blob2.Y);
//      Serial.print(" Size:");
//      Serial.println(ircam.Blob2.Size);
//    }
//    if (result & BLOB3)
//    {
//      Serial.print("BLOB3 detected. X:");
//      Serial.print(ircam.Blob3.X);
//      Serial.print(" Y:");
//      Serial.print(ircam.Blob3.Y);
//      Serial.print(" Size:");
//      Serial.println(ircam.Blob3.Size);
//    }
//    if (result & BLOB4)
//    {
//      Serial.print("BLOB4 detected. X:");
//      Serial.print(ircam.Blob4.X);
//      Serial.print(" Y:");
//      Serial.print(ircam.Blob4.Y);
//      Serial.print(" Size:");
//      Serial.println(ircam.Blob4.Size);
//    }
//      digitalWrite(ledPin, LOW);   // sets the LED on
 
if (result & BLOB1){     

    // Get sum of points for average of blotch 1
    blob1X[i] = ircam.Blob1.X;
    blob1Xsum += ircam.Blob1.X;
    blob1Y[i] = ircam.Blob1.Y;
    blob1Ysum += ircam.Blob1.Y;
    
//    Serial.print("Reading X");
//    Serial.print(i);
//    Serial.print(": ");
//    Serial.println(blob1X[i]);
//    Serial.print("Reading Y");
//    Serial.print(i);
//    Serial.print(": ");
//    Serial.println(blob1Y[i]);

  
    }
  }
    if ((blob1Xsum != 0)&& (blob1Ysum != 0)){
    blob1Xavg = blob1Xsum / n;
    Serial.print("Xave: ");
    Serial.println(blob1Xavg);
    blob1Xsum = 0;
    
    blob1Yavg = blob1Ysum / n;
    Serial.print("Yave: ");
    Serial.println(blob1Yavg);
    blob1Ysum = 0;
  
  Serial.println("-------------------------------------------");
    }
}