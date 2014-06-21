#include <SoftwareSerial.h>
#include "ModularVisionSystem.h"
SoftwareSerial xbee(2,4);
//This software assumes that all xbees  in the cluster has the required firmware to run at PANID:2000, with Transmit Request API mode. This firmware flash was done with X-CTU
char msg = ' ';
int SWITCH = 8;
int switch_status = 0;
int pitch;
int roll; 
int yaw; 
int throttle;
int auxillary;
int node_id = 1;
void setup(){
  Serial.begin(9600); //Serial Interface to terminal.
  /*
  *RX is digital pin 2
  *TX is digital pin 4
  */
  xbee.begin(9600);  //Xbee serial interface
  
  /*
  *Pin for debugging
  */
   pinMode(SWITCH,INPUT);
  digitalWrite(SWITCH,HIGH);
  
  
}


void loop(){
  
    
    switch(node_id){
     case 1:
     //NODE 1
      node_position(node_id);
      transmit_data_node_1(pitch,roll,yaw,throttle,auxillary);
      break;
      case 2:
      node_position(node_id);
      transmit_data_node_2(pitch,roll,yaw,throttle,auxillary);
      break;
      case 3: 
      node_position(node_id);    
      transmit_data_node_3(pitch,roll,yaw,throttle,auxillary);
      break;
    }
      node_id = node_id + 1; 
      if(node_id > 3){
        node_id = 0;
      }
      delay(1500);
      
      
    
   
}


void transmit_data_node_1(int pitch, int roll, int yaw, int throttle, int auxillary){

  
  xbee.print(0x00,HEX);
  xbee.print(0x0E,HEX);
  xbee.print(0x10,HEX);
  xbee.print(0x00,HEX);
  xbee.print(0x13,HEX);
  xbee.print(0xA2,HEX);
  xbee.print(0x00,HEX);
  xbee.print(0x40,HEX);
  xbee.print(0xA7,HEX);
  xbee.print(0xAD,HEX);
  xbee.print(0x5C,HEX);
  xbee.print(0xFF,HEX);
  xbee.print(0xFC,HEX);
  xbee.print(0x00,HEX);
  xbee.print(0x00,HEX);
  xbee.print(pitch,HEX);
  xbee.print(roll,HEX);
  xbee.print(yaw,HEX);
  xbee.print(throttle,HEX);
  xbee.print(auxillary,HEX);
  
  


}
void transmit_data_node_2(int pitch, int roll, int yaw, int throttle, int auxillary){

  
  xbee.print(0x00,HEX);
  xbee.print(0x0E,HEX);
  xbee.print(0x10,HEX);
  xbee.print(0x00,HEX);
  xbee.print(0x13,HEX);
  xbee.print(0xA2,HEX);
  xbee.print(0x00,HEX);
  xbee.print(0x40,HEX);
  xbee.print(0xA7,HEX);
  xbee.print(0xAD,HEX);
  xbee.print(0x66,HEX);
  xbee.print(0xFF,HEX);
  xbee.print(0xFC,HEX);
  xbee.print(0x00,HEX);
  xbee.print(0x00,HEX);
  xbee.print(pitch,HEX);
  xbee.print(roll,HEX);
  xbee.print(yaw,HEX);
  xbee.print(throttle,HEX);
  xbee.print(auxillary,HEX);
  
  


}

void transmit_data_node_3(int pitch, int roll, int yaw, int throttle, int auxillary){

  
  xbee.print(0x00,HEX);
  xbee.print(0x0E,HEX);
  xbee.print(0x10,HEX);
  xbee.print(0x00,HEX);
  xbee.print(0x13,HEX);
  xbee.print(0xA2,HEX);
  xbee.print(0x00,HEX);
  xbee.print(0x40,HEX);
  xbee.print(0xA7,HEX);
  xbee.print(0xAD,HEX);
  xbee.print(0xD0,HEX);
  xbee.print(0xFF,HEX);
  xbee.print(0xFC,HEX);
  xbee.print(0x00,HEX);
  xbee.print(0x00,HEX);
  xbee.print(pitch,HEX);
  xbee.print(roll,HEX);
  xbee.print(yaw,HEX);
  xbee.print(throttle,HEX);
  xbee.print(auxillary,HEX);
  
  


}

void node_position(int node_id){
   sendMovementCommand(); //Located in the Vision System Dependicies. 
   
}
