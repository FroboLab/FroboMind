#!/usr/bin/env python
#/****************************************************************************
# FroboMind positionGoalActionServer.py
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
import actionlib
import numpy as np
import math

from nav_msgs.msg import Odometry
from geometry_msgs.msg import TwistStamped
from position_action_server.msg import positionAction

class vector():
    """
        Utility class to handle simple 2D vector calculations
    """
    def __init__(self,a,b):
        self.vec = [a,b]
    
    def __sub__(self,other):
        return vector(self.vec[0] - other[0] , self.vec[1] - other[1])
    
    def __getitem__(self,k):
        return self.vec[k]
        
    def length(self):
        return math.sqrt(np.dot(self.vec,self.vec))
    
    def angle(self,other):
        tmp = np.dot(self.vec,other.vec) / (self.length() * other.length())
        if tmp > 1 :
            return math.acos(1)
        elif  tmp < -1 :
            return math.acos(-1)
        else :
            return math.acos(tmp)
    
    def rotate(self,rad):
        new_x = math.cos(rad)*self.vec[0] - math.sin(rad)*self.vec[1]
        new_y = math.sin(rad)*self.vec[0] + math.cos(rad)*self.vec[1]
        return vector(new_x,new_y)

class positionGoalActionServer():
    """
        Action server taking position goals and generating twist messages accordingly
    """
    def __init__(self,name):
        # Get topics and parameters from parameter server
        self.max_linear_velocity = rospy.get_param("~max_linear_velocity",2)
        self.max_angular_velocity = rospy.get_param("~max_angular_velocity",1)
        self.max_distance_error = rospy.get_param("~max_distance_error",0.1)
        self.straight_line = rospy.get_param("~straight_line",False)
        
        self.odometry_topic = rospy.get_param("~odometry_topic","/base_pose_ground_truth")
        self.cmd_vel_topic = rospy.get_param("~cmd_vel_topic","/fmSignals/cmd_vel")
        
        self.odom_sub = rospy.Subscriber(self.odometry_topic, Odometry, self.onOdometry )
        self.twist_pub = rospy.Publisher(self.cmd_vel_topic, TwistStamped)
        
        # Set parameters not yet on server
        self.rate = rospy.Rate(5)
        
        # Init variables
        self.twist = TwistStamped()
        self.destination = vector(0,0)
        self.position = vector(0,0)
        self.quaternion = [0,0,0,0]
        self.distance_error = 0
        self.angle_error = 0
         
        self.z = 0
        self.w = 0
        
        # Setup and start simple action server      
        self._action_name = name
        self._server = actionlib.SimpleActionServer(
            self._action_name, positionAction, auto_start=False, execute_cb=self.execute)
        self._server.register_preempt_callback(self.preempt_cb);
        self._server.start()
        
    def preempt_cb(self):                   
        # Publish zero twist message
        self.twist.header.stamp = rospy.Time.now()
        self.twist.twist.linear.x = 0
        self.twist.twist.angular.z = 0            
        self.twist_pub.publish(self.twist)

        self._server.set_preempted()
   
    def execute(self,goal):
        # Construct a vector from position goal
        self.destination.vec[0] = goal.x
        self.destination.vec[1] = goal.y  

        while not rospy.is_shutdown() :
            # Check for new goal
            if self._server.is_new_goal_available() :
                break
            
            # Construct a vector from desired path
            path = self.destination - self.position
            self.distance_error = path.length()
            
            # If position is unreached
            if self.distance_error > self.max_distance_error :
                # Calculate roll from quaternion and construct a heading vector
                roll = math.atan2( 2*(self.quaternion[0]*self.quaternion[1] + self.quaternion[2]*self.quaternion[3]), 
                                   1 - 2*(math.pow(self.quaternion[1], 2) + math.pow(self.quaternion[2],2)))
                head = vector(math.cos(roll),math.sin(roll))
                
                # Calculate angle between heading vector and path vector
                self.angle_error = head.angle(path)

                # Rotate the heading vector according to the calculated angle and test correspondence
                # with the path vector. If not zero sign must be flipped. This is to avoid the sine trap.
                t1 = head.rotate(self.angle_error)
                if path.angle(t1) != 0 :
                    self.angle_error = -self.angle_error
                    
                # Generate twist from distance and angle errors (For now simple 1:1)
                self.twist.twist.linear.x = self.distance_error
                self.twist.twist.angular.z = self.angle_error
                
                # If straight line driving is true, angle error must be corrected before distance error
                if self.straight_line and ( self.angle_error > 0.5 or self.angle_error < -0.5 ) :
                        self.twist.twist.linear.x = self.distance_error * 0.1    

                # Implement maximum linear velocity and maximum angular velocity
                if self.twist.twist.linear.x > self.max_linear_velocity:
                    self.twist.twist.linear.x = self.max_linear_velocity
                if self.twist.twist.linear.x < -self.max_linear_velocity:
                    self.twist.twist.linear.x = -self.max_linear_velocity
                if self.twist.twist.angular.z > self.max_angular_velocity:
                    self.twist.twist.angular.z = self.max_angular_velocity
                if self.twist.twist.angular.z < -self.max_angular_velocity:
                    self.twist.twist.angular.z = -self.max_angular_velocity
                
                # If not preempted, add a time stamp and publish the twist
                if not self._server.is_preempt_requested() :     
                    self.twist.header.stamp = rospy.Time.now()               
                    self.twist_pub.publish(self.twist)
                
                # Block   
                self.rate.sleep()
            else:
                # Succeed the action - position has been reached
                self._server.set_succeeded()
                
                # Publish a zero twist to stop the robot
                self.twist.header.stamp = rospy.Time.now()
                self.twist.twist.linear.x = 0
                self.twist.twist.angular.z = 0
                self.twist_pub.publish(self.twist)                
                break
        # Return statement
        if self._server.is_preempt_requested() :
            return 'preempted' 
        elif rospy.is_shutdown() :
            return 'aborted'
        else :               
            return 'succeeded'
        
    def onOdometry(self, msg):
        # Extract the orientation quaternion
        self.quaternion[0] = msg.pose.pose.orientation.x
        self.quaternion[1] = msg.pose.pose.orientation.y
        self.quaternion[2] = msg.pose.pose.orientation.z
        self.quaternion[3] = msg.pose.pose.orientation.w
        
        # Extract the position vector
        self.position.vec[0] = msg.pose.pose.position.x
        self.position.vec[1] = msg.pose.pose.position.y       
        
if __name__ == '__main__':
    try:
        rospy.init_node('positionAction')
        action_server = positionGoalActionServer(rospy.get_name())
        rospy.spin()
    except rospy.exceptions.ROSInterruptException:
        pass        