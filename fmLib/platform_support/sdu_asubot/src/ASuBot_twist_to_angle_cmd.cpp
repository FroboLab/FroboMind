/****************************************************************************
 #
 # Copyright (c) 2011 Søren Hundevadt Nielsen <shn@kbm.sdu.dk>
 #
 # Permission is hereby granted, free of charge, to any person obtaining a copy
 # of this software and associated documentation files (the "Software"), to deal
 # in the Software without restriction, including without limitation the rights
 # to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 # copies of the Software, and to permit persons to whom the Software is
 # furnished to do so, subject to the following conditions:
 #
 # The above copyright notice and this permission notice shall be included in
 # all copies or substantial portions of the Software.
 #
 # THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 # IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 # FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 # AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 # LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 # OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 # THE SOFTWARE.
 #
 *****************************************************************************
 # File: ASuBot_fwd_kinematics_node.cpp
 # Purpose: ASuBot forward kinematics.
 # Project: Field Robot - Vehicle Interface Computer
 # Author: Søren Hundevadt Nielsen <shn@kbm.sdu.dk>
 # Created: Aug 23, 2011 Søren Hundevadt Nielsen, Source written
 ****************************************************************************/

#include "ros/ros.h"
#include "geometry_msgs/TwistStamped.h"
#include "msgs/steering_angle_cmd.h"

ros::Timer can_tx_timer;
msgs::steering_angle_cmd aes25_msg;
geometry_msgs::Twist twist_cmd_out;
ros::Publisher wheel_pub;
ros::Publisher twist_pub;

double vehicle_length;

void twistmsgCallbackHandler(const geometry_msgs::TwistStampedConstPtr& twist_msg) {

	aes25_msg.header.stamp = ros::Time::now();

	const double V = twist_msg->twist.linear.x; // velocity m/s
	const double L = vehicle_length; // distance between back and frontwheels

	double omega = twist_msg->twist.angular.z;

	aes25_msg.steering_angle = atan2(omega*L,V);

	twist_cmd_out.linear.x = V;

	twist_pub.publish(twist_cmd_out);
	wheel_pub.publish(aes25_msg);
}

int main(int argc, char **argv) {

	ros::init(argc, argv, "ASuBot_twist_to_angle_node");

	ros::NodeHandle nh("~");
	ros::NodeHandle n;

	std::string publisher_topic;
	std::string twist_subscriber_topic, twist_publisher_topic;

	std::string ASubot_wheel_publisher_topic;

	nh.param<std::string> ("steering_angle_publisher_topic", ASubot_wheel_publisher_topic,"/fmKinematics/steering_angle_cmd");
	nh.param<std::string> ("cmd_vel_publisher_topic", twist_publisher_topic,"/fmKinematics/cmd_vel");
	nh.param<std::string> ("twist_subscriber_topic", twist_subscriber_topic,"/fmControllers/cmd_vel");
	nh.param<double> ("axle_distance_front_rear",vehicle_length,1.56);

	wheel_pub = nh.advertise<msgs::steering_angle_cmd> (ASubot_wheel_publisher_topic.c_str(),1,1);
	twist_pub = nh.advertise<geometry_msgs::Twist>(twist_publisher_topic,1,1);
	ros::Subscriber twist_sub = nh.subscribe<geometry_msgs::TwistStamped> (twist_subscriber_topic.c_str(), 1, &twistmsgCallbackHandler);

	aes25_msg.header.stamp = ros::Time::now();


	aes25_msg.steering_angle = 0;
	twist_cmd_out.linear.x = 0;
	wheel_pub.publish(aes25_msg);
	twist_pub.publish(twist_cmd_out);

	ros::Rate r(10);
	while (ros::ok()) {
		ros::spinOnce();
		r.sleep();
	}

	return 0;
}

