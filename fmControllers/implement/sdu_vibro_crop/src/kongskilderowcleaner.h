/*
 * kongskilderowcleaner.h
 *
 *  Created on: Aug 8, 2012
 *      Author: morl
 */

#ifndef KONGSKILDEROWCLEANER_H_
#define KONGSKILDEROWCLEANER_H_

#include <ros/ros.h>
#include <msgs/serial.h>
#include <sdu_vibro_crop/move_tool_simpleAction.h>
#include <actionlib/server/simple_action_server.h>
#include <boost/bind.hpp>

class kongskilde_rowcleaner {
public:
	kongskilde_rowcleaner(ros::NodeHandle& nn,ros::Duration max_move_time,bool invert);
	virtual ~kongskilde_rowcleaner();


	void on_timer(const ros::TimerEvent& e);
	void on_serial_rx(const msgs::serial::ConstPtr& msg);

	ros::Publisher serial_pub;

	bool sim;

private:

	void on_action_goal(const sdu_vibro_crop::move_tool_simpleGoalConstPtr& goal);
	void transmitStop();
	void transmitAction(int action);

	bool invert;
	actionlib::SimpleActionServer<sdu_vibro_crop::move_tool_simpleAction> as;
	msgs::serial tx_msg;

	ros::Duration maximum_move_time;

	sdu_vibro_crop::move_tool_simpleResult result;

	bool serial_detected;

};

#endif /* KONGSKILDEROWCLEANER_H_ */
