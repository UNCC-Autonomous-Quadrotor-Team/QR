import Xbee 
import time as t



#Establish a serial object for the xbee connection

device_location = '/dev/ttyAMA0'
baudrate = '9600'
xbee_obj = Xbee.Xbee(device_location,baudrate)


#listen on the Wireless channel for any transmissions. 


while 1: 

t.sleep(1) 

xbee.obj.receive_packet()

