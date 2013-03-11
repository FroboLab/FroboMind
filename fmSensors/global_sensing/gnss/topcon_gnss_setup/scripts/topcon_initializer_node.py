#!/usr/bin/env python
#/****************************************************************************
# FroboMind init_serial_nmea.py
# Copyright (c) 2011-2013, author Leon Bonde Larsen <leon@bondelarsen.dk>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name FroboMind nor the
#      names of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#****************************************************************************/
import rospy
from msgs.msg import serial

class serialInitializer():
    """    
        Publishes serial message every second
    """
    def __init__(self):
        rospy.init_node('topcon_initializer')
        
        self.topic = rospy.get_param("~serial_topic",'/fmData/tx')
        self.message1 = rospy.get_param("~message_string1","")
        self.message2 = rospy.get_param("~message_string2","")
        self.message3 = rospy.get_param("~message_string3","")
        
        self.rate = rospy.Rate(1)
        self.serial_pub = rospy.Publisher(self.topic, serial)
        self.serial_msg = serial()
        
        
    def spin(self): 
        while not rospy.is_shutdown() :
            self.serial_msg.header.stamp = rospy.Time.now()
            if self.message1 :
                self.serial_msg.data = self.message1     
                self.serial_pub.publish(self.serial_msg)
            if self.message2 :
                self.serial_msg.data = self.message2     
                self.serial_pub.publish(self.serial_msg)
            if self.message3 :
                self.serial_msg.data = self.message3     
                self.serial_pub.publish(self.serial_msg)    
            self.rate.sleep()
            
    
if __name__ == '__main__':
    node = serialInitializer()
    try:
        node.spin()
    except rospy.ROSInterruptException:
        pass