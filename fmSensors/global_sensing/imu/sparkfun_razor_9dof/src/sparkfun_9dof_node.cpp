/*****************************************************************************
# Sparkfun Razor 9DOF IMU interface
# Copyright (c) 2012-2013,
#	Morten Larsen <mortenlarsens@gmail.com>
#	Kjeld Jensen <kjeld@frobomind.org>
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
# This node subscribes to left and right encoder ticks from a differentially
# steered robot platform and publishes an odometry message.
#
# 2012-03-13 morl original code
# 2013-03-04 kjen cleanup, now utilizes serial_nmea,
# 2013-03-04 kjen now publishes ROS Imu message instead of acceleration and gyro
#
#****************************************************************************/

#include <string>
#include "ros/ros.h"
#include "sparkfun_9dof.h"

using namespace std;

int main(int argc, char **argv)
{
	ros::init(argc, argv, "sparkfun_razor_9dof");
	ros::NodeHandle nh;
	ros::NodeHandle n("~");

	string sub_id;
	string pub_imu, pub_mag;
	string frame_id;
	bool enu_selected;
	bool use_imu,use_mag;

	n.param<string>("frame_id", frame_id, "imu_link");
	n.param<bool>("use_enu",enu_selected,false);

	n.param<string>("nmea_from_imu_sub",sub_id,"/fmData/nmea_from_imu");
	n.param<string>("imu_pub",pub_imu,"/fmInformation/imu");
	n.param<string>("magnetometer_pub",pub_mag,"/fmInformation/magnetometer");

	n.param<bool>("publish_imu",use_imu,true);
	n.param<bool>("publish_magnetometer",use_mag,true);

	SparkFun9DOF node;

	node.setFrameId(frame_id);

	node.setImuTopic(nh.advertise<sensor_msgs::Imu>    (pub_imu, 1));
	node.setMagTopic (nh.advertise<msgs::magnetometer> (pub_mag, 1));

	node.enableImu(use_imu);
	node.enableMag(use_mag);
	node.selectENU (enu_selected);

	ros::Subscriber sub = nh.subscribe(sub_id.c_str(), 1, &SparkFun9DOF::newMsgCallback,&node);

	ros::spin();

	return 0;

}
