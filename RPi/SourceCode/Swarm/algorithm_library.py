from auxiliary_library import Data_manipulation
import struct
import Xbee
import time as t



class coordinator_descision:
    
    def __init__(self,verbose):
        
        #create a library object for data manipulation. This is needed to convert from int to bytearray, bytearray to int, etc. 
        self.data_lib = Data_manipulation()
        
        baudrate = '9600'
        device_location = '/dev/ttyUSB0'
        self.xbee_obj = Xbee.Xbee(device_location,baudrate)
        if verbose:
            print self.xbee_obj

    def move_swarm(self,verbose):
            
        received_messages = self.movement_data_request()
        
        if len(received_messages) > 0 : 
            self.detect_potential_collisions(received_messages,verbose)
            
        else:
            print 'No quadrotors responded.. Re attempting broadcast..'
            received_messages = self.movement_data_request()
    def movement_data_request(self):
        options = 0x00
        verbose = 1
        cmd_id = 1
        destination_address = 0xFFFF
        msg = 0
        received_messages =[]
        self.xbee_obj.SendTransmitRequest(msg,destination_address,cmd_id,options,verbose)
        t.sleep(0.7)
        received_messages = self.xbee_obj.receive_packet(verbose)
        return received_messages


    def detect_potential_collisions(self,rxmessages,verbose):
        self.nodeid = []
        self.distance_data = []
        self.orientation_data = []
        
        #COMMAND IDENTIFIER TYPE:
        #01 - Movement Data Request
        #02 - Obstacale detection alert
        #03 - Report to base Station
        #04 - Report Data 
        
        
        
        if verbose:
            print " Detecting possible collisions..."
            
        self.extract_data(rxmessages,verbose)
        if verbose: # Debug only 

            print self.distance_data[0]
            print self.orientation_data[0]

        #Determine Possible collisions. 
        

    def extract_data(self,rxmessages,verbose):
        if verbose:
            print "Parsing position data.."
            
        for rxmessage in rxmessages:
            if rxmessage[2] == 4:
                self.distance_data.append(self.data_lib.bytearray_to_int(rxmessage[3:7]))
                self.orientation_data.append(self.data_lib.bytearray_to_int(rxmessage[7:]))
                
                self.nodeid.append(self.data_lib.bytearray_to_int(rxmessage[0:2]))
            

        
