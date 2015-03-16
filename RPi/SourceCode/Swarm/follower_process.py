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
cmd_id = 4


#listen on the Wireless channel for any transmissions. 


while 1: 
#    received_dataframes = []
<<<<<<< HEAD
    t.sleep(0.5) 
=======
    t.sleep(0) 
>>>>>>> f9b5d9cff51cc5a40649cf6a5a5698f52947eb27
    
    recevied_dataframes =xbee_obj.receive_packet(verbose)
   # xbee_obj.receive_packet()
    if len(recevied_dataframes) > 0 :
        message = [25,30]
        xbee_obj.SendTransmitRequest(message,destination_address,cmd_id,options,1)
        
