import time as t
import Xbee
import algorithm_library as swarm_alg
import profile 
#Create a object from the coordination_decision class from the swarm algorithm library so that its functions can be used 
verbose = 1
device_location = '/dev/ttyUSB0'
baudrate = '9600'

coord_descision_library = swarm_alg.coordinator_descision(device_location,baudrate,verbose)


#Initialize the cluster.
profile.run('coord_descision_library.initialize_swarm()')

#proceed to move the cluster.

coord_descision_library.move_swarm(verbose)
#coord_descision_library.move_swarm(verbose)
