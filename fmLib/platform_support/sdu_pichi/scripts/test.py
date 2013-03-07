#!/usr/bin/env python
import rospy
import math
from msgs.msg import *
from sensor_msgs.msg import *
from std_msgs.msg import *
from geometry_msgs.msg import *

class TestInterface():
    def __init__(self):
        # Init node and setup topics
        rospy.init_node('test_interface')
        self.publisher = rospy.Publisher("/fmData/rx", serial)
        
        self.tx_sub = rospy.Subscriber("/fmData/tx", serial, self.onTx)
        self.rx_sub = rospy.Subscriber("/fmData/rx", serial, self.onRx)
        self.msg = serial()

        # Spin
        try:
            while not rospy.is_shutdown():
                self.msg.header.stamp = rospy.Time.now()
                self.msg.data = "FF=0"
                self.publisher.publish(self.msg)
                rospy.sleep(0.1)
                
                self.msg.header.stamp = rospy.Time.now()
                self.msg.data = "FS=0"
                self.publisher.publish(self.msg)
                rospy.sleep(0.1)
                
                self.msg.header.stamp = rospy.Time.now()
                self.msg.data = "CB=30:30"
                self.publisher.publish(self.msg)
                rospy.sleep(0.1)
                
                self.msg.header.stamp = rospy.Time.now()
                self.msg.data = "P=40:40"
                self.publisher.publish(self.msg)
                rospy.sleep(0.1)
                
                self.msg.header.stamp = rospy.Time.now()
                self.msg.data = "T=40:10:10"
                self.publisher.publish(self.msg)
                rospy.sleep(0.1)
        except rospy.ROSInterruptException:
            pass
    
    def onRx(self,msg):
        print(msg.data)
            
    def onTx(self,msg):
        print(msg.data)
        if "?FID" in msg.data :
            self.msg.header.stamp = rospy.Time.now()
            self.msg.data = "FID=Roboteq blah blah"
            self.publisher.publish(self.msg)


if __name__ == '__main__':
    node = TestInterface()
    rospy.spin()
    



    