import time as t
import Xbee


#open Xbee object


device_location ='/dev/ttyUSB0'
baudrate = '9600'
xbee_obj = Xbee.Xbee(device_location,baudrate)


#Broadcast a request to the immediate cluster for data.
# FRAME IDENTIFIER TYPE:
# 01 - Position Data Request
# 02 - Collision detection alert
# 03 - Command Issue. 
# 04 - Movement Error/Reception Error 
msg = 'Request Acknowledgement'
address = 0xFFFF
options = 0x00
frame_id = 0x00

result = xbee_obj.SendTransmitRequest(msg,address,frame_id,options,1)

#Listen on the channel for responses from the Nodes within the cluster.

for i  in range(0,2) : 
    xbee_obj.receive_packet()
    t.sleep(2)
