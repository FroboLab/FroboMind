/*****************************************************************************
# FroboMind (differential_odometry)
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
# The node subscribes to left and right encoder ticks from a differentially
# steered robot platform and optionally gyro rate or orientation from an IMU.
# The node publishes an odometry message.
#
# 2012-04-25 morl Created
# 2013-03-07 kjen Major cleanup, renamed to differential_odometry
#                 now supporting global robot parameters
#                 now supporting odometry, imu angular rate and imu_orientation
#                 as yaw angle source
#                 now supporting x,y,z as angular rate yaw axis (including inverted)
#
#****************************************************************************/
// includes
#include <ros/ros.h>
#include <ros/console.h>

#include <msgs/encoder.h>
#include <sensor_msgs/Imu.h>
#include <nav_msgs/Odometry.h>
#include <ros/subscriber.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_broadcaster.h>
#include <string>

// defines	
#define IDENT 					"differential_odometry_node"

#define TIMEOUT_LAUNCH				5
#define TIMEOUT_ENCODER				0.5
#define TIMEOUT_IMU				0.5

#define M_PI2					2*M_PI

#define YAW_AXIS_X 				1
#define YAW_AXIS_X_INVERTED			2
#define YAW_AXIS_Y				3
#define YAW_AXIS_Y_INVERTED			4
#define YAW_AXIS_Z				5
#define YAW_AXIS_Z_INVERTED			6

#define YAW_SOURCE_ODOMETRY 			1
#define YAW_SOURCE_IMU_ANGULAR_VELOCITY 	2
#define YAW_SOURCE_IMU_ORIENTATION 		3

using namespace std;

class SimpleOdom
{
public:
	SimpleOdom(double tick_to_meter, double wheel_dist, int yaw_source, int yaw_axis)
	{
		this->tick_to_meter = tick_to_meter;
		this->wheel_dist = wheel_dist;
		this->yaw_source = yaw_source;
		this->yaw_axis = yaw_axis;

		delta_l = delta_r = 0; // reset encoder ticks (since last published odometry)
		x = y = theta = 0; // reset map pose

		imu_yaw = prev_imu_yaw = 0; // reset imu angle

		encoder_timeout = false;
		imu_timeout = false;
	
		time_launch = l_time_prev = r_time_prev = imu_time_prev = ros::Time::now();
		l_updated = r_updated = l_ready = r_ready = false;
	}

	~SimpleOdom()
	{
	}

	double angle_limit (double angle) // keep angle within [0;2*pi[
	{ 
		while (angle >= M_PI2)
			angle -= M_PI2;
		while (angle < 0)
			angle += M_PI2;
		return angle;
	}

