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

from fmExecutors.msg import timed_turnAction,timed_turnGoal

from msgs.msg import adc
from geometry_msgs.msg import TwistStamped


class TimedTurn():
    """
    Behaviour to navigate in a row given an estimate of the offset 
    and heading of the row from a sensor.
    This Behaviour does not require that the robot knows where it is, i.e the planner is not used 
    to drive in the row, instead a rabbit is produced locally to the robot.
    """
    
    def __init__(self,name,vel_topic):
        """
            initialises the behaviour.
            starts the action server, connects to the row_topic
        """
        rospy.loginfo("Starting")        
        
        self.vel_x = 0.0
        self.vel_z = 0.0
        self.turn_time = 0
        self.turn_vel = 0
        self.start_time = rospy.Time.now()
        
        
        self.vel_pub = rospy.Publisher(vel_topic,TwistStamped)
        
        self._action_name = name
        self._server = actionlib.SimpleActionServer(self._action_name,timed_turnAction,auto_start=False)
        self._server.register_goal_callback(self.goal_cb)
        self._server.register_preempt_callback(self.preempt_cb);
        
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
        vel.twist.linear.x = 0
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
        self.turn_time = g.time
        self.turn_vel = g.vel
        self.start_time = rospy.Time.now()
            
    def on_timer(self,e):
        rospy.loginfo("Running control loop")
        if self._server.is_active():
            if (rospy.Time.now() - self.start_time) > rospy.Duration(self.turn_time):
                self.vel_x = 0
                self.vel_z = 0
                self._server.set_succeeded(None)
            else:
                self.vel_x = 0
                self.vel_z = self.turn_vel
        
            vel = TwistStamped()
            vel.header.stamp = rospy.Time.now()
            vel.twist.linear.x = self.vel_x
            vel.twist.angular.z = self.vel_z
            self.vel_pub.publish(vel)


if __name__ == "__main__":
    
    rospy.init_node("timed_turn")
    
    TimedTurn(rospy.get_name(),"/fmKinematics/cmd_vel")
    
    rospy.spin()
