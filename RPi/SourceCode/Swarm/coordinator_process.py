import time as t
import Xbee
import algorithm_library as swarm_alg

#open Xbee object


device_location ='/dev/ttyUSB0'


baudrate = '9600'
xbee_obj = Xbee.Xbee(device_location,baudrate)
coord_descision = swarm_alg.coord_descision()

#Broadcast a request to the immediate cluster for data.
# COMMAND IDENTIFIER TYPE:
# 01 - Position Data Request
# 02 - Collision detection alert
# 03 - Command Issue. 
# 04 - Report Data
#msg = 'Request Acknowledgement'
options = 0x00
verbose = 1 


#Send Transmit Request for position data to immediate token ring.
xbee_obj.SendTransmitRequest(25,0xFFFF,1,options,verbose)

#Listen on the channel for responses from the Nodes within the cluster.

t.sleep(2) 
received_messages = []
received_messages =  xbee_obj.receive_packet(verbose) # List of Bytearrays
print len(received_messages)
if len(received_messages) > 0 :
    position_data = coord_descision.extract_data(received_messages,verbose)
    for data in position_data :
        
        print data
        print type(data)
# print type(nodeids)
#  print type(nodeids(0))

    #print all contents in recieved_messages buffer. 
   # for recieved_message in recieved_messages:
    #    print xbee_obj.format_to_string(recieved_message)
     #   print '----------------------------------------------'

   # t.sleep(2)
