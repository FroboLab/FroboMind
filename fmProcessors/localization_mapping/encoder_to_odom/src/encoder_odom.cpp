/*
 * simple_odom.cpp
 *
 *  Created on: Apr 25, 2012
 *      Author: morl
 */
#include <ros/ros.h>
#include <ros/console.h>

#include <msgs/encoder.h>
#include <nav_msgs/Odometry.h>
#include <ros/subscriber.h>
#include <geometry_msgs/TransformStamped.h>
#include <tf/transform_broadcaster.h>
#include <string>

using namespace std;

class SimpleOdom
{
public:
	SimpleOdom(double tm_l, double tm_r, double wdist, double max_diff)
	{
		this->tm_l = tm_l;
		this->tm_r = tm_r;
		this->wdist = wdist;
		this->max_diff = max_diff;

		delta_l = delta_r = 0;
		x = y = theta = 0;

		l_updated = r_updated = l_ready = r_ready = false;
	}

	~SimpleOdom()
	{
	}

	void processLeftEncoder(const msgs::encoder::ConstPtr& msg)
	{
		if(!l_ready)
		{
			prev_l = *msg;
			l_ready = true;
		}
		else
		{
			ROS_INFO("TicksLeft %d",msg->encoderticks);

			l_up_time = ros::Time::now();
			delta_l += msg->encoderticks;
			prev_l = *msg;
			l_updated = true;
		}

	}

	void processRightEncoder(const msgs::encoder::ConstPtr& msg)
	{
		if(!r_ready)
		{
			prev_r = *msg;
			r_ready = true;
		}
		else
		{
			r_up_time = ros::Time::now();
			delta_r += msg->encoderticks;
			prev_r = *msg;
			r_updated = true;
		}

	}

	void publishOdometry(const ros::TimerEvent& e)
	{
		time_before = time_now; // update time stamps
		time_now = ros::Time::now(); 

		if(l_updated && r_updated)
		{
			// check update times
			if((l_up_time - r_up_time).toSec() > max_diff)
			{
				ROS_WARN("Encoder stamp (left - right) differs %.4f",(l_up_time - r_up_time).toSec());
			}
			r_updated = l_updated = false;
			ROS_INFO("Odo deltaticks %.1f %.1f",delta_l, delta_r);
			
			delta_l *= tm_l; // convert from ticks to meter
			delta_r *= tm_r;

			double dx = (delta_l + delta_r)/2; // approx. distance (assuming linear motion during dt)
			double dtheta = (delta_r - delta_l)/wdist; // change in orientation
			// ROS_INFO("Odo %.3f %.3f",delta_l, delta_r);
 			delta_l = delta_r = 0;

			double ang = theta + dtheta/2;
			x += cos(ang)*dx; //update odometry given position of the robot
			y += sin(ang)*dx;
			theta += dtheta;

			if (theta < -M_PI) // angle housekeeping
				theta += 2*M_PI;
			else if(theta > M_PI)
				theta -= 2*M_PI;

			ROS_INFO("Odo %.3f %.3f %2f",x, y, theta);

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
			odom.twist.twist.linear.x  = dx;
			odom.twist.twist.angular.z = dtheta;
			odom_pub.publish(odom);
		}
	}

	// public variables
	ros::Publisher odom_pub;
	tf::TransformBroadcaster odom_broadcaster;
	std::string base_frame,odom_frame;

private:
	double tm_l, tm_r;
	double wdist;
	double delta_l, delta_r;
	bool l_updated, r_updated;
	ros::Time l_up_time, r_up_time;
	bool l_ready, r_ready;
	double max_diff;
	double x, y, theta;
	ros::Time time_now, time_before;
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

	double wdist, tm_r, tm_l, max_diff;
	ros::Subscriber s1,s2;

	// publishers
	nh.param<string>("odom_pub", publish_topic, "/fmKnowledge/odom");

	// subscribers
	nh.param<string>("enc_left_sub", subscribe_enc_l, "/fmInformation/encoder_left");
	nh.param<string>("enc_right_sub", subscribe_enc_r, "/fmInformation/encoder_right");

	// parameters
	nh.param<double>("ticks_to_meter_left", tm_l, 2*(M_PI)*0.24/8192);
	nh.param<double>("ticks_to_meter_right", tm_r, 2*(M_PI)*0.24/8192);
	nh.param<double>("max_time_diff", max_diff,1);
	nh.param<double>("wheel_distance_meter", wdist, 0.59);

	// init class
	SimpleOdom p(tm_l, tm_r, wdist, max_diff);

	// subscriber callback functions
	s1 = nh.subscribe(subscribe_enc_l,15,&SimpleOdom::processLeftEncoder,&p);
	s2 = nh.subscribe(subscribe_enc_r,15,&SimpleOdom::processRightEncoder,&p);

	// publish odometry at 0.02s = 50 Hz
	p.odom_pub = n.advertise<nav_msgs::Odometry>(publish_topic.c_str(), 25);
	nh.param<std::string>("vehicle_frame",p.base_frame,"base_footprint");
	nh.param<std::string>("odom_estimate_frame",p.odom_frame,"odom_combined");
	ros::Timer t;
	t = nh.createTimer(ros::Duration(0.02), &SimpleOdom::publishOdometry,&p);

	ros::spin();

	return 0;
}




