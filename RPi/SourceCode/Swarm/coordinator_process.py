import time as t
import Xbee
import algorithm_library as swarm_alg

#Create a object from the coordination_decision class from the swarm algorithm library so that its functions can be used 
verbose = 1
coord_descision_library = swarm_alg.coordinator_descision(verbose)


#Start the process to move the swarm. 

coord_descision_library.move_swarm(verbose)
