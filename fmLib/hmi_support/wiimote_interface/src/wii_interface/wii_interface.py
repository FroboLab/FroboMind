#/****************************************************************************
# FroboMind wii_interface.py
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
import math
from msgs.msg import StringStamped
from sensor_msgs.msg import Joy,JoyFeedback,JoyFeedbackArray
from std_msgs.msg import Bool
from geometry_msgs.msg import TwistStamped
import smach
import smach_ros

class WiiInterface():
    """
        Wiimote interface.
        Button B is deadman button
        Button 1 enters automode
        Button 2 exits automode
        Rumble feedback upon under voltage
        When not in automode, remote controlled driving is active.
    """
    def __init__(self):
        # Setup parameters
        self.automode = False
        self.deadman = Bool(False)
        self.linear = 0
        self.angular = 0
        self.next_state_change = rospy.Time.now() + rospy.Duration(1)
        self.rumble_on = False
        self.warning = False
        self.pitch = [0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0]
        self.roll = [0 , 0 , 0 , 0 , 0 , 0 , 0, 0 , 0 , 0]
        self.ptr = 0
        self.twist = TwistStamped()
        self.fb = JoyFeedbackArray( array=[JoyFeedback( type=JoyFeedback.TYPE_LED, intensity=0, id=0 ), 
                                           JoyFeedback( type=JoyFeedback.TYPE_LED, intensity=0, id=1 ),
                                           JoyFeedback( type=JoyFeedback.TYPE_LED, intensity=0, id=2 ), 
                                           JoyFeedback( type=JoyFeedback.TYPE_LED, intensity=0, id=3 ), 
                                           JoyFeedback( type=JoyFeedback.TYPE_RUMBLE, intensity=0, id=0 )] ) 

        # Get parameters
        self.reduced_range = rospy.get_param("~reduced_range",50) # Given in percent
        self.reduced_range = self.reduced_range / 100.0 # Convert to ratio
        self.deadband = rospy.get_param("~deadband",5) # Given in percent
        self.deadband = self.deadband / 100.0 # Convert to ratio
        self.max_linear_velocity = rospy.get_param("~max_linear_velocity",2)
        self.max_angular_velocity = rospy.get_param("~max_angular_velocity",1)
        self.publish_frequency = rospy.get_param("~publish_frequency",5)    
        
        # Get topic names
        self.deadman_topic = rospy.get_param("~deadman_topic",'deadman')
        self.cmd_vel_topic = rospy.get_param("~cmd_vel_topic",'cmd_vel')
        self.feedback_topic = rospy.get_param("~feedback_topic",'joy/set_feedback') 
        self.joy_topic = rospy.get_param("~joy_topic",'joy')
        self.status_topic = rospy.get_param("~status_topic",'status')     

        # Setup topics
        self.deadman_pub = rospy.Publisher(self.deadman_topic, Bool)
        self.twist_pub = rospy.Publisher(self.cmd_vel_topic, TwistStamped)
        self.fb_pub = rospy.Publisher(self.feedback_topic, JoyFeedbackArray)
        self.joy_sub = rospy.Subscriber(self.joy_topic, Joy, self.onJoy )
        self.status_sub = rospy.Subscriber(self.status_topic, StringStamped , self.onStatus)
        
    def onJoy(self,msg):
        """
            Callback method handling Joy messages
        """
        # Handle automode buttons
        if msg.buttons[0] == 1 :
            self.automode = True
        if msg.buttons[1] == 1 :
            self.automode = False
            
        # Handle deadman button
        if msg.buttons[3] == 1 :
            self.deadman = True
        else :
            self.deadman = False      
             
        # Generate pitch and roll and save value in list
        self.pitch[self.ptr] = math.atan2(msg.axes[1], math.sqrt(math.pow(msg.axes[0], 2) + math.pow(msg.axes[2], 2))) / 1.57;
        self.roll[self.ptr] = math.atan2(msg.axes[0], math.sqrt(math.pow(msg.axes[1], 2) + math.pow(msg.axes[2], 2))) / 1.57;
        self.ptr = self.ptr + 1
        if self.ptr > 9 :
            self.ptr = 0

    def onStatus(self,msg):
        """
            Callback method to translate status string into user feedback
        """
        if "under voltage" in msg.data :
            self.warning = True
        else:
            self.warning = False
                        
    def publishCmdVel(self): 
        """
            Method to average and publish twist from wiimote input
        """
        # Calculate average of the ten latest messages
        self.linear = (sum(self.pitch)/len(self.pitch))
        self.angular = -(sum(self.roll)/len(self.roll))      
        
        # Implement deadband on linear velocity
        if self.linear < self.deadband and self.linear > -self.deadband :
            self.linear = 0;
            
        # Implement deadband on angular velocity
        if self.angular < self.deadband and self.angular > -self.deadband :
            self.angular = 0;
        
        # Implement reduced range on linear velocity    
        if self.linear > self.reduced_range :
            self.linear = self.reduced_range
        elif self.linear < - self.reduced_range :
            self.linear = - self.reduced_range
        self.linear = ( self.linear / self.reduced_range ) * self.max_linear_velocity
        
        # Implement reduced range on angular velocity
        if self.angular > self.reduced_range :
            self.angular = self.reduced_range
        elif self.angular < - self.reduced_range :
            self.angular = - self.reduced_range
        self.angular = ( self.angular / self.reduced_range ) * self.max_angular_velocity

        # Publish twist message                   
        self.twist.header.stamp = rospy.Time.now()
        self.twist.twist.linear.x = self.linear
        self.twist.twist.angular.z = self.angular
        self.twist_pub.publish(self.twist)
        
    def publishDeadman(self):
        """
            Method to publish Bool message on deadman topic
        """
        self.deadman_pub.publish(self.deadman)
    
    def publishFeedback(self):
        """
            Method to generate and publish wiimote feedback message.
            array[0-3] are leds (set .intensity to 1 for on or 0 for off)
            array[4] is rumble (set .intensity to 1 for on or 0 for off)
        """
        # Handle periodic rumble feedback
        if self.warning or self.rumble_on :
            if rospy.Time.now() > self.next_state_change :
                if self.rumble_on :
                    self.next_state_change = rospy.Time.now() + rospy.Duration(1)
                    self.fb.array[4].intensity = 0
                    self.rumble_on = False
                else :
                    self.next_state_change = rospy.Time.now() + rospy.Duration(0.5)
                    self.fb.array[4].intensity = 1
                    self.rumble_on = True
                    
        # Publish feedback message            
        self.fb_pub.publish(self.fb)   
    