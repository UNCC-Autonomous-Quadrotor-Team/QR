import time as t
import Xbee


#open Xbee object


device_location ='/dev/ttyUSB0'


baudrate = '9600'
xbee_obj = Xbee.Xbee(device_location,baudrate)


#Broadcast a request to the immediate cluster for data.
# COMMAND IDENTIFIER TYPE:
# 01 - Position Data Request
# 02 - Collision detection alert
# 03 - Command Issue. 
# 04 - Movement Error/Reception Error 
#msg = 'Request Acknowledgement'
msg = 2
cmd_id = 10
destination_address = 0xFFFF
options = 0x00

result = xbee_obj.SendTransmitRequest(msg,destination_address,cmd_id,options,1)

#Listen on the channel for responses from the Nodes within the cluster.

for i  in range(0,2) : 
    recieved_messages = []
    recieved_messages =  xbee_obj.receive_packet()
   # xbee_obj.receive_packet()
    #print all contents in recieved_messages buffer. 
    for recieved_message in recieved_messages:
        print xbee_obj.format_to_string(recieved_message)
        print '----------------------------------------------'

    t.sleep(2)
