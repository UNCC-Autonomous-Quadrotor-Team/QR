class coord_descision:

    

    def detect_potential_collisions(self,verbose):
        if verbose:
            print " Detecting possible collisions..."
            
        return 0

    def extract_data(self,rxmessages,verbose):
        if verbose:
            print "Parsing position data.."
            
        nodeid = [] 
        for rxmessage in rxmessages:
            self.nodeid.append(rxmessage[0:2])
        return nodeid
