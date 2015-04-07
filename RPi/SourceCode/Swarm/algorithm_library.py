from auxiliary_library import Data_manipulation
import struct
import Xbee
import time as t
import math


class coordinator_descision:
    
    def __init__(self,device_location,baudrate,verbose):
        
        #create a library object for data manipulation. This is needed to convert from int to bytearray, bytearray to int, etc. 
        self.data_lib = Data_manipulation()
        
        baudrate = '9600'
        device_location = '/dev/ttyUSB0'
        self.xbee_obj = Xbee.Xbee(device_location,baudrate)
        if verbose:
            print self.xbee_obj

    def move_swarm(self,verbose): 
    #ONLY CALL THIS FUNCTION WHEN THE SWARM IS INITIALIZED 
        received_messages = self.movement_data_request(verbose)
        
        if len(received_messages) > 0 : 
            self.detect_potential_collisions(received_messages,verbose)
            
        else:
            print 'No quadrotors responded.. Re attempting broadcast..'
            received_messages = self.movement_data_request(verbose)

    def initialize_swarm(self):
        #initate a movmenet data request to get the nodeids and intital positions
        received_messages = self.movement_data_request(0) # this will return all messages received from the local cluster after page.
        self.extract_data(received_messages,1,"nodeid_only")
        
        try:
            self.cluster_nodes = []
            for i in range(0,max(self.nodeid) + 1): #add one to compensate for coordinator offset. 
                self. cluster_nodes.append(0)
    
        
        #assign the data of the coordinator 
            self.cluster_nodes[0] = node_status(0)
            for node in self.nodeid:
            #create an object of the QR in the swarm for memory retention of current location. 
                self.cluster_nodes[node] = (node_status(node))
                
            print "Swarm Cluster Initalized."
            print "Cluster Size: " + (str(len(self.nodeid)+ 1 )) # added 1 to account for the coordinator. 


        except : 
            print self.nodeid
            print "No quadrotors are detected within the vicinity. Re Run initialization sequence."
    
    def movement_data_request(self,verbose):
        #COMMAND IDENTIFIER TYPE:
        #01 - Movement Data Request
        #02 - Obstacle Detection Alert
        #03 - Report to Base Station
        #04 - Report Data
        options = 0x00
        cmd_id = 1
        destination_address = 0xFFFF
        msg = 0
        received_messages =[]
        self.xbee_obj.SendTransmitRequest(msg,destination_address,cmd_id,options,verbose)
        t.sleep(1.15)
        received_messages = self.xbee_obj.receive_packet(verbose)
        #self.detect_potential_collisions(received_messasges,verbose)
        return received_messages

    def detect_potential_collisions(self,rxmessages,verbose):
        
        
        #COMMAND IDENTIFIER TYPE:
        #01 - Movement Data Request
        #02 - Obstacale detection alert
        #03 - Report to base Station
        #04 - Report Data 
        
        
        
        if verbose:
            print " Detecting possible collisions..."
            
        self.extract_data(rxmessages,verbose,"to_cluster_obj")
        
        #Determine Possible collisions. 
       
        #DEBUGGING
        for cluster_node in self.cluster_nodes:
           if cluster_node != 0: 
               print "Cluster Node " + str(cluster_node.nodeid) + " Information"
               print "----------------------------------------------------------"
               print " Distance:"  + str(cluster_node.distance) 
           
               print " Height:"  + str(cluster_node.height)
              
               print " Perpendicularity:"  + str(cluster_node.perpendicularity)
               print " "

               print " Coordinate Representation " + str(cluster_node.coordinate_representation)
           
           elif cluster_node == 0:
               print "Cluster Node is empty"

    def extract_data(self,rxmessages,verbose,extraction_type):
        
    
         if extraction_type == "all_data":

            self.nodeid = []
            self.distance_data = [] 
            self.perpendicularity_data = []
            self.height_data = []
       
            if verbose:
                print "Parsing position data.."
                
            for rxmessage in rxmessages:
                if rxmessage[2] == 4:
                    self.distance_data.append(self.data_lib.bytearray_to_int(rxmessage[13:]))
                    self.perpendicularity_data.append(self.data_lib.bytearray_to_int(rxmessage[8:12]))
                    self.height_data.append(self.data_lib.bytearray_to_int(rxmessage[3:7]))
                    self.nodeid.append(self.data_lib.bytearray_to_int(rxmessage[0:2]))
    
         elif extraction_type == "nodeid_only":
             self.nodeid = [] 
             
             for rxmessage in rxmessages:
                 if rxmessage[2] == 4:
                     self.nodeid.append(self.data_lib.bytearray_to_int(rxmessage[0:2]))

         elif extraction_type == "to_cluster_obj":
            
             for rxmessage in rxmessages:
                 if rxmessage[2] == 4:
                     nodeid = self.data_lib.bytearray_to_int(rxmessage[0:2])
                     height = self.data_lib.bytearray_to_int(rxmessage[3:7])
                     perpendicularity = self.data_lib.bytearray_to_int(rxmessage[7:11])
                     distance = self.data_lib.bytearray_to_int(rxmessage[11:])
                    
                     

                     #update corresponding cluster node object with correct data
                     
                     self.cluster_nodes[nodeid].Update_Location(height,distance,perpendicularity)
         else:
             
             print "error occured."
                     
                     
                     
             
class follower_decision:

    def __init__(self,device_location,baudrate,verbose):
    
        #create an object for the auxiliary library for data manipulation of packets.
        
        self.data_lib = Data_manipulation()
        
        #Create an Xbee Object. 
    
        self.xbee_obj = Xbee.Xbee(device_location,baudrate);
        if verbose:
            print self.xbee_obj

        self.nodeid = self.xbee_obj.obtain_xbee_status(verbose) 

    def listen_to_channel(self):
        
        received_dataframes = []
        while len(received_dataframes) == 0: 
        #listen to the channel until a packet arrives. 
        #Each dataframe will contain the following information. 

        # NODEID   COMMANDID   DATA 
        #<2 Bytes> <1 Byte>    <Variable> 

            received_dataframes = self.xbee_obj.receive_packet(verbose)
            
        
        
        #when packet(s) has arrived, determine if the packet is meant for this node. 
        
        
            
            
            
class node_status:
    
    def __init__(self,nodeid): 
        
        self.nodeid = nodeid
        self.distance = 0
        self.height = 0
        self.perpendicularity = 0 
        self.yaw = 0  
        self.coordinate_representation = [0,0]
        
    def Update_Location(self,height,distance,perpendicularity):
        
        self.perpendicularity = perpendicularity
        self.height = height
        self.distance = distance 
        self.angle  = math.atan2(self.perpendicularity,self.distance)
        hypoten_distance = math.hypot(self.distance,self.perpendicularity)
        self.coordinate_representation = [self.distance,self.perpendicularity]
       # self.coordinate_representation = [(hypoten_distance * math.cos(self.angle),(hypoten_distance * math.sin(self.angle)))];

        
    
        

