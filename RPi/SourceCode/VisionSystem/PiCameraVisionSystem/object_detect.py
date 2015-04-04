import cv2 as cv
import numpy as np
import picamera
import math
import io


#Constants
lower_blue = np.array([85,40,40])
upper_blue = np.array([100,255,255])
#lower_yellow = np.array([20,110,110])
#upper_yellow = np.array([30,255,255])
lower_pink = np.array([0,110,110])
upper_pink = np.array([20,255,255])
res = [640,480]

#NEED TO ADD :
# bg subraction pic needs to update whenever qr changes position
with picamera.PiCamera() as camera:
    camera.resolution = (res)
    camera.capture('bg.jpg')

bg = cv.imread('/home/pi/bg.jpg')
while(1):
    stream = io.BytesIO()
    with picamera.PiCamera() as camera:
        camera.resolution = (res)
        camera.capture(stream, format = 'jpeg')

    data = np.fromstring(stream.getvalue(), dtype=np.uint8)
    image = cv.imdecode(data,1)
    image = image - bg
    hsv = cv.cvtColor(image, cv.COLOR_BGR2HSV) 
 
    kernel = np.ones((5,5),np.float32)/25
    #mask for possible blue values
    blue_mask = cv.inRange(hsv, lower_blue, upper_blue)
    blue_mask = cv.filter2D(blue_mask,-1,kernel)
    ret, blue_mask = cv.threshold(blue_mask,200,255,cv.THRESH_BINARY)
    blue_res = cv.bitwise_and(image,image,mask= blue_mask)
    
    pink_mask = cv.inRange(hsv, lower_pink, upper_pink)
    pink_mask = cv.filter2D(pink_mask,-1,kernel)
    ret, pink_mask = cv.threshold(pink_mask,200,255,cv.THRESH_BINARY) 
    pink_res = cv.bitwise_and(image,image,mask= pink_mask)
    #yellow_mask = cv.inRange(hsv, lower_yellow, upper_yellow)
    #yellow_res = cv.bitwise_and(image,image,mask= yellow_mask)

    
 

#find center of blue object
    blueM = cv.moments(blue_mask)
    try:
        bx = int(blueM['m10']/blueM['m00'])
        by = int(blueM['m01']/blueM['m00'])
    except ZeroDivisionError:
        bx = 0
        by = 0
#find center of pink object
    pinkM = cv.moments(pink_mask)
    try:
        px = int(pinkM['m10']/pinkM['m00'])
        py = int(pinkM['m01']/pinkM['m00'])
    except ZeroDivisionError:
        py = 0
        px = 0

#count the number of pixels for each beacon to determine size
    bpixcount = cv.countNonZero(blue_mask)
    ppixcount = cv.countNonZero(pink_mask)


#Calculate angle camera is facing away from beacon
#for instances where the cam only sees one beacon
    if bpixcount > 2*ppixcount:
        print "The camera sees only the blue beacon. QR needs to turn 90 degrees."
    if ppixcount > 2*bpixcount:
        print "The camera sees only the yellow beacon. QR needs to turn 90 degrees."
#for slightly more precise turning
    if bpixcount > ppixcount+100:
        print "The QR needs to rotate left."
        angle_set = 0
    elif ppixcount > bpixcount+100:
        print "The QR needs to rotate right."
        angle_set = 0
    else:
        print "QR is at the correct angle."
        angle_set = 1

#Calculate distance each object is from camera
#radius = -distance + 75

    bradius = math.sqrt(bpixcount/math.pi)
    pradius = math.sqrt(ppixcount/math.pi)
    D1 = 75 - bradius
    D2 = 75 - pradius

    if angle_set:
        DN = (D1+D2)/2
        if DN > 50:
            print "QR needs to move forward."
        elif DN < 30:
            print "QR needs to move backward."
        else:
            print "QR is at the correst distance."


#These are only for debugging/user interface
#blend the images
    dst = cv.add(blue_res,pink_res)


#draw dot on middle of objects
    bradius = int(bradius)
    pradius = int(pradius)
    cv.circle(dst, (bx,by), bradius, (0,0,255), 3, 8)
    cv.circle(dst, (px,py), pradius, (255,0,0), 3, 8)
    font = cv.FONT_HERSHEY_SIMPLEX
    cv.putText(dst,'BACON_1',(bx+10,by+10), font, 1,(255,255,255),2,cv.LINE_AA) 
    cv.putText(dst,'BACON_2',(px+10,py+10), font, 1,(255,255,255),2,cv.LINE_AA)

#Outputs
#print "The beacons are %s" % Db + " cm apart.
    print "The number of blue pixels is %s" % bpixcount + "."
    print "The number of pink pixels is %s" % ppixcount + "."
    print "The blue beacon is %s" % D1 + " cm away."
    print "The pink beacon is %s" % D2 + " cm away."
    cv.imshow('Captured', dst)
    cv.waitKey(30)



