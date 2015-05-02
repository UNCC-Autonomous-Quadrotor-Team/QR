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

        print "Getting position of all followers in cluster..."
        received_messages = self.position_data_request(verbose)
        
        if len(received_messages) > 0 : 
            self.detect_potential_collisions(received_messages,verbose)
            
        else:
            print 'No quadrotors responded.. Re attempting broadcast..'
            self.move_swarm(verbose)
        
        #BEGIN THE PROCESS FOR MOVING THE SWARM
        
        #get the position of the coordinator in relation to the object of interest. 
        print "moving swarm..."
        height = -10
        distance = -10
        perpendicularity = -10 
        self.cluster_nodes[0].Update_Location(height,distance,perpendicularity)
        #******************Obtain neccessary vector movements.***********#
        #TODO
        movement_vector  = [height,distance,perpendicularity]
        self.send_movement_command(0xFFFF,movement_vector,verbose)
        
        

    def initialize_swarm(self):
        #initate a movmenet data request to get the nodeids and intital positions
        received_messages = self.position_data_request(1) # this will return all messages received from the local cluster after page.
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
            self.initialize_swarm()
    
    def position_data_request(self,verbose): # Request Position Data from followers
        #COMMAND IDENTIFIER TYPE:
        #01 - Position Data Request
        #02 - Obstacle Detection Alert
        #03 - Report to Base Station
        #04 - Report Data
        #05 - Movement Command
        #06 - Report Acknowledgement
        options = 0x00
        cmd_id = 1
        destination_address = 0xFFFF
        msg = 0
        received_messages =[]
        self.xbee_obj.SendTransmitRequest(msg,destination_address,cmd_id,options,verbose)
        t.sleep(0.3)
        received_messages = self.xbee_obj.receive_packet(verbose)
        #self.detect_potential_collisions(received_messasges,verbose)
        return received_messages

    def detect_potential_collisions(self,rxmessages,verbose):
        
        
        #COMMAND IDENTIFIER TYPE:
        #01 - Movement Data Request
        #02 - Obstacale detection alert
        #03 - Report to base Station
        #04 - Report Position  Data 
        #05 - Movement Command 
        #06 - Report Acknowledgement
        
        
        if verbose:
            print " Detecting possible collisions..."
            
        self.extract_data(rxmessages,verbose,"to_cluster_obj")
        
        #Determine Possible collisions. 
       
        #DEBUGGING
        if verbose:
            for cluster_node in self.cluster_nodes:
                if cluster_node != 0: 
                    print "Cluster Node " + str(cluster_node.nodeid) + " Information"
                    print "---------------------------------------------------------"
                    print " Distance:"  + str(cluster_node.distance) 
                    
                    print " Height:"  + str(cluster_node.height)
                    
                    print " Perpendicularity:"  + str(cluster_node.perpendicularity)
                    print " "
                    
                    print " Coordinate Representation " + str(cluster_node.coordinate_representation)
           
                elif cluster_node == 0:
                    print "Cluster Node is empty"


            for cluster_node in self.cluster_nodes:
            # Detection algorithm
                if cluster_node != 0: #make sure the cluster node object is available.  
                    if cluster_node.nodeid != 0: #make sure the node isn't the coordinator.      
                        message = [25,67,100]
                        self.send_movement_command(cluster_node.nodeid,message,verbose)
                        print cluster_node
                        #listen for acknowledgements from the followers
                        t.sleep(0.3)
                        rxmessages = self.xbee_obj.receive_packet(verbose)
                        ACK = None
                        ACK = self.extract_data(rxmessages,verbose,"LookForACK")
                        
                        
                        if ACK != cluster_node.nodeid: #Re transmit if ACK is not received. 
                            if verbose: 
                                print "Acknowledgement not received from node " +  str(cluster_node.nodeid) + " Re Transmitting Movement Command.."
                            self.send_movement_command(cluster_node.nodeid,message,verbose)
                            t.sleep(0.3)
                            rxmessages = self.xbee_obj.receive_packet(verbose)
                            ACK = self.extract_data(rxmessages,verbose,"LookForACK")
                            
                            if ACK!= cluster_node.nodeid: # Re transmit for a second time.
                                if verbose:
                                    print "Acknowledgment still not received from node " +  str(cluster_node.nodeid) + " Re Transmitting Movement Commmand.."
                                self.send_movement_command(cluster_node.nodeid,message,verbose)
                                t.sleep(0.3)
                                rxmessages = self.xbee_obj.receive_packet(verbose)
                                ACK = self.extract_data(rxmessages,verbose,"LookForACK")

                                if ACK != cluster_node.nodeid: # Still no acknowledgement received. The link is considered broken.
                                    print "Unsuccessful in completing handshake with node " + str(cluster_node.nodeid)
                                else: 
                                    print "ACK received for Node : " + str(cluster_node.nodeid)
                            else:
                                print "ACK received for Node : " + str(cluster_node.nodeid)
                    
                        else:
                            print "ACK received from Node " + str(cluster_node.nodeid)
                        

    def send_movement_command(self,nodeid,message,verbose):
        #message should be a list of integers or acknowledgement string
        destination_address = nodeid #address of the desired nodeid. 
        options = 0x00 
        cmd_id = 5
        self.xbee_obj.SendTransmitRequest(message,destination_address,cmd_id,options,verbose)
        
    
        
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
                 if rxmessage[2] == 4: # Determine if the received message is of type "Report Data"
                     nodeid = self.data_lib.bytearray_to_int(rxmessage[0:2])
                     height = self.data_lib.bytearray_to_int(rxmessage[3:7])
                     perpendicularity = self.data_lib.bytearray_to_int(rxmessage[7:11])
                     distance = self.data_lib.bytearray_to_int(rxmessage[11:])
                    
                     

                     #update corresponding cluster node object with correct data
                     
                     self.cluster_nodes[nodeid].Update_Location(height,distance,perpendicularity)

         elif extraction_type == "LookForACK":
             for rxmessage in rxmessages:
                 if rxmessage[2] == 6: 
                    # print "Acknowledgement Received for:" + self.data_lib.bytearray_to_string(rxmessage[0:2])
                     return self.data_lib.bytearray_to_int(rxmessage[0:2])
            
         else:
             
             print "error occured."
                     
                     
    
            
