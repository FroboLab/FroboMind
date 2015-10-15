#!/usr/bin/env python
#/****************************************************************************
# Differential odometry reset node
# Copyright (c) 2014, Kjeld Jensen <kjeld@frobomind.org>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#	* Redistributions of source code must retain the above copyright
#	  notice, this list of conditions and the following disclaimer.
#	* Redistributions in binary form must reproduce the above copyright
#	  notice, this list of conditions and the following disclaimer in the
#	  documentation and/or other materials provided with the distribution.
#	* Neither the name FroboMind nor the
#	  names of its contributors may be used to endorse or promote products
#	  derived from this software without specific prior written permission.
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
"""
This node publishes a single message which resets the current state of the
differential_odometry_node and then quits.

2014-02-27 KJ First version
"""

# imports
import rospy
from msgs.msg import FloatArrayStamped
from math import pi

# parameters
update_interval = 0.2 # [s]
reset_timeout = 1.0

# launch node
rospy.init_node('differential_odometry_reset')

# variables
reset_sent = False
reset_time = rospy.get_time() + reset_timeout

# get parameters
easting = rospy.get_param("~easting", '0.0')
northing = rospy.get_param("~northing", '0.0')
heading = rospy.get_param("~heading", '0.0')

# create a publisher
topic_odom_reset = rospy.get_param("~odom_reset_pub", '/fmKnowledge/odometry_reset')
pub = rospy.Publisher(topic_odom_reset, FloatArrayStamped)

# define the reset message
reset_msg = FloatArrayStamped()
reset_msg.header.stamp = rospy.Time.now()
reset_msg.data = [easting, northing, heading*pi/180.0]

# loop until shutdown
while not rospy.is_shutdown() and rospy.get_time() < reset_time:
	pub.publish(reset_msg)
	rospy.sleep(update_interval)

rospy.loginfo(rospy.get_name() + ": Odometry has been reset to [%.3f %.3f %.1f]" % (easting, northing, heading))

