#!/usr/bin/env python
import rospy
from indoor_tracker.MarkerLocator import *

from time import time
import sys
import numpy as np
import cv2.cv as cv
import math

if __name__ == '__main__':
    t0 = time()
    t1 = time()
    t2 = time()
    
    print 'function vers1 takes %f' %(t1-t0)
    print 'function vers2 takes %f' %(t2-t1)
    
    toFind = [7]    
    
    if PublishToROS:  
        RP = RosPublisher(toFind)
       
    cd = CameraDriver(toFind)
    t0 = time()
     
    while cd.running:
        (t1, t0) = (t0, time())
      #  print "time for one iteration: %f" % (t0 - t1)
        cd.getImage()
        cd.processFrame()
        #cd.drawDetectedMarkers()
        cd.showProcessedFrame()
        cd.handleKeyboardEvents()
        y = cd.returnPositions()     
        if PublishToROS:
            RP.publishMarkerLocations(y)
        else:
            pass
            #print y
            try:
                print("%3d %3d %8.3f" % (y[0][0], y[0][1], y[0][2]))
            except:
                pass            
    print("Stopping")