	double angle_diff (double angle_new, double angle_old) // return signed difference between new and old angle
	{
		double diff = angle_new - angle_old;
		while (diff < -M_PI)
			diff += M_PI2;
		while (diff > M_PI)
			diff -= M_PI2;
		return diff;
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
					double qx = msg->orientation.x;
					double qy = msg->orientation.y;
					double qz = msg->orientation.z;
					double qw = msg->orientation.w;
					imu_yaw = atan2(2*(qx*qy + qw*qz), qw*qw + qx*qx - qy*qy - qz*qz);
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
						case YAW_AXIS_X_INVERTED:
							imu_yaw -= (msg->angular_velocity.x*dt);
							break;
						case YAW_AXIS_Y:
							imu_yaw += (msg->angular_velocity.y*dt);
							break;
						case YAW_AXIS_Y_INVERTED:
							imu_yaw -= (msg->angular_velocity.y*dt);
							break;
						case YAW_AXIS_Z:
							imu_yaw += (msg->angular_velocity.z*dt);
							break;
						case YAW_AXIS_Z_INVERTED:
							imu_yaw -= (msg->angular_velocity.z*dt);
							break;
					}
					imu_yaw = angle_limit (imu_yaw);
				}
				break;
		}
	}

	void publishOdometry(const ros::TimerEvent& e)
	{

		ros::Time time_now = ros::Time::now();
		if(l_updated && r_updated)
		{
			encoder_timeout = false;

			// check if we are receiving IMU updates
			if (yaw_source != YAW_SOURCE_ODOMETRY) 
			{
				if (! imu_timeout)
				{
					if ((time_now - imu_time_latest).toSec() >= TIMEOUT_IMU && (time_now - time_launch).toSec() > TIMEOUT_LAUNCH)
					{
						imu_timeout = true;
						ROS_WARN("%s: IMU data not available!", IDENT);
					}
				}
				else
				{
					if ((time_now - imu_time_latest).toSec() < TIMEOUT_IMU)
					{
						imu_timeout = false;
						ROS_INFO("%s: Receiving data from IMU.", IDENT);
					}
				}
			}
			
			delta_l *= tick_to_meter; // convert from ticks to meter
			delta_r *= tick_to_meter;

			double dx = (delta_l + delta_r)/2; // approx. distance (assuming linear motion during dt)
			double dtheta;

			// calculate change in orientation using odometry
			if (yaw_source == YAW_SOURCE_ODOMETRY)
			{
				dtheta = (delta_r - delta_l)/wheel_dist; 
			}
			else // or calculate change in orientation using IMU
			{
				dtheta = angle_diff (imu_yaw, prev_imu_yaw);
				prev_imu_yaw = imu_yaw;
			}

 			delta_l = delta_r = 0;
			double ang = theta + dtheta/2;
			x += cos(ang)*dx; //update odometry given position of the robot
			y += sin(ang)*dx;
			theta += dtheta;
			theta = angle_limit (theta); // keep theta within [0;2*pi[

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
		else if (! encoder_timeout)
		{
			if(((time_now-l_time_latest).toSec() > TIMEOUT_ENCODER || (time_now-r_time_latest).toSec() > TIMEOUT_ENCODER)
				&& (time_now-time_launch).toSec() > TIMEOUT_LAUNCH)
			{
				encoder_timeout = true;
				ROS_WARN("%s: Encoder data not available!", IDENT);
			}
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
	bool encoder_timeout, imu_timeout;
	double delta_l, delta_r;
	bool l_updated, r_updated;
	ros::Time time_launch, l_time_latest, l_time_prev, r_time_latest, r_time_prev, imu_time_latest, imu_time_prev;
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
	nh.param<string>("yaw_angle_source", yaw_source_str, "not initialized");
	if ( yaw_source_str.compare ("odometry") == 0)
	{
		yaw_source = YAW_SOURCE_ODOMETRY;
		ROS_INFO("%s Source for rotation about yaw axis: Odometry", IDENT);
	}
	else if ( yaw_source_str.compare ("imu_angular_velocity") == 0)
	{
		yaw_source = YAW_SOURCE_IMU_ANGULAR_VELOCITY;
		ROS_INFO("%s Source for rotation about yaw axis: IMU angular velocity", IDENT);
	}
	else if ( yaw_source_str.compare ("imu_orientation") == 0)
	{
		yaw_source = YAW_SOURCE_IMU_ORIENTATION;
		ROS_INFO("%s Source for rotation about yaw axis: IMU orientation", IDENT);
	}
	else
	{
		yaw_source = YAW_SOURCE_ODOMETRY;
		ROS_WARN("%s Source for rotation about yaw axis: Unknown, defaults to odometry", IDENT);
	}
	if (yaw_source == YAW_SOURCE_IMU_ANGULAR_VELOCITY)
	{
		std::string yaw_axis_str;
		nh.param<string>("imu_angular_velocity_yaw_axis", yaw_axis_str, "not initialized");
		if ( yaw_axis_str.compare ("x") == 0)
		{
			yaw_axis = YAW_AXIS_X;
			ROS_INFO("%s IMU yaw axis (ENU): X", IDENT);
		}		
		else if ( yaw_axis_str.compare ("-x") == 0)
		{
			yaw_axis = YAW_AXIS_X_INVERTED;
			ROS_INFO("%s IMU yaw axis (ENU): X (inverted)", IDENT);
		}
		else if ( yaw_axis_str.compare ("y") == 0)
		{
			yaw_axis = YAW_AXIS_Y;
			ROS_INFO("%s IMU yaw axis (ENU): Y", IDENT);
		}
		else if ( yaw_axis_str.compare ("-y") == 0)
		{
			yaw_axis = YAW_AXIS_Y_INVERTED;
			ROS_INFO("%s IMU yaw axis (ENU): Y (inverted)", IDENT);
		}
		else if ( yaw_axis_str.compare ("z") == 0)
		{
			yaw_axis = YAW_AXIS_Z;
			ROS_INFO("%s IMU yaw axis (ENU): Z", IDENT);
		}
		else if ( yaw_axis_str.compare ("-z") == 0)
		{
			yaw_axis = YAW_AXIS_Z_INVERTED;
			ROS_INFO("%s IMU yaw axis (ENU): Z (inverted)", IDENT);
		}
		else
		{
			yaw_axis = YAW_AXIS_Z;
			ROS_WARN("%s IMU yaw axis: Unknown, defaults to Z", IDENT);
		}		
	}

	// init class
	SimpleOdom p(tick_to_meter, wheel_dist, yaw_source, yaw_axis);

	// subscriber callback functions
	s1 = nh.subscribe(subscribe_enc_l,15,&SimpleOdom::processLeftEncoder,&p);
	s2 = nh.subscribe(subscribe_enc_r,15,&SimpleOdom::processRightEncoder,&p);
	s3 = nh.subscribe(subscribe_imu,15,&SimpleOdom::processImu,&p);

	// publish odometry at 0.02s = 50 Hz
	p.odom_pub = n.advertise<nav_msgs::Odometry>(publish_topic.c_str(), 25);
	nh.param<std::string>("vehicle_frame",p.base_frame,"base_footprint");
	nh.param<std::string>("odom_estimate_frame",p.odom_frame,"/odom_combined");
	ros::Timer t;
	t = nh.createTimer(ros::Duration(0.02), &SimpleOdom::publishOdometry,&p);

	ros::spin();

	return 0;
}




