import Xbee 
import time as t
from algorithm_library import follower_decision


#Establish a serial object for the xbee connection

device_location = '/dev/ttyAMA0'
baudrate = 9600
#xbee_obj = Xbee.Xbee(device_location,baudrate)
verbose =1 
follower_proc = follower_decision(device_location,baudrate,verbose)

while 1:
    follower_proc.listen_to_channel(verbose)

#Parameters for packet transmission

destination_address = 0x0000 #coordinator destination <16 bit address>
options = 0x00 #no options specified
cmd_id = 4


#listen on the Wireless channel for any transmissions. 


#while 1: 
 #   received_dataframes = []
  #  t.sleep(0.5) 
 

    
   # recevied_dataframes = xbee_obj.receive_packet(verbose)
   # if len(recevied_dataframes) > 0 :
      #  message : [height,perpendicularity,distance]
      #  message = [25,30,35]
      #  xbee_obj.SendTransmitRequest(message,destination_address,cmd_id,options,1)
        
