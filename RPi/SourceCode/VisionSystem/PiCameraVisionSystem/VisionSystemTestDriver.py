import VisionSystem




resolution = 'low_res'
contrast = 100
exposure_mode = 'off'
awb_mode = 'auto'
vflip = True
iso = 800


ir_vision_sys = VisionSystem.IRVisionSys(resolution,contrast,exposure_mode,awb_mode,vflip,iso)


ir_vision_sys.obtain_beacon_location()
