//------------------------------------------------------------
// Code Developed by:       Cory Engel
// Date Last Modified:      11/27/2013
//
// Description:
//      This code is used to be a controller for autonomous 
// operation with the quadrotor. The state of a switch is saved 
// and output over Xbee network to tell the quad to either start
// or stop its operations. (A kill switch if you will)
// 
// References:
//       N/A - Original Code
//------------------------------------------------------------
// Global Variables
byte input;

void setup(){
  // Set up the Xbee on the Serial COM port (Pin 0,1)  
  Serial.begin(9600);
  
  // Define Digital Pin 2 for Switch
  pinMode(2, INPUT);
  // Define Digital Pin 7 for LED
  pinMode(7, OUTPUT);
}

void loop(){
  // Read the state of the switch (0 or 1)
  int SW1 = digitalRead(2);

  // Save HIGH when the switch is pressed
  if (SW1 == 1){
    input = 1;
    // Toggle LED ON
    digitalWrite(7, HIGH);
  }
  // Save LOW when the switch is not pressed
  if (SW1 == 0){
    input = 0;
    // Toggle LED OFF
    digitalWrite(7, LOW);
  }

  // Ouput the state of the Switch over Xbee network
  Serial.write(input);
} 
