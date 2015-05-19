/*
 * This code is to isolate the YAW.
 *
 *          SETUP:
 *          Connect the Wii Camera to the Arduino RedBoard: Connect GND, 
 *          3.3V, 5V, SDA to Pin 4, SCL to Pin 5. Set Arduino IDE to use 
 *          board as 'Arduino Uno' and set the COM port. 
 */

#include <Servo.h> 
#include <Wire.h>
#include <PVision.h>
#include <Blobs.h>

Servo myservo2;  // create servo object to control a servo 
                
PVision ircam;

byte result;

void setup() 
{ 
  WiiCameraInitialize();
  ServoSetup();
} 
 
void loop() 
{ 
  result = ircam.read();    
  checkLogical();
  
  //PrintPointCoordinates();

  //Serial.println(InvertCoordinates(12,25));
 
  /*** Sorting all points ***/ 
    
    
  
  
    
//  //Isolating the yaw
//  Serial.print("3-2 Difference: ");
//  Serial.println(ircam.Blob3.X-ircam.Blob2.X);
//
//  Serial.print("4-3 Difference: ");
//  Serial.println(ircam.Blob4.X-ircam.Blob3.X);
  
 //Divider();
}


  
