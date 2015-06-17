from PiCameraVisionSystem import VisionSystem
resolution = 'tiny_res'                                                                                       
contrast = 100                                                                                               
exposure_mode = 'off'                                                                                       
awb_mode = 'auto'            
vflip = True                                                                                                
iso = 800                                                                                                    
verbose = 1
 
vision_system = VisionSystem.IRVisionSys(resolution,contrast,exposure_mode,awb_mode,vflip,iso)           
if verbose:             
    print vision_system

vision_system.obtain_beacon_location()
