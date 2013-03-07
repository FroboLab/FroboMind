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

from tf import TransformListener, TransformBroadcaster
from geometry_msgs.msg import Twist

from fmExecutors.msg import *



class DriveForwardAction():
    """
        Drives x meters either forward or backwards depending on the given distance.
        the velocity should always be positive. 
    """
    def __init__(self,name,odom_frame,base_frame):
        """
        
        @param name: the name of the action
        @param odom_frame: the frame the robot is moving in (odom_combined)
        @param base_frame: the vehicles own frame (usually base_link)
        """
        
        self._action_name = name
        self.__odom_frame = odom_frame
        self.__base_frame = base_frame
        self.__server =  actionlib.SimpleActionServer(self._action_name,drive_forwardAction,auto_start=False)
        self.__server.register_preempt_callback(self.preempt_cb)
        self.__server.register_goal_callback(self.goal_cb)
        
        self.__cur_pos = 0
        self.__start_yaw = 0
        self.__cur_yaw = 0
        
        self.__feedback = drive_forwardFeedback()
        
        self.__listen = TransformListener()
        self.vel_pub = rospy.Publisher("/fmControllers/cmd_vel_auto",Twist)
        
        self.__turn_timeout = 200
        self.__start_time = rospy.Time.now()
        self.turn_vel = 0
        self.new_goal = False
        
        self.fix_offset = 0
        
        self.__server.start()

    def preempt_cb(self):
        rospy.loginfo("Preempt requested")
        self.__publish_cmd_vel(0)
        self.__server.set_preempted()
    
    def goal_cb(self):
        """
            called when we receive a new goal
            the goal contains a desired radius and a success radius in which we check if the turn succeeded or not
            the message also contains if we should turn left or right
        """
        g = self.__server.accept_new_goal()
        self.__desired_amount= g.amount
        self.vel = g.vel
        self.fix_offset = g.fix_offset
        
        self.new_goal = True
    
    def on_timer(self,e):
        """
            called regularly by a ros timer
            in here we simply orders the vehicle to start moving either forward 
            or backwards depending on the distance sign, while monitoring the 
            travelled distance, if the distance is equal to or greater then this
            action succeeds.
        """
        if self.__server.is_active():
            if self.new_goal:
                self.new_goal = False
                if self.__get_start_position():
                    self.__start_time = rospy.Time.now()
                else:
                    self.__server.set_aborted(text="could not find vehicle")
            else:
                if rospy.Time.now() - self.__start_time > rospy.Duration(self.__turn_timeout):
                    self.__server.set_aborted(text="timeout on action")
                    self.__publish_cmd_vel(0)
                else:
                    if self.__get_current_position():
                        if self.__get_distance() >= math.fabs(self.__desired_amount):
                            result = drive_forwardResult()
                            result.end_dist = self.__get_distance()
                            self.__server.set_succeeded(result, "distance covered")
                            self.__publish_cmd_vel(0)
                        else:
                            self.__publish_cmd_vel(1)
                            self.__feedback.progress = self.__get_distance()
                            self.__server.publish_feedback(self.__feedback)
                            
    def __get_distance(self):
        return math.sqrt(math.pow(self.__cur_pos[0][0] - self.__start_pos[0][0],2) +
                         math.pow(self.__cur_pos[0][1] - self.__start_pos[0][1],2))
    
    def __get_start_position(self):
        ret = False
        try:
            self.__start_pos = self.__listen.lookupTransform(self.__odom_frame,self.__base_frame,rospy.Time(0))
            self.__start_yaw = tf.transformations.euler_from_quaternion(self.__start_pos[1])[2]
            ret = True
        except (tf.LookupException, tf.ConnectivityException),err:
            rospy.loginfo("could not locate vehicle")
        
        return ret
    
    def __get_current_position(self):
        ret = False
        try:
            self.__cur_pos = self.__listen.lookupTransform(self.__odom_frame,self.__base_frame,rospy.Time(0))
            self.__cur_yaw = tf.transformations.euler_from_quaternion(self.__cur_pos[1])[2]
            ret = True
        except (tf.LookupException, tf.ConnectivityException),err:
            rospy.loginfo("could not locate vehicle")
        
        return ret
    
    def __publish_cmd_vel(self,stop):
        vel = Twist()
        if self.__desired_amount > 0:
            vel.linear.x = self.vel
        else:
            vel.linear.x = -self.vel
        
        vel.angular.z = self.fix_offset
        
        if stop == 0:
            vel.linear.x = 0
            vel.angular.z = 0
            
        self.vel_pub.publish(vel)

if __name__ == "__main__":
    
    rospy.init_node("drive_forward")
    
    action_server = DriveForwardAction("drive_forward","odom_combined","base_footprint")
    
    t = rospy.Timer(rospy.Duration(0.05),action_server.on_timer)
    
    rospy.spin()
    
