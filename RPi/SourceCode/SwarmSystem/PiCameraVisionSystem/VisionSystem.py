 
import numpy as np
import cv2 as cv
import operator
import io
import picamera
import picamera.array
import time
import math





class LED:
    ledCount = 0
    def __init__(self,name,loc,vis,group):
        self.name = name
        self.loc = loc
        self.vis = vis
        self.group = group
        LED.ledCount += 1

    def displayCount(self):
        print "Total LEDs: %d" % LED.ledCount

    def displayLED(self):
        print "Name: ", self.name, ", Location: ", self.loc, "Visibility: ", self.vis


class IRVisionSys:


    def __init__(self,resolution,contrast,exposure_mode,awb_mode,vflip,iso):
        self.camera = picamera.PiCamera()
        
        if resolution == 'tiny_res':
            self.res = [200,150]
        elif resolution == 'low_res':
            self.res = [400,300]
        elif resolution == 'mid_res':
            self.res = [1280,780]
        elif resolution == 'high_res':
            self.res = [1920,1080]
        elif resolution == 'ultra_res':
            self.res = [292,1944]
        else:
            self.res = [400,300]
            
        self.camera.resolution = (self.res)
        self.camera.contrast = contrast
        if (exposure_mode != 'off') or (exposure_mode != 'on'):
            #default the exposure to off.
            exposure_mode = 'off'

        self.camera.exposure_mode = exposure_mode 
        self.camera.awb_mode = awb_mode
        self.camera.vflip = vflip
        self.camera.iso = iso 
        
        self.stream = picamera.array.PiRGBArray(self.camera)
        



        
    def contourSearch(self,im):
        imgray = cv.cvtColor(im,cv.COLOR_BGR2GRAY)
        ret,thresh = cv.threshold(imgray,5,255,0)
        dilation = np.ones((5,5), "uint8")
        imgray = cv.dilate(imgray,dilation)
        contours,_ = cv.findContours(thresh,cv.RETR_EXTERNAL,cv.CHAIN_APPROX_SIMPLE)
        return(contours,imgray)

    def findLeds(self,contours):
        cx = [0]*len(contours)
        cy = [0]*len(contours)
        M = [0]*len(contours)
        ledDict={'0': 0,}
        contourlist = [0]*len(contours)
        for c in range (0,len(contours)):
            M[c] = cv.moments(contours[c]) 
            try:
                cx[c] = int(M[c]['m10']/M[c]['m00'])
                cy[c] = int(M[c]['m01']/M[c]['m00'])
            except ZeroDivisionError:
                contourlist[c] = contours[c].tolist()
                cx[c] = contourlist[c][0][0][0]
                cy[c] = contourlist[c][0][0][1]
        for j in range (0,len(contours)):
            while cx[j] in ledDict:
                cx[j] += 1
            ledDict[cx[j]] = cy[j]
        del ledDict['0']

        return(ledDict)

    def sortLeds(self,ledDict,contours):
        sortedLeds = sorted(ledDict.items(),key=operator.itemgetter(0))
        ledList = [0]*len(contours)
        LED.ledCount = 0
        for b in range (0,len(ledList)):
            ledList[b] = LED(str(b),sortedLeds[b],len(contours[b]),'none')
        return(ledList)

    def ledGroup(self,ledList):
        leftCount = 0
        rightCount = 0
        leftList = []
        rightList = []
        ledSpacing = [0]*(len(ledList)-1)
        leftY,rightY,leftMean,rightMean,innerLeft,innerRight = 0,0,0,0,0,0
        oldDif = 0
        dif = 0
        for x in range (1,len(ledList)):  
            dif = ledList[x].loc[0] - ledList[x-1].loc[0]
            ledSpacing[x-1] = dif
            if dif > oldDif:
                innerRight = ledList[x].loc[0]
                innerLeft = ledList[x-1].loc[0]
                oldDif = dif
        for x in range (0,len(ledList)-1):
            if ledList[x].loc[0] < (innerRight + innerLeft)/2:
                leftList.append(ledList[x].loc[0])
                leftY = ledList[x].loc[1]
                ledList[x].group = 'left'
                leftCount += 1
            else:
                rightList.append(ledList[x].loc[0])
                rightY = ledList[x].loc[1]
                ledList[x].group = 'right'
                rightCount+=1
        try:        
            leftMean = np.convolve(np.array(leftList),np.ones((leftCount,))/leftCount)[(leftCount-1):][0]
        except ValueError:
            leftMean = 0
        try:
            rightMean = np.convolve(np.array(rightList),np.ones((rightCount,))/rightCount)[(rightCount-1):][0]
        except ValueError:
            rightMean = 0
        beacInfo = [leftMean,leftY,rightMean,rightY,leftCount,rightCount,ledSpacing]
    
        return(beacInfo)


    def calcPosition(self,beacInfo):
        b_len = 7.5
        mid = self.res[0]/2
        b_left = beacInfo[0]
        b_right = beacInfo[2]
        b_avg = (b_left+b_right)/2
        pixratio = self.res[0]/54
        ledSpacing = beacInfo[6]
        angle = 0
    
    #distance
        if b_left < mid and b_right > mid:
            if b_avg == mid:
                b_pix = 1
                x_pix = 0
            elif b_avg > mid:
                b_pix = b_right - b_avg
                x_pix = b_avg - mid
            elif b_avg < mid:
                b_pix = b_avg - b_left
                x_pix = -(mid - b_avg)
        elif b_left < mid and b_right < mid:
            b_pix = b_avg - b_left
            x_pix = -(mid - b_avg)
        elif b_left > mid and b_right > mid:
            b_pix = b_avg - b_left
            x_pix = mid - b_avg
        else:
            x_pix = 0
            b_pix = 0

        theta = b_pix/pixratio
        theta_bar = x_pix/pixratio
        theta_bar2 = 90 - theta_bar
        theta_dot = theta_bar2 - theta

    #offset
        try:
            x_off = b_len*(x_pix/b_pix)
        except ZeroDivisionError:
            x_off = 0

        try:
            distance = ((b_len)/2)*(math.degrees(math.sin(math.radians(theta_dot))))/(math.degrees(math.sin(math.radians(theta))))
        except ZeroDivisionError:
            distance = 0
    
    #angle
        if len(ledSpacing)>3:
            leftSpace = ledSpacing[1]
            rightSpace = ledSpacing[3]
            spaceDiff = rightSpace-leftSpace
            
            if distance >= 12:
                angle = 7.14*spaceDiff-20.7
            elif distance >= 14:
                angle = 4.35*spaceDiff-4.78

        return(distance,x_off,angle)
        
    def obtain_beacon_location(self):
        
        startTime = time.time()
        self.camera.capture(self.stream, format = 'bgr')
        im = self.stream.array
        # print "capture time: " + str(time.time()-startTime)
        contours,imgray = self.contourSearch(im)
        ledDict = self.findLeds(contours)
        ledList = self.sortLeds(ledDict,contours)
        # print "detect time: " + str(time.time()-startTime)
    #beacInfo:
    #[0]-> mean x-coordinate of left-side LEDs (float)
    #[1]-> mean y-coordinate of left-side LEDs (float)
    #[2]-> mean x-coordinate of right-side LEDs (float)
    #[3]-> mean y-coordinate of right-side LEDs (float)
    #[4]-> number of leds in left group (int)
    #[5]-> number of leds in right group (int)
    #[6]-> spacing between consecutive leds
        beacInfo = self.ledGroup(ledList) 
            
        distance,offset,angle= self.calcPosition(beacInfo)
        print "Distance (cm): " + str(distance)
        print "Offset:" + str(offset)
        print "Angle:" + str(angle)
    



