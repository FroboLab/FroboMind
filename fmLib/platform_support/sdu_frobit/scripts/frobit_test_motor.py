#!/usr/bin/env python
#/****************************************************************************
# FroboMind (frobit_motor_test)
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
#****************************************************************************/

import rospy
from msgs.msg import nmea
from operator import xor

first_time = 1
node_upd_freq = 10 # Hz


nmea_pfbst_interval = 200 # ms
nmea_pfbct_watchdog_timeout = 100 # ms

spd_left = 17
spd_right = 22

def nmea_checksum(msg):
	s = map(ord, msg[1:msg.index('*')])
	checksum = reduce(xor, s)
	return (hex(checksum).upper()[-2:])

def talker():
	global first_time
	global nmea_pfbst_interval
	global nmea_pfbct_watchdog_timeout
	pub = rospy.Publisher('/fmLib/nmea_to_frobit', nmea)
	nm = nmea()
	rospy.init_node('talker')
	r = rospy.Rate(node_upd_freq) # hz

	while not rospy.is_shutdown():
		if first_time == 1:
#			nm.type = 'PFBCO'
#			nm.data = ['%d' %(nmea_pfbst_interval), '%d' % (nmea_pfbct_watchdog_timeout)]
#			nm.header.stamp = rospy.Time.now()
#			pub.publish(nm)
			first_time = 0

#		nm.type = 'PFBCO'
#		nm.data = ['%d' %(nmea_pfbst_interval), '%d' % (nmea_pfbct_watchdog_timeout)]
		nm.type = 'PFBCT'
		nm.data = ['%d' %(spd_left), '%d' % (spd_right)]
		nm.header.stamp = rospy.Time.now()
		pub.publish(nm)
		r.sleep()

if __name__ == '__main__':
	try:
		talker()
	except rospy.ROSInterruptException:
		pass

