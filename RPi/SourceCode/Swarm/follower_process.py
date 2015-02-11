import Xbee 
import time as t



#Establish a serial object for the xbee connection

device_location = '/dev/ttyAMA0'
baudrate = '9600'
xbee_obj = Xbee.Xbee(device_location,baudrate)


#Parameters for packet transmission

destination_address = 0x00 #coordinator destination <16 bit address>
options = 0x00 #no options specified
frameid = 0x00 # frame id of 0 is used


#listen on the Wireless channel for any transmissions. 


while 1: 
    received_dataframes = []
    t.sleep(1) 
    
    recevied_dataframes =xbee_obj.receive_packet()

    if len(recevied_dataframes) > 0 :
        message = 'Received'
        xbee_obj.SendTransmitRequest(message,destination_address,frameid,options,1)
        
