/*****************************************************************************
# FroboMind (encoder_imu_odom)
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
# 2012-04-25 morl Created
# 2013-03-05 kjen Major cleanup, now supporting global robot parameters
#
#****************************************************************************/

#include <ros/ros.h>
#include <ros/console.h>

#include <msgs/encoder.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <ros/subscriber.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_broadcaster.h>
#include <string>

#define YAW_AXIS_X 1
#define YAW_AXIS_Y 2
#define YAW_AXIS_Z 3

#define YAW_SOURCE_ODOMETRY 1
#define YAW_SOURCE_IMU_ANGULAR_VELOCITY 2
#define YAW_SOURCE_IMU_ORIENTATION 3

using namespace std;

class SimpleOdom
{
public:
	SimpleOdom(double tick_to_meter, double wheel_dist, int yaw_source, int yaw_axis, double max_time_diff)
	{
		this->tick_to_meter = tick_to_meter;
		this->wheel_dist = wheel_dist;
		this->yaw_source = yaw_source;
		this->yaw_axis = yaw_axis;
		this->max_time_diff = max_time_diff;

		delta_l = delta_r = 0; // reset encoder ticks (since last published odometry)
		x = y = theta = 0; // reset map pose

		imu_yaw = prev_imu_yaw = 0; // reset imu angle

		l_time_prev = r_time_prev = imu_time_prev = ros::Time::now();
		l_updated = r_updated = l_ready = r_ready = false;
	}

	~SimpleOdom()
	{
	}

	void processLeftEncoder(const msgs::encoder::ConstPtr& msg)
	{
		l_time_prev = l_time_latest;
		l_time_latest = ros::Time::now();
		delta_l += msg->encoderticks;
		prev_l = *msg;
		l_updated = true;
	}

	void processRightEncoder(const msgs::encoder::ConstPtr& msg)
	{
		r_time_prev = r_time_latest;
		r_time_latest = ros::Time::now();
		delta_r += msg->encoderticks;
		prev_r = *msg;
		r_updated = true;
	}

	void processImu(const sensor_msgs::Imu::ConstPtr& msg)
	{
		switch (yaw_source)
		{
			case YAW_SOURCE_IMU_ORIENTATION:
				{
					double q0 = msg->orientation.x;
					double q1 = msg->orientation.y;
					double q2 = msg->orientation.z;
					double q3 = msg->orientation.w;
					imu_yaw = atan2 (2*(q0*q1+q3*q2),(pow(q3,2)-pow(q2,2)-pow(q1,2)+pow(q0,2)));
				}			
				break;

			case YAW_SOURCE_IMU_ANGULAR_VELOCITY:
				{
					imu_time_latest = ros::Time::now();
					double dt = (imu_time_latest - imu_time_prev).toSec();
					imu_time_prev = imu_time_latest;				
					switch (yaw_axis)
					{
						case YAW_AXIS_X:
							imu_yaw += (msg->angular_velocity.x*dt);
							break;
						case YAW_AXIS_Y:
							imu_yaw += (msg->angular_velocity.y*dt);
							break;
						case YAW_AXIS_Z:
							imu_yaw += (msg->angular_velocity.z*dt);
							break;
					}
				}
				break;
		}
	}

	void publishOdometry(const ros::TimerEvent& e)
	{

		if(l_updated && r_updated)
		{
			ros::Time time_now = ros::Time::now(); 

			// check update times
			if((l_time_latest - r_time_latest).toSec() > max_time_diff)
			{
				ROS_WARN("Encoder stamp (left - right) differs %.3f",(l_time_latest - r_time_latest).toSec());
			}
			r_updated = l_updated = false;
			
			delta_l *= tick_to_meter; // convert from ticks to meter
			delta_r *= tick_to_meter;

			double dx = (delta_l + delta_r)/2; // approx. distance (assuming linear motion during dt)

			// calculate change in orientation using odometry
			// double dtheta = (delta_r - delta_l)/wheel_dist; 

			// or calculate change in orientation using IMU
			double dtheta = imu_yaw - prev_imu_yaw;
			prev_imu_yaw = imu_yaw;

 			delta_l = delta_r = 0;
			double ang = theta + dtheta/2;
			x += cos(ang)*dx; //update odometry given position of the robot
			y += sin(ang)*dx;
			theta += dtheta;

			if (theta < -M_PI) // angle housekeeping
				theta += 2*M_PI;
			else if(theta > M_PI)
				theta -= 2*M_PI;

			// ROS_INFO("Odo %.3f %.3f %2f",x, y, theta);

   			//since all odometry is 6DOF we'll need a quaternion created from yaw
			geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromYaw(theta);

			// publish the transform message
			odom_trans.header.stamp = time_now;
			odom_trans.header.frame_id = odom_frame;
			odom_trans.child_frame_id = base_frame;
			odom_trans.transform.translation.x = x;
			odom_trans.transform.translation.y = y;
			odom_trans.transform.translation.z = 0.0;
			odom_trans.transform.rotation = odom_quat;
			odom_broadcaster.sendTransform(odom_trans);

			// publish odometry message
			odom.header.stamp = time_now;
			odom.header.frame_id = odom_frame;
			odom.pose.pose.position.x = x;
			odom.pose.pose.position.y = y;
			odom.pose.pose.orientation = odom_quat;
			double dt = (l_time_latest - l_time_prev).toSec(); // assuming that left and right have the same interval
			odom.twist.twist.linear.x  = dx/dt; 
			odom.twist.twist.angular.z = dtheta/dt;
			odom_pub.publish(odom);
		}
	}

