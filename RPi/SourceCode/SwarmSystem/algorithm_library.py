from auxiliary_library import Data_manipulation
from PiCameraVisionSystem import VisionSystem 
import struct
import Xbee
import time as t
import math


class coordinator_descision:
    
    def __init__(self,device_location,baudrate,verbose):
        
        #create a library object for data manipulation. This is needed to convert from int to bytearray, bytearray to int, etc. 
        self.data_lib = Data_manipulation()
        
        baudrate = '9600'
#        device_location = '/dev/ttyUSB0'
        self.xbee_obj = Xbee.Xbee(device_location,baudrate)


        #Create object to interface to vision system.
        resolution = 'tiny_res'
        contrast = 100
        exposure_mode = 'off'
        awb_mode = 'auto'
        vflip = True
        iso = 800 
        
        self.vision_system = VisionSystem.IRVisionSys(resolution,contrast,exposure_mode,awb_mode,vflip,iso)
        if verbose:
            print self.xbee_obj

    def move_swarm(self,verbose): 

    #ONLY CALL THIS FUNCTION WHEN THE SWARM IS INITIALIZED 

#        print "Getting position of all followers in cluster..."
 #       received_messages = self.position_data_request(0xFFFF,verbose)
        
  #      if len(received_messages) > 0 : 
        self.detect_potential_collisions(verbose)
            
      #  else:
       #     print 'No quadrotors responded.. Re attempting broadcast..'
        #    self.move_swarm(verbose)
        
        #BEGIN THE PROCESS FOR MOVING THE SWARM
        
        #get the position of the coordinator in relation to the object of interest. 
        print "moving swarm..."
        height = 10
        distance = 10
        perpendicularity = -10
        angular_offset = -20
        self.cluster_nodes[0].Update_Location(height,perpendicularity,distance,angular_offset)
        #******************Obtain neccessary vector movements.***********#
        #TODO
        movement_vector  = [height,perpendicularity,distance,angular_offset]
        self.send_movement_command(0xFFFF,movement_vector,verbose)
        
        #Receive Acknowledgements from the followers.
