#include <SoftwareSerial.h>
SoftwareSerial xbee(2,4);
char msg = ' ';
void setup(){
  Serial.begin(9600); //Serial Interface to terminal.
  /*
  *RX is digital pin 2
  *TX is digital pin 4
  */
  xbee.begin(9600);  //Xbee serial interface
}


void loop(){

      //digitalWrite(13,LOW);
     while(xbee.available()){    //until Xbee receives data
       msg = xbee.read();
       //Serial.println(msg);
     }
       if(msg == 'X'){          //only prints when transmitter is transmitting
        //digitalWrite(13,HIGH);
        Serial.println(msg);
        msg = ' ';
       }
}
