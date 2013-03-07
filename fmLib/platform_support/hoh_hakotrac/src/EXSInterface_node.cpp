/*
 * EXSInterface_node.cpp
 *
 *  Created on: Sep 23, 2012
 *      Author: morl
 */

#include <ros/ros.h>
#include <msgs/can.h>
#include <msgs/encoder.h>
#include <msgs/steering_angle_cmd.h>
#include "EXSInterface.h"

#include <sensor_msgs/Joy.h>

int main(int argc, char** argv)
{
	ros::init(argc,argv,"EXSInterface_node");
	ros::NodeHandle nh("~");
	ros::NodeHandle n;
	ros::Subscriber s1,s2,s3,s4,s5;
	std::string can_pub,can_sub,encoder_pub,cmd_vel_sub,steering_sub,angle_pub,rpm_topic,rpm_sub,wii_sub;
	double rr;

	nh.param<std::string>("can_tx_publisher_topic",can_pub,"/fmCSP/can0_tx");
	nh.param<std::string>("can_rx_subscriber_topic",can_sub,"/fmCSP/can0_rx");
	nh.param<std::string>("encoder_publisher_topic",encoder_pub,"/fmSensors/encoder_lr");
	nh.param<std::string>("angle_publisher_topic",angle_pub,"/fmSensors/encoder_angle");
	nh.param<std::string>("cmd_vel_subscriber_topic",cmd_vel_sub,"/fmKinematics/cmd_vel");
	nh.param<std::string>("steering_angle_subscriber_topic",steering_sub,"fmKinematics/steering_angle_cmd");
	nh.param<std::string>("rpm_subscriber_topic",rpm_sub,"fmControllers/engine_rpm_cmd");
	nh.param<std::string>("joy_deadman_subscriber_topic",wii_sub,"/fmHMI/joy");
	nh.param<std::string>("rpm_cmd_topic",rpm_topic,"/fmPSP/rpm_cmd");
	nh.param<double> ("output_rate",rr,20);

	EXSInterface esx;

	esx.encoder_pub = nh.advertise<msgs::encoder>(encoder_pub,10);
	esx.angle_pub = nh.advertise<msgs::encoder>(angle_pub,10);
	esx.can_tx_pub = nh.advertise<msgs::can>(can_pub,10);

	s1 = nh.subscribe<msgs::can>(can_sub.c_str(),10,&EXSInterface::onCANMsg,&esx);
	s2 = nh.subscribe<msgs::steering_angle_cmd> (steering_sub,10,&EXSInterface::onSteeringAngle,&esx);
	s5 = nh.subscribe<msgs::engine_rpm> (rpm_topic,10,&EXSInterface::onRPMCmd,&esx);

	s3 = nh.subscribe<geometry_msgs::Twist> (cmd_vel_sub,10,&EXSInterface::onCmdVel,&esx);
	s4 = nh.subscribe<sensor_msgs::Joy>(wii_sub,10,&EXSInterface::onJoy,&esx);

	ros::Timer t = nh.createTimer(ros::Rate(rr),&EXSInterface::onTimer,&esx);

	ros::spin();

	return 0;
}