#        t.sleep(0.3)
        rxmessages = self.xbee_obj.receive_packet(verbose)
        received_ack_frames = []
        received_ack_frames = self.extract_data(rxmessages,verbose,"LookForACK")
        if verbose: 
            print "Acknowledged Nodes:" + str(received_ack_frames) 
        
        if received_ack_frames  == []: # No nodes responded.
            if verbose:
                print "No nodes responded. Retransmitting movement vector."
            self.send_movement_command(0xFFFF,movement_vector,verbose)
            #t.sleep(0.3)
            rxmessages = self.xbee_obj.receive_packet(verbose)
            received_ack_frames = self.extract_data(rxmessages,verbose,"LookForACK")
            
                
        for cluster_node in self.cluster_nodes:
            if cluster_node != None: # See if the  Cluster node exist.
                if cluster_node.nodeid in received_ack_frames: # Acknowledge was received. 
                    if verbose:
                        print "Acknowledgement received for node " + str(cluster_node.nodeid)
                    else:# Acknowledgement not received
                        if verbose:
                            print "Acknowledgement not recieved for node" + str(cluster_node.nodeid)
                            print "Re Transmitting movement Vector"
                            self.send_movement_command(cluster_node.nodeid,movement_vector,verbose)
                            #t.sleep(0.3)
                            rxmessages = self.xbee_obj.receive_packet(verbose)
                            if cluster_node.nodeid in self.extract_data(rxmessages,verbose,"LookForACK"):
                                if verbose:
                                    print "Acknowledgement received for node" + str(cluster_node.nodeid)
                            else:
                                if verbose: 
                                    print "Acknowledgement still not received from node" + str(cluster_node.nodeid)
                                    self.send_movement_command(cluster_node.nodeid,movement_vector,verbose)
                                    t.sleep(0.3)
                                    rxmessages = self.xbee_obj.receive_packet(verbose)
                                    if cluster_node.nodeid in self.extract_data(rxmesssages,verbose,"LookForACK"):
                                        if verbose:
                                            print "Acknowledgment received for node" + str(cluster_node.nodeid)
                                    else:
                                        if verbose:
                                            print "Failed to establish communication to node" + str(cluster_node.nodeid) + " The communication link may be destroyed."


                            
    def initialize_swarm(self):
        #INFO:
        # cluster_nodes <= list of objects of type node_status. This contains the position and status of each node.
        # nodeid <= list of nodeids of all participating quadcopters in the cluster.
        #initate a movmenet data request to get the nodeids and intital positions
        received_messages = self.position_data_request(0xFFFF,1) # this will return all messages received from the local cluster after page.
        self.extract_data(received_messages,1,"nodeid_only")
        
        try:
            self.cluster_nodes = []
            for i in range(0,max(self.nodeid) + 1): #add one to compensate for coordinator offset. 
                self.cluster_nodes.append(None)
    
        
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
    
    def position_data_request(self,destination_address,verbose): # Request Position Data from followers
        #COMMAND IDENTIFIER TYPE:
        #01 - Position Data Request
        #02 - Obstacle Detection Alert
        #03 - Request to join the swarm
        #04 - Report Data
        #05 - Movement Command
        #06 - Report Acknowledgement
        options = 0x00
        cmd_id = 1
        msg = 0
        received_messages =[]
        self.xbee_obj.SendTransmitRequest(msg,destination_address,cmd_id,options,verbose)
        #t.sleep(0.3)
        received_messages = self.xbee_obj.receive_packet(verbose)
        #self.detect_potential_collisions(received_messasges,verbose)
        return received_messages

    def detect_potential_collisions(self,verbose):
        
        
        #COMMAND IDENTIFIER TYPE:
        #01 - Movement Data Request
        #02 - Obstacale detection alert
        #03 - Report to base Station
        #04 - Report Position  Data 
        #05 - Movement Command 
        #06 - Report Acknowledgement
        
        
        if verbose:
            print " Starting Fine-Grain Algorithm and Detecting possible collisions..."
            
     #   self.extract_data(rxmessages,verbose,"to_cluster_obj")
        
        #Determine Possible collisions. 
       
        #DEBUGGING
       # if verbose:
        #    for cluster_node in self.cluster_nodes:
         #       if cluster_node != None: # If the Cluster Node exists in the swarm
          #          print "Cluster Node " + str(cluster_node.nodeid) + " Information"
           #         print "---------------------------------------------------------"
            #        print " Distance:"  + str(cluster_node.distance) 
                    
            #        print " Height:"  + str(cluster_node.height)
                    
            #        print " Perpendicularity:"  + str(cluster_node.perpendicularity)
            #        print " "
            #        print " Angular Displacement:" + str(cluster_node.angular_offset)
            #        print " "
            #        print " Coordinate Representation " + str(cluster_node.coordinate_representation)
                    
                    
           
             #   elif cluster_node == 0:
              #      print "Cluster Node is empty"

            starttime = t.time()
            for cluster_node in self.cluster_nodes:
            # Detection algorithm
                if cluster_node != None: #make sure the cluster node object is available.  
                    if cluster_node.nodeid != 0: #make sure the node isn't the coordinator.
                        #request position Data
                        received_message = self.position_data_request(cluster_node.nodeid,verbose)
                        self.extract_data(received_message,verbose,"to_cluster_obj")
                       # t.sleep(0.2)
                        # store the data into the respective cluster object. 
                        
                        #print len(received_message)
                       # Print position of the node
                        if verbose:
                             print "Cluster Node " + str(cluster_node.nodeid) + " Information"
                             print "---------------------------------------------------------"
                             print " Distance:"  + str(cluster_node.distance)

                             print " Height:"  + str(cluster_node.height)

                             print " Perpendicularity:"  + str(cluster_node.perpendicularity)
                             print " "
                             print " Angular Displacement:" + str(cluster_node.angular_offset)
                             print " "
                             print " Coordinate Representation " + str(cluster_node.coordinate_representation)

                        message = [25,67,100,50]
                        
                        self.send_movement_command(cluster_node.nodeid,message,verbose)
                        print cluster_node
                        #listen for acknowledgements from the followers
                        #t.sleep(0.3)
                        rxmessages = self.xbee_obj.receive_packet(verbose)
                        
                        print len(rxmessages)
                        ACK = None
                        ACK = self.extract_data(rxmessages,verbose,"LookForACK") # returns a list of ACknowledged nodes.
                                                                     
                        if cluster_node.nodeid not in ACK: #Re transmit if ACK is not received. 
                            if verbose: 
                                print "Acknowledgement not received from node " +  str(cluster_node.nodeid) + " Re Transmitting Movement Command.."
                            self.send_movement_command(cluster_node.nodeid,message,verbose)
                            #t.sleep(0.3)
                            rxmessages = self.xbee_obj.receive_packet(verbose)
                            ACK = self.extract_data(rxmessages,verbose,"LookForACK") #returns a list of acknowledged nodes
                            
                            if cluster_node.nodeid not in ACK: # Re transmit for a second time.
                                if verbose:
                                    print "Acknowledgment still not received from node " +  str(cluster_node.nodeid) + " Re Transmitting Movement Commmand.."
                                self.send_movement_command(cluster_node.nodeid,message,verbose)
                             #   t.sleep(0.3)
                                rxmessages = self.xbee_obj.receive_packet(verbose)
                                ACK = self.extract_data(rxmessages,verbose,"LookForACK")
                                if cluster_node.nodeid not in ACK: # Still no acknowledgement received. The link is considered broken.
                                    print "Unsuccessful in completing handshake with node " + str(cluster_node.nodeid)
                                else: 
                                    print "ACK received for Node : " + str(cluster_node.nodeid)
                            else:
                                print "ACK received for Node : " + str(cluster_node.nodeid)
                    
                        else:
                            print "ACK received from Node " + str(cluster_node.nodeid)
            print "Elapsed Time for Fine-Grained Implementation:" + str(t.time() - starttime)

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
                     distance = self.data_lib.bytearray_to_int(rxmessage[12:15])
                     angular_offset = self.data_lib.bytearray_to_int(rxmessage[16:])

                     

                     #update corresponding cluster node object with correct data
                     
                     self.cluster_nodes[nodeid].Update_Location(height,distance,perpendicularity,angular_offset)
                     
         elif extraction_type == "LookForACK":
           
             #rxmessages should be a list of bytearrays or a bytearray.
           
             #OUTPUT: The format of the output follows the format of the input. EX. An input of a list of bytearrays will yeild a output of a list of integers(that represent the node id of the successful ACK transmission). An input of a single bytearray  will yeild a single integer representing the node id of the node that sent a succesful ACK.  

             
             if type(rxmessages) == list: 
                 #note that this is not a two dimensional array. This is a list of bytearrays. The first argument below specifies the index of the list rxmessages. The second argument specifies the index within the bytearray that is stored in said list index.

                  #FORMAT: rxmessages[list_index][index of bytearray]  ex. rxmessages = [bytearray1,bytearray2,bytearray3]
                 # bytearray[1] = bytearray(b' x00/x02) . Therefore rxmessage[0][1]  == x02 
                 

                 ACKS = [] # list of integers
                     
                 for rxmessage in rxmessages:
                     if rxmessage[2] == 6:
                         ACKS.append(self.data_lib.bytearray_to_int(rxmessage[0:2]))
                         
                 return ACKS 
                                         
                                         
             elif type(rxmessages) == bytearray:
                 
                 if(rxmessages[2] == 6):
                     return self.data_lib.bytearray_to_int(rxmessages[0:2])
                 
                 else:
                     return None 
             
             else:
                 
                 return None 
             
                     
                     
                     
         
         else:
             
             print "Incorrect extraction Type Chosen"
             return None
                     
    
            
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
        #    t.sleep(0.05)
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
                position = [25,30,45,52]
                coordinator_address = 0x0000
                cmd_id = 4
                self.xbee_obj.SendTransmitRequest(position,coordinator_address,cmd_id,0x00,1)
            
            elif len(message) > 1 :
                if verbose:
                    print "Message Type: Movement Command."
                cmd_id = 6
                message = [0,0,0,0]
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
                    height = self.data_lib.bytearray_to_int(received_dataframe[3:7])
                    perpendicularity = self.data_lib.bytearray_to_int(received_dataframe[7:11])
                    distance = self.data_lib.bytearray_to_int(received_dataframe[11:15])
                    angular_offset = self.data_lib.bytearray_to_int(received_dataframe[15:])
                    
                    data.append([height,perpendicularity,distance,angular_offset])  # Since this is a movement command, collect the data from the received_dataframe. FORMAT : [height,perpendicularity,distance]
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
        self.angular_offset = 0  
        self.coordinate_representation = [0,0,0]
        
    def Update_Location(self,height,perpendicularity,distance,angular_offset):
        
        self.perpendicularity = perpendicularity
        self.height = height
        self.distance = distance 
        self.angle  = math.atan2(self.perpendicularity,self.distance)
        self.angular_offset = angular_offset
        hypoten_distance = math.hypot(self.distance,self.perpendicularity)
        self.coordinate_representation[0] += self.distance 
        self.coordinate_representation[1] += self.perpendicularity
        self.coordinate_representation[2]  = self.height 
       # self.coordinate_representation = [(hypoten_distance * math.cos(self.angle),(hypoten_distance * math.sin(self.angle)))];

        
    
        