class follower_decision:

    def __init__(self,device_location,baudrate,verbose):
    
        #create an object for the auxiliary library for data manipulation of packets.
        
        self.data_lib = Data_manipulation()
        
        #Create an Xbee Object. 
    
        self.xbee_obj = Xbee.Xbee(device_location,baudrate)
        if verbose:
            print self.xbee_obj

        t.sleep(0.5)
        self.nodeid = self.xbee_obj.obtain_xbee_nodeid(verbose) 

    def listen_to_channel(self,verbose):
        
        received_dataframes = []
        while len(received_dataframes) == 0: 
        #listen to the channel until a packet arrives. 
        #Each dataframe will contain the following information. 

        # NODEID   COMMANDID   DATA 
        #<2 Bytes> <1 Byte>    <Variable> 
            t.sleep(0.05)
            received_dataframes = self.xbee_obj.receive_packet(verbose)
            #t.sleep(1)
            #t.sleep(0.5)
        #determine the type of message transmitted. Messages should only come from the Coordinator. A failsafe will be implemented just in case a non coordinator device transmitted to a follower. 
        
        received_messages = self.process_data(received_dataframes)
        
        print "received message:" 
        print type(received_messages)
        print received_messages
        self.reply_to_message(received_messages,verbose) 
            

    def reply_to_message(self,messages,verbose):
 
        for message in messages:
            if message == None:
                if verbose:
                    print "Message Type: Position Data Request"
                position = [25,30,45]
                coordinator_address = 0x0000
                cmd_id = 4
                self.xbee_obj.SendTransmitRequest(position,coordinator_address,cmd_id,0x00,1)
            
            elif len(message) > 1 :
                if verbose:
                    print "Message Type: Movement Command."
                cmd_id = 6
                message = 'ACK'
                coordinator_address = 0x0000
                self.xbee_obj.SendTransmitRequest(message,coordinator_address,cmd_id,0x00,1)

            else: 
                if verbose:
                    print "Message Type: Unrecognized message." 

        

    def process_data(self,received_dataframes):
       
        data = []
        #Parse the dataframe into the source node id, command id, and data.

        #INPUT: list of bytearrays. 
        #OUTPUT: list of integers from the coordinator.
        #counter = 0 
        for received_dataframe in received_dataframes:
            nodeid = -1 
            nodeid = self.data_lib.bytearray_to_int(received_dataframe[0:1])
            
            if nodeid == 0 : #See if the received dataframe was transmitted by the coordinator. 
                if received_dataframe[2] == 1: # Determine if the received dataframe contains a message that is a transmit data request
                    data.append(None) # Send a "NULL" value to data. Since it is a data request. 
                    
                    

                elif received_dataframe[2] == 5: # Determine if the received dataframe contains a message that is a movement command. 
                    data.append([self.data_lib.bytearray_to_int(received_dataframe[3:7]),self.data_lib.bytearray_to_int(received_dataframe[7:11]),self.data_lib.bytearray_to_int(received_dataframe[11:])])  # Since this is a movement command, collect the data from the received_dataframe. FORMAT : [height,perpendicularity,distance]
                    #Follow Movement Command#
                   
                    #Send Acknowledgement#
                    
                else:
                    data.append(-1)# The command ID was an error. 
        #    counter = counter + 1
        return data



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
        self.coordinate_representation[0] += self.distance 
        self.coordinate_representation[1] += self.perpendicularity
       # self.coordinate_representation = [(hypoten_distance * math.cos(self.angle),(hypoten_distance * math.sin(self.angle)))];

        
    
        

