import Xbee 
import time as t



#Establish a serial object for the xbee connection

device_location = '/dev/ttyAMA0'
baudrate = '9600'
xbee_obj = Xbee.Xbee(device_location,baudrate)


#listen on the Wireless channel for any transmissions. 


while 1: 
    received_dataframes = []
    t.sleep(1) 
    
    recevied_dataframes =xbee.obj.receive_packet()

    if len(recevied_dataframes) > 0 :
        message = 'Received'
        xbee_obj.SendTransmitRequest(message)
        
