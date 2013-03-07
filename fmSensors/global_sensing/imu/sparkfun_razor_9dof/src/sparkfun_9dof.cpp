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
#include "sparkfun_9dof.h"

#define DEG2RAD M_PI/180.0

SparkFun9DOF::SparkFun9DOF()
{
	// TODO Auto-generated constructor stub

}

SparkFun9DOF::~SparkFun9DOF()
{
	// TODO Auto-generated destructor stub
}

void SparkFun9DOF::enableImu(bool yes)
{
	is_enabled_imu_ = yes;
}

void SparkFun9DOF::enableMag(bool yes)
{
	is_enabled_mag_ = yes;
}

void SparkFun9DOF::selectENU(bool yes)
{
	is_selected_enu_ = yes;
}

void SparkFun9DOF::setFrameId(string frame_id)
{
	frame_id_ = frame_id;
}

void SparkFun9DOF::newMsgCallback(const msgs::nmea::ConstPtr& msg)
{
	if ((msg->type).compare("SFIMU") == 0)
	{
		if(msg->length == 6)
		{
			if(msg->valid == false)
			{
				ROS_WARN("IMU checksum did not match, skipping measurement");
			}
			else
			{
				try
				{
					a_x = boost::lexical_cast<int>(msg->data[0]) / 1000.0 * 4 * 9.82;
					a_y = boost::lexical_cast<int>(msg->data[1]) / 1000.0 * 4 * 9.82;
					a_z = boost::lexical_cast<int>(msg->data[2]) / 1000.0 * 4 * 9.82;
					g_x = boost::lexical_cast<int>(msg->data[3]) * 1/14.375 * DEG2RAD;
					g_y = boost::lexical_cast<int>(msg->data[4]) * 1/14.375 * DEG2RAD;
					g_z = boost::lexical_cast<int>(msg->data[5]) * 1/14.375 * DEG2RAD;

					if(is_enabled_imu_)
					{
						publishImu();
					}
				}
				catch(boost::bad_lexical_cast &)
				{
					ROS_ERROR("Could not convert Sparkfun Razor 9dof IMU accelerometer and gyro readings");
				}
			}
		}
		else
		{
			ROS_ERROR("Unexpected number of tokens in $SPIMU message");
		}
	}
	else if ((msg->type).compare("SFMAG") == 0)
	{
		if (msg->length == 3)
		{
			if(msg->valid == false)
			{
				ROS_WARN("MAG checksum did not match, skipping measurement");
			}
			else
			{
				try
				{
					m_x = boost::lexical_cast<int>(msg->data[0]) / 660.0;
					m_y = boost::lexical_cast<int>(msg->data[1]) / 660.0;
					m_z = boost::lexical_cast<int>(msg->data[2]) / 660.0;

					if(is_enabled_mag_)
					{
						publishMag();
					}
				}
				catch (boost::bad_lexical_cast &)
				{
					ROS_ERROR("Could not convert Sparkfun Razor 9dof IMU magnetormeter readings");
				}
			}
		}
		else
		{
			ROS_ERROR("Unexpected number of tokens in $SPMAG message");
		}
	}
	else
	{
		ROS_INFO("Ignoring Unknown Sparkfun Razor 9dof IMU NMEA type");
	}
}

void SparkFun9DOF::publishMag()
{
	++msg_mag_.header.seq;
	msg_mag_.header.frame_id = frame_id_;
	msg_mag_.header.stamp = ros::Time::now();
	msg_mag_.x = m_x;
	msg_mag_.y = m_y;
	msg_mag_.z = m_z;

	pub_mag_.publish(msg_mag_);
}

void SparkFun9DOF::publishImu()
{


	++msg_imu.header.seq;
	msg_imu.header.frame_id = frame_id_;
	msg_imu.header.stamp = ros::Time::now();

	if(is_selected_enu_)
	{
		// ENU orientation
		msg_imu.orientation_covariance[0] = -1; // As instructed here: http://www.ros.org/doc/api/sensor_msgs/html/msg/Imu.html
		msg_imu.orientation.x = 0;
		msg_imu.orientation.y = 0;
		msg_imu.orientation.z = 0;
		msg_imu.orientation.w = 0;

		// acceleration
		msg_imu.linear_acceleration.x = a_x;
		msg_imu.linear_acceleration.y = a_y;
		msg_imu.linear_acceleration.z = a_z;

		// angular rates
		msg_imu.angular_velocity.x = g_x;
		msg_imu.angular_velocity.y = g_y;
		msg_imu.angular_velocity.z = g_z;
	}
	else
	{
		// NED ORIENTATION
		/* swap x and y and negate z */
		msg_imu.orientation.y = 0;
		msg_imu.orientation.x = 0;
		msg_imu.orientation.z = 0;
		msg_imu.orientation.w = 0;

		// acceleration swap x y negate z
		msg_imu.linear_acceleration.y = a_x;
		msg_imu.linear_acceleration.x = a_y;
		msg_imu.linear_acceleration.z = - a_z;

		// angular rates swap x y and negate z
		msg_imu.angular_velocity.y = g_x;
		msg_imu.angular_velocity.x = g_y;
		msg_imu.angular_velocity.z = -g_z;
	}
	pub_imu_.publish(msg_imu);
}

void SparkFun9DOF::setImuTopic(ros::Publisher pub)
{
	pub_imu_ = pub;
}

void SparkFun9DOF::setMagTopic(ros::Publisher pub)
{
	pub_mag_ = pub;
}

