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
double blob1Xavg = 0;
double blob1Yavg = 0;
double blob1hyp = 0;

Servo servoBlob1;
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
  pinMode(10, OUTPUT);
  
  servoBlob1.attach(10);
} 
 
 
void loop() 
{ 
  //Serial.println("Begin For Loop");
  for (i = 0; i < 3; i = i +1){
    
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
      digitalWrite(ledPin, LOW);   // sets the LED on
      
    blob1X[i] = ircam.Blob1.X;
    Serial.print("Reading X");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(blob1X[i]);
    blob1Xsum += ircam.Blob1.X;
    
    blob1Y[i] = ircam.Blob1.Y;
    Serial.print("Reading Y");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(blob1Y[i]);
    blob1Ysum += ircam.Blob1.Y;
  }
  
  blob1Xavg = blob1Xsum / 3;
  Serial.print("Average Value X: ");
  Serial.println(blob1Xavg);
  blob1Xsum = 0;
  
  blob1Yavg = blob1Ysum / 3;
  Serial.print("Average Value Y: ");
  Serial.println(blob1Yavg);
  blob1Ysum = 0;
  
  blob1hyp = 0.707 * sqrt((blob1Xavg*blob1Xavg) + (blob1Yavg*blob1Yavg));
  Serial.println(blob1hyp);
  analogWrite(10,blob1hyp);
  delay(10);
  Serial.println("-------------------------------------------");

}