	// public variables
	ros::Publisher odom_pub;
	tf::TransformBroadcaster odom_broadcaster;
	std::string base_frame,odom_frame;

private:
	double tick_to_meter, wheel_dist;
	int yaw_source, yaw_axis;
	double imu_yaw, prev_imu_yaw;
	double max_time_diff;
	double delta_l, delta_r;
	bool l_updated, r_updated;
	ros::Time l_time_latest, l_time_prev, r_time_latest, r_time_prev, imu_time_latest, imu_time_prev;
	bool l_ready, r_ready;
	double x, y, theta;
	msgs::encoder prev_l, prev_r;
	nav_msgs::Odometry odom;
	geometry_msgs::TransformStamped odom_trans;
};

// main
int main(int argc, char** argv) {
	ros::init(argc, argv, "odometry_publisher");
	ros::NodeHandle nh("~");
	ros::NodeHandle n;

	string publish_topic;
	string subscribe_enc_l;
	string subscribe_enc_r;
	string subscribe_imu;

	double wheel_radius, wheel_ticks_rev, tick_to_meter;
	double wheel_dist;
	int yaw_source, yaw_axis;
	double max_time_diff;
	ros::Subscriber s1,s2,s3;

	// publishers
	nh.param<string>("odom_pub", publish_topic, "/fmKnowledge/odom");

	// subscribers
	nh.param<string>("enc_left_sub", subscribe_enc_l, "/fmInformation/encoder_left");
	nh.param<string>("enc_right_sub", subscribe_enc_r, "/fmInformation/encoder_right");
	nh.param<string>("imu_sub", subscribe_imu, "/fmInformation/imu");

	// robot parameters
	nh.param<double>("/diff_steer_wheel_radius", wheel_radius, 0.25);
	nh.param<double>("/diff_steer_wheel_ticks_per_rev", wheel_ticks_rev, 360);
	nh.param<double>("/diff_steer_wheel_distance", wheel_dist, 1.0);
	tick_to_meter = 2*M_PI*wheel_radius/wheel_ticks_rev;

	// other parameters
	std::string yaw_source_str;
	yaw_source = YAW_SOURCE_ODOMETRY;
	nh.param<string>("yaw_angle_source", yaw_source_str, "odometry");
	if ( yaw_source_str.compare ("odometry") == 0)
		yaw_source = YAW_SOURCE_ODOMETRY;
	else if ( yaw_source_str.compare ("imu_angular_velocity") == 0)
		yaw_source = YAW_SOURCE_IMU_ANGULAR_VELOCITY;
	else if ( yaw_source_str.compare ("imu_orientation") == 0)
		yaw_source = YAW_SOURCE_IMU_ORIENTATION;

	std::string yaw_axis_str;
	yaw_source = YAW_AXIS_Z;
	nh.param<string>("imu_yaw_axis", yaw_axis_str, "z");
	if ( yaw_axis_str.compare ("x") == 0)
		yaw_axis = YAW_AXIS_X;
	else if ( yaw_axis_str.compare ("y") == 0)
		yaw_axis = YAW_AXIS_Y;
	else if ( yaw_axis_str.compare ("z") == 0)
		yaw_axis = YAW_AXIS_Z;

	nh.param<double>("enc_max_time_diff", max_time_diff,1);

	// init class
	SimpleOdom p(tick_to_meter, wheel_dist, yaw_source, yaw_axis, max_time_diff);

	// subscriber callback functions
	s1 = nh.subscribe(subscribe_enc_l,15,&SimpleOdom::processLeftEncoder,&p);
	s2 = nh.subscribe(subscribe_enc_r,15,&SimpleOdom::processRightEncoder,&p);
	s3 = nh.subscribe(subscribe_imu,15,&SimpleOdom::processImu,&p);

	// publish odometry at 0.02s = 50 Hz
	p.odom_pub = n.advertise<nav_msgs::Odometry>(publish_topic.c_str(), 25);
	nh.param<std::string>("vehicle_frame",p.base_frame,"base_footprint");
	nh.param<std::string>("odom_estimate_frame",p.odom_frame,"odom_combined");
	ros::Timer t;
	t = nh.createTimer(ros::Duration(0.02), &SimpleOdom::publishOdometry,&p);

	ros::spin();

	return 0;
}




