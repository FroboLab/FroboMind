/****************************************************************************
# FroboMind template_cpp_node
# Copyright (c) 2011-2013, author Morten Larsen mortenlarsens@gmail.com
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
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "ros/ros.h"
#include <msgs/serial.h>
#include "sensor_msgs/Imu.h"
#include "geometry_msgs/Pose.h"
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>

// defines
#define IDENT "vectornav_vn100_node"

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

class VectorNav
{
public:
	VectorNav()
	{


	}

	~VectorNav()
	{

	}

	void set_covariance(double cov_x, double cov_y, double cov_z)
	{
		imu_msg.orientation_covariance[0] = cov_x;
		imu_msg.orientation_covariance[4] = cov_y;
		imu_msg.orientation_covariance[8] = cov_z;
	}

	void processSerialCallback(const msgs::serial::ConstPtr& rx_msg)
	{
		boost::char_separator<char> sep("$*,");
		tokenizer::iterator tok_iter;
		tokenizer tokens(rx_msg->data, sep);

		tok_iter = tokens.begin();
		// get NMEA identifier
		if((*tok_iter).compare("VNQMR") == 0)
		{
			processIMU(tokens,rx_msg->data);
		}
		else
		{
			ROS_INFO("Ignoring Unknown NMEA identifier %s",(*tok_iter).c_str());
		}

	}
	bool enu_selected;
	sensor_msgs::Imu imu_msg;
	ros::Publisher imu_pub;
	std::string frame_id;

private:

	void processIMU(tokenizer& tokens,std::string raw)
	{
		unsigned int chk,calc_chk;
		tokenizer::iterator tok_iter = tokens.begin();

		// skip identifier
		tok_iter++;

		if(countTokens(tokens) == 15)
		{
			chk = extractChecksum(tokens,14);
			calc_chk = calculateChecksum(raw);
			if(chk == calc_chk)
			{
				try
				{
					imu_msg.header.stamp = ros::Time::now();
					imu_msg.header.frame_id = frame_id;

					if(enu_selected)
					{
						// ENU ORIENTATION
						// swap x and y and negate z
						imu_msg.orientation.y = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.orientation.x = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.orientation.z = - boost::lexical_cast<float>(*tok_iter++);
						imu_msg.orientation.w = boost::lexical_cast<float>(*tok_iter++);

						tok_iter++; //skip magnetometer
						tok_iter++;
						tok_iter++;

						// acceleration swap x y negate z
						imu_msg.linear_acceleration.y = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.linear_acceleration.x = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.linear_acceleration.z = - boost::lexical_cast<float>(*tok_iter++);

						// angular rates swap  x y and negate z
						imu_msg.angular_velocity.y = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.angular_velocity.x = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.angular_velocity.z = - boost::lexical_cast<float>(*tok_iter++);
					}
					else
					{
						// NED orientation
						imu_msg.orientation.x = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.orientation.y = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.orientation.z = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.orientation.w = boost::lexical_cast<float>(*tok_iter++);

						tok_iter++; //skip magnetometer
						tok_iter++;
						tok_iter++;

						// acceleration
						imu_msg.linear_acceleration.x = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.linear_acceleration.y = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.linear_acceleration.z = boost::lexical_cast<float>(*tok_iter++);

						// angular rates
						imu_msg.angular_velocity.x = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.angular_velocity.y = boost::lexical_cast<float>(*tok_iter++);
						imu_msg.angular_velocity.z = boost::lexical_cast<float>(*tok_iter++);
					}

					imu_pub.publish(imu_msg);

				}
				catch(boost::bad_lexical_cast &)
				{
					ROS_ERROR("Could not convert accelerometer and gyro readings");
				}

			}
			else
			{
				ROS_WARN("Checksum did not match");
			}
		}

	}

	unsigned int extractChecksum(tokenizer tokens,int chk_msg_start)
	{
		tokenizer::iterator tok_iter_chk;
		unsigned int chk;

		tok_iter_chk = tokens.begin();

		for(int j=0;j<chk_msg_start;j++)
		{
			tok_iter_chk++;
		}

		std::stringstream ss(*tok_iter_chk);
		ss >> std::hex >> chk;

		return chk;
	}

	unsigned int calculateChecksum(std::string s)
	{
		uint8_t chk = 0;

		for(unsigned int i=1;i<s.length();i++)
		{
			if(s.at(i) == '*')
			{
				break;
			}
			chk ^= (uint8_t)s.at(i);
		}

		return chk;
	}

	unsigned int countTokens(tokenizer& tokens)
	{
		tokenizer::iterator tok_iter;
		unsigned int count = 0;
		for (tok_iter = tokens.begin(); tok_iter != tokens.end(); ++tok_iter)
		{
			count++;
		}

		return count;
	}

};


int main(int argc, char **argv)
{
  ros::init(argc, argv, "imu_parser");
  ros::NodeHandle nh("~");
  ros::NodeHandle n;

  VectorNav imu;

  std::string subscribe_topic_id;
  std::string publish_topic_id;

  double cov_x,cov_y,cov_z;

  nh.param<std::string> ("vectornav_vn100_sub", subscribe_topic_id, "/fmData/imu_rx");
  nh.param<std::string> ("imu_pub", publish_topic_id, "/fmInformation/imu");
  nh.param<std::string> ("frame_id", imu.frame_id, "imu_link");
  nh.param<bool>("use_enu",imu.enu_selected,true);

	if (imu.enu_selected == true)
		ROS_INFO("%s Coordinate system: East, North, Up (ENU)", IDENT);
	else
		ROS_INFO("%s Coordinate system: North, East, Down (NED)", IDENT);

  nh.param<double> ("covariance_x",cov_x,1.0);
  nh.param<double> ("covariance_y",cov_y,1.0);
  nh.param<double> ("covariance_z",cov_z,1.0);

  imu.set_covariance(cov_x,cov_y,cov_z);

  ros::Subscriber sub = nh.subscribe(subscribe_topic_id, 1,&VectorNav::processSerialCallback,&imu);
  imu.imu_pub = nh.advertise<sensor_msgs::Imu> (publish_topic_id, 1);

  ros::spin();

  return 0;

}
