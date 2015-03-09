import Xbee 
import time as t



#Establish a serial object for the xbee connection

device_location = '/dev/ttyAMA0'
baudrate = '9600'
xbee_obj = Xbee.Xbee(device_location,baudrate)
verbose = 1 

#Parameters for packet transmission

destination_address = 0x0000 #coordinator destination <16 bit address>
options = 0x00 #no options specified
cmd_id = 2


#listen on the Wireless channel for any transmissions. 


while 1: 
#    received_dataframes = []
    t.sleep(1) 
    
    recevied_dataframes =xbee_obj.receive_packet(verbose)
   # xbee_obj.receive_packet()
    if len(recevied_dataframes) > 0 :
        message = 25
        xbee_obj.SendTransmitRequest(message,destination_address,cmd_id,options,1)
        
