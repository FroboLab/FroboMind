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
# 2013-03-04 kjen cleanup, now utilizes serial_nmea
#
#****************************************************************************/

#ifndef SPARKFUN9DOF_H_
#define SPARKFUN9DOF_H_
#include <ros/ros.h>
#include <ros/console.h>

#include <msgs/nmea.h>
#include <sensor_msgs/Imu.h>
#include <msgs/magnetometer.h>

#include <string>

using namespace std;
class SparkFun9DOF
{
public:
	SparkFun9DOF();
	virtual ~SparkFun9DOF();
	void enableImu(bool yes);
	void enableMag(bool yes);
	void selectENU(bool yes);

	void setFrameId(string frame_id);
	void setImuTopic(ros::Publisher pub);
	void setMagTopic(ros::Publisher pub);

	void newMsgCallback(const msgs::nmea::ConstPtr& msg);


private:
	bool is_enabled_mag_,is_enabled_imu_;
	bool is_selected_enu_;

	double a_x,a_y,a_z,g_x,g_y,g_z,m_x,m_y,m_z;

	void publishMag();
	void publishImu();

	sensor_msgs::Imu msg_imu;
	msgs::magnetometer msg_mag_;

	ros::Publisher pub_imu_,pub_mag_;

	string frame_id_;
};

#endif /* SPARKFUN9DOF_H_ */
