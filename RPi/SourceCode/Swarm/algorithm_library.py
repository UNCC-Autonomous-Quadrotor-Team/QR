from auxiliary_library import Data_manipulation
import struct




class coord_descision:
    
    def __init__(self):
        
        #create a library object for data manipulation. This is needed to convert from int to bytearray, bytearray to int, etc. 
        self.data_lib = Data_manipulation()
    

    def detect_potential_collisions(self,verbose):
        if verbose:
            print " Detecting possible collisions..."
            
        return 0

    def extract_data(self,rxmessages,verbose):
        if verbose:
            print "Parsing position data.."
            
        nodeid = [] 
        position_data = []
        for rxmessage in rxmessages:
            nodeid.append(self.data_lib.bytearray_to_int(rxmessage[0:2]))
            position_data.append(self.data_lib.bytearray_to_int(rxmessage[3:]))

        return nodeid
