/*
 * kongskilderowcleaner.cpp
 *
 *  Created on: Aug 8, 2012
 *      Author: morl
 */

#include "kongskilderowcleaner.h"


kongskilde_rowcleaner::kongskilde_rowcleaner(ros::NodeHandle& nn,ros::Duration max_move_time,bool invert) :
as(nn,"move_tool", boost::bind(&kongskilde_rowcleaner::on_action_goal, this, _1), false)
{
	this->invert = invert;
	maximum_move_time = max_move_time;
	as.start();
	serial_detected = false;
}

kongskilde_rowcleaner::~kongskilde_rowcleaner() {
	// TODO Auto-generated destructor stub
}

void kongskilde_rowcleaner::on_timer(const ros::TimerEvent& e)
{
	if(serial_detected)
	{
		tx_msg.data = "a\n";
		// transmit keep alive signal
		serial_pub.publish(tx_msg);
	}

}

void kongskilde_rowcleaner::on_serial_rx(const msgs::serial::ConstPtr& msg)
{
	if(msg->data[0] == 'a')
	{
		ROS_INFO_THROTTLE(1,"Alive: status %s",msg->data.c_str());
		serial_detected = true;
	}
	else if(msg->data[0] == 'n')
	{
		ROS_INFO_THROTTLE(1,"RoboCard detected: beginning to transmit alive signal");
		serial_detected = true;
	}
	else
	{
		ROS_INFO_THROTTLE(1,"Not Alive: communication is there but we are not reading alive status");
		serial_detected = false;
	}
}

void kongskilde_rowcleaner::on_action_goal(
		const sdu_vibro_crop::move_tool_simpleGoalConstPtr& goal)
{
	ros::Rate r(10);
	bool success=false;
	ros::Time started = ros::Time::now();

	maximum_move_time = ros::Duration(goal->timeout);


	while(!success)
	{
		if(as.isPreemptRequested() || !ros::ok())
		{
			ROS_INFO("Goal is preempted");
			// action is preempted
			transmitStop();
			as.setPreempted(result,"Preempt requested");
			break;
		}
		else
		{


			if(!serial_detected && sim==false)
			{
				transmitStop();
				as.setAborted(result,"No communication with robocard");
				break;
			}
			else if((ros::Time::now() - started) > maximum_move_time )
			{
				transmitStop();
				as.setSucceeded(result,"Move completed");
				success = true;
				break;
			}
			else
			{
				transmitAction(goal->direction);
			}
		}
		r.sleep();
	}
	transmitStop();
}

void kongskilde_rowcleaner::transmitStop()
{
	tx_msg.data = "s\n";
	serial_pub.publish(tx_msg);
}

void kongskilde_rowcleaner::transmitAction(int action)
{

	if(action == sdu_vibro_crop::move_tool_simpleGoal::UP)
	{
		if( this->invert)
		{
			tx_msg.data = "d\ne\n";
		}
		else
		{
			// transmit enable and up
			tx_msg.data = "u\ne\n";
		}
		serial_pub.publish(tx_msg);
	}
	else if (action == sdu_vibro_crop::move_tool_simpleGoal::DOWN)
	{
		if(this->invert)
		{
			tx_msg.data = "u\ne\n";
		}
		else
		{
			tx_msg.data = "d\ne\n";
		}
		serial_pub.publish(tx_msg);
	}
}



