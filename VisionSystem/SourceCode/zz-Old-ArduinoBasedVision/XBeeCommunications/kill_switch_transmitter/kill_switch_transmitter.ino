
#include <SoftwareSerial.h>
SoftwareSerial xbee(4,5);  // Setup a software serial interface.
int switch_activated = 2;
int LED = 7;
int switchstate = 0;

void setup(){
  Serial.begin(9600); //Serial Interface to terminal.
  /*
  *RX is digital pin 4
  *TX is digital pin 5
  */
  xbee.begin(9600); //Serial interface to xbee. Note this must be different from regular serial. 
  
 pinMode(switch_activated,INPUT); //Set the physical switch as an input to pin 2.
 digitalWrite(switch_activated,HIGH); // Enable pull up resistor for the switch/
 pinMode(LED,OUTPUT); //Set output pin 13 as an LED. 

 
  
  
}
/*
* Main Loop
*/

void loop(){ 
 switchstate = digitalRead(switch_activated); //Read the status of the switch. If it is pressed or not.
 if(!switchstate){ // If the switch is activated
   digitalWrite(LED,HIGH);  // Turn on the LED on pin 7.
   xbee.print('X');  // Print the character X to xbee register for transmission. 
   Serial.println('X');
   delay(2000); // Wait for a couple of seconds in order to prevent the receiver from getting overloaded.
   switchstate = 1;
   
 }
 else{  //When the Switch is turned off..
  digitalWrite(LED,LOW);   //Make sure the LED (pin 7) is off, and nothing transmitts.
  Serial.println('NO');
 }
  
}

