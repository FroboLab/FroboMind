/****************************************************************************
# FroboMind frobit_interface.cpp
# Copyright (c) 2011-2013, author Leon Bonde Larsen <leon@bondelarsen.dk>
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
#include "frobit_interface.hpp"

FrobitInterface::FrobitInterface()
:local_node_handler("~"),global_node_handler()
{
	left_vel = 0;
	right_vel = 0;
	vel_to_motor_const = 0;

	active = false;
	deadman = false;

	last_deadman_received = ros::Time::now();
};

void FrobitInterface::makeItSpin( void )
{
	local_node_handler.param<std::string>(	"nmea_sub", 			topics.nmea_sub, 		"/fmData/nmea_from_frobit"		);
	local_node_handler.param<std::string>(	"nmea_pub", 			topics.nmea_pub, 		"/fmActuators/duty"				);
	local_node_handler.param<std::string>(	"deadman_sub", 			topics.deadman, 		"/fmSignals/deadman"			);
	local_node_handler.param<std::string>(	"cmd_vel_left_sub",		topics.cmd_vel_left, 	"/fmDecision/twist"				);
	local_node_handler.param<std::string>(	"cmd_vel_right_sub", 	topics.cmd_vel_right, 	"/fmDecision/twist"				);
	local_node_handler.param<std::string>(	"encoder_left_pub", 	topics.encoder_left, 	"/fmInformation/encoder_left"	);
	local_node_handler.param<std::string>(	"encoder_right_pub", 	topics.encoder_right, 	"/fmAInformation/encoder_right"	);

	local_node_handler.param<double>(		"max_velocity", 		parameters.max_velocity,	255		);
	local_node_handler.param<double>(		"wheel_diameter", 		parameters.wheel_diameter, 	0.01	);
	local_node_handler.param<double>(		"ticks_pr_round", 		parameters.ticks_pr_round, 	360		);
	local_node_handler.param<double>(		"ms_in_between", 		parameters.ms_in_between, 	100		);
	local_node_handler.param<double>(		"vel_publish_interval",	parameters.interval,		0.05	);
	local_node_handler.param<double>(		"vel_timeout",			parameters.timeout,			1		);

	/* m/s -> ticks/entry * 1 m/s = 1/(pi*wheel_diameter) rps = ticks_pr_round/(pi*wheel_diameter) ticks/sec = */
	vel_to_motor_const = ( parameters.ticks_pr_round / ( 3.14 * parameters.wheel_diameter ) ) * ( 1 / parameters.ms_in_between );

	subscribers.cmd_vel_left = 	global_node_handler.subscribe<geometry_msgs::TwistStamped>(topics.cmd_vel_left, 2,&FrobitInterface::on_vel_msg_left,this);
	subscribers.cmd_vel_right = global_node_handler.subscribe<geometry_msgs::TwistStamped>(topics.cmd_vel_right,2,&FrobitInterface::on_vel_msg_right,this);
	subscribers.deadman = 		global_node_handler.subscribe<std_msgs::Bool>(topics.deadman,2,&FrobitInterface::on_deadman,this);
	subscribers.nmea = 			global_node_handler.subscribe<msgs::nmea>(topics.nmea_sub,2,&FrobitInterface::on_encoder,this);

	publishers.nmea = 			global_node_handler.advertise<msgs::nmea>(topics.nmea_pub, 1);
	publishers.encoder_left = 	global_node_handler.advertise<msgs::encoder>(topics.encoder_left, 1);
	publishers.encoder_right = 	global_node_handler.advertise<msgs::encoder>(topics.encoder_right, 1);

	ros::Timer t1= global_node_handler.createTimer(ros::Duration(parameters.interval),&FrobitInterface::on_timer,this);

	ros::spin();
}

void FrobitInterface::on_vel_msg_left(const geometry_msgs::TwistStamped::ConstPtr& msg)
{
	messages.cmd_vel_left = *msg;
}

void FrobitInterface::on_vel_msg_right(const geometry_msgs::TwistStamped::ConstPtr& msg)
{
	messages.cmd_vel_right = *msg;
}

void FrobitInterface::on_deadman(const std_msgs::Bool::ConstPtr& msg)
{
	deadman = msg->data;
	last_deadman_received = ros::Time::now();
}

void FrobitInterface::on_encoder(const msgs::nmea::ConstPtr& msg)
{
	messages.encoder.header.stamp = msg->header.stamp;
	messages.encoder.encoderticks = boost::lexical_cast<int>(msg->data.at(1));
	publishers.encoder_left.publish(messages.encoder);

	messages.encoder.encoderticks = boost::lexical_cast<int>(msg->data.at(2));
	publishers.encoder_right.publish(messages.encoder);
}

void FrobitInterface::on_timer(const ros::TimerEvent& e)
{

	active = true;
	if((ros::Time::now() - messages.cmd_vel_left.header.stamp).toSec() > parameters.timeout)
	{
		ROS_WARN_THROTTLE(1,"Time for left cmd_vel is out of date");
		active = false;
	}

	if((ros::Time::now() - messages.cmd_vel_right.header.stamp).toSec() > parameters.timeout)
	{
		ROS_WARN_THROTTLE(1,"Time for right cmd_vel is out of date");
		active = false;
	}

	if( ros::Time::now() > last_deadman_received + ros::Duration(parameters.interval) )
	{
		deadman = false;
	}

	if(active && deadman)
	{
		left_vel = messages.cmd_vel_left.twist.linear.x * 100;
		right_vel = messages.cmd_vel_right.twist.linear.x * 100;

		//correct high velocities
		if ( left_vel > parameters.max_velocity )
			left_vel = parameters.max_velocity;
		else if ( left_vel < - parameters.max_velocity )
			left_vel = - parameters.max_velocity;

		if ( right_vel > parameters.max_velocity )
			right_vel = parameters.max_velocity;
		else if ( right_vel < - parameters.max_velocity )
			right_vel = - parameters.max_velocity;

		left_vel *= vel_to_motor_const;
		right_vel *= vel_to_motor_const;
	}
	else
	{
		left_vel = 0;
		right_vel = 0;
	}

	//build message
	messages.motor_command.header.stamp = ros::Time::now();
	messages.motor_command.type = "PFBCT";
	messages.motor_command.data.clear();
	messages.motor_command.data.push_back( boost::lexical_cast<std::string>( (int)right_vel ) );
	messages.motor_command.data.push_back( boost::lexical_cast<std::string>( (int)left_vel ) );

	//publish message
	publishers.nmea.publish(messages.motor_command);
}



