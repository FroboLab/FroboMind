#/****************************************************************************
# FroboMind template_cpp_node
# Copyright (c) 2011-2013, author Kent Stark Olsen kent.stark.olsen@gmail.com
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#	* Redistributions of source code must retain the above copyright
#  	notice, this list of conditions and the following disclaimer.
#	* Redistributions in binary form must reproduce the above copyright
#  	notice, this list of conditions and the following disclaimer in the
#  	documentation and/or other materials provided with the distribution.
#	* Neither the name FroboMind nor the
#  	names of its contributors may be used to endorse or promote products
#  	derived from this software without specific prior written permission.
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

#!/usr/bin/env python
import roslib; 
roslib.load_manifest("fmExecutors")
import rospy

import actionlib
import math
import tf

from fmExecutors.msg import *

from msgs.msg import adc
from geometry_msgs.msg import TwistStamped


class FollowLine():
    """
    Behaviour to navigate in a row given an estimate of the offset 
    and heading of the row from a sensor.
    This Behaviour does not require that the robot knows where it is, i.e the planner is not used 
    to drive in the row, instead a rabbit is produced locally to the robot.
    """
    
    def __init__(self,name,adc_topic,vel_topic):
        """
            initialises the behaviour.
            starts the action server, connects to the row_topic
        """
        rospy.loginfo("Starting")        
        self.imax = 2
        self.i = 0.0
        self.igain = 0.0
        self.pgain = 0.0
        self.dgain = 0.0
        self.error = 0.0
        self.prev_error = 0.0
        self.i = 0
        
        
        self.__feedback_msg = navigate_in_rowFeedback()
        
        self.vel_x = 0.0
        self.vel_z = 0.0
        
        self.prev_error = 0
        
        self.cur_adc = None
        
        self.vel_pub = rospy.Publisher(vel_topic,TwistStamped)
        
        self._action_name = name
        self._server = actionlib.SimpleActionServer(self._action_name,navigate_in_rowAction,auto_start=False)
        self._server.register_goal_callback(self.goal_cb)
        self._server.register_preempt_callback(self.preempt_cb);
        
        self._subscriber = rospy.Subscriber(adc_topic, adc, callback=self.adc_msg_callback)
        self._last_adc_msg = None
        self._server.start()
        
        self.t = rospy.Timer(rospy.Duration(0.1),self.on_timer)
        
    def preempt_cb(self):
        """
            Called when the action client whishes to preempt us.
        """
        rospy.loginfo("preempt received")
        self._server.set_preempted()
        vel = TwistStamped()
        vel.header.stamp = rospy.Time.now()
        vel.twist.linear.x =0
        vel.twist.angular.z = 0
        self.vel_pub.publish(vel)
        
    
    def goal_cb(self):
        """
            Called when we receive a new goal.
            We could inspect the goal before accepting it,
            however for start we only accept it.
        """
        rospy.loginfo("goal received")
        g = self._server.accept_new_goal()
        
        self.pgain = g.P
        self.igain = g.I
        self.dgain = g.D
        self.imax = g.I_max
        
        rospy.loginfo("running line follow with PID: %.4f, %.4f, %.4f" % (self.pgain,self.igain,self.dgain))
        
    def adc_msg_callback(self,msg):
        """
            This function is called every time a new row message is
            received. 
        """
        self.cur_adc = msg
            
    def on_timer(self,e):
        rospy.loginfo("Running control loop")
        if self._server.is_active():
            if self.cur_adc is not None:
                left = self.cur_adc.value[0]
                right = self.cur_adc.value[1]
                
                error = left - right;
                
                self.i += error
                if self.i > self.imax:
                    self.i = self.imax
                if self.i < -self.imax:
                    self.i = -self.imax
                    
                
                self.__feedback_msg.error = error
                self.__feedback_msg.integral = self.i
                self.__feedback_msg.differential = (error - self.prev_error)
                self._server.publish_feedback(self.__feedback_msg)
                self.vel_x = 0.2
                self.vel_z = error * self.pgain + self.i*self.igain + (error - self.prev_error)* self.dgain;
                self.prev_error = error
            else:
                self.vel_x = 0
                self.vel_z = 0
            
            vel = TwistStamped()
            vel.header.stamp = rospy.Time.now()
            vel.twist.linear.x = self.vel_x
            vel.twist.angular.z = self.vel_z
            self.vel_pub.publish(vel)
        


if __name__ == "__main__":
    
    rospy.init_node("follow_line")
    
    FollowLine(rospy.get_name(),"/fmSensors/adc","/fmKinematics/cmd_vel")
    
    rospy.spin()
