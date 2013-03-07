#!/usr/bin/env python
#*****************************************************************************
# FroboMind (frobit_test_set_speed)
# Copyright (c) 2013, Kjeld Jensen <kjeld@frobomind.org>
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
#*****************************************************************************
#
# The script publishes the topics below to the frobit_node
#	/fmSignals/deadman
#	/fmSignals/cmd_vel_left
#	/fmSignals/cmd_vel_right
#
# This makes it possible to test different wheel speeds etc.
#
#*****************************************************************************
import rospy
from geometry_msgs.msg import TwistStamped
from std_msgs.msg import Bool

node_upd_freq = 10 # Hz
spd_left = 0.32 # m/s
spd_right = 0.32 # m/s

def talker():
	rospy.init_node('frobit_test_set_speed')

	deadman_pub = rospy.Publisher('/fmSignals/deadman', Bool)
	deadman = Bool()

	cmd_vel_left_pub = rospy.Publisher('/fmSignals/cmd_vel_left', TwistStamped)
	twist_left = TwistStamped()

	cmd_vel_right_pub = rospy.Publisher('/fmSignals/cmd_vel_right', TwistStamped)
	twist_right = TwistStamped()

	r = rospy.Rate(node_upd_freq) # set talker update frequency

	while not rospy.is_shutdown():
		deadman.data = 1 # publish deadman topic
		deadman_pub.publish(deadman)

		twist_left.header.stamp = rospy.Time.now()
		twist_left.twist.linear.x = spd_left; # publish cmd_vel topic for left wheel
		cmd_vel_left_pub.publish(twist_left)

		twist_right.header.stamp = rospy.Time.now()
		twist_right.twist.linear.x = spd_right; # publish cmd_vel topic for right wheel
		cmd_vel_right_pub.publish(twist_right)

		r.sleep()

if __name__ == '__main__':
	try:
		talker()
	except rospy.ROSInterruptException:
		pass

