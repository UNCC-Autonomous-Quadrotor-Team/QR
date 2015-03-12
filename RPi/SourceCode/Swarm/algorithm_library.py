from auxiliary_library import Data_manipulation
import struct




class coord_descision:
    
    def __init__(self):
        
        #create a library object for data manipulation. This is needed to convert from int to bytearray, bytearray to int, etc. 
        self.data_lib = Data_manipulation()
    

    def detect_potential_collisions(self,verbose):
        self.nodeid = []
        self.distance_data = []
        self.orientation_data = []
        
        if verbose:
            print " Detecting possible collisions..."
            
        return 0

    def extract_data(self,rxmessages,verbose):
        if verbose:
            print "Parsing position data.."
            
        for rxmessage in rxmessages:
            if rxmessage[2] == 4:
                distance_data.append(self.data_lib.bytearray_to_int(rxmessage[3:7]))
                orientation_data.append(self.data_lib.bytearray_to_int(rxmessage[7:]))
                
            nodeid.append(self.data_lib.bytearray_to_int(rxmessage[0:2]))
            

        return orientation_data
