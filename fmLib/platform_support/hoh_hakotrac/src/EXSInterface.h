/*
 * EXSInterface.h
 *
 *  Created on: Sep 23, 2012
 *      Author: morl
 */

#ifndef EXSINTERFACE_H_
#define EXSINTERFACE_H_

#include <ros/ros.h>
#include <msgs/can.h>
#include <msgs/steering_angle_cmd.h>
#include <geometry_msgs/Twist.h>
#include <msgs/encoder.h>
#include <msgs/engine_rpm.h>
#include <sensor_msgs/Joy.h>


typedef struct
{

	/*
	 * IDs on can 1
	 * */
	static const uint32_t CAN_ID_HAKO_IO = 0x105;
	static const uint32_t CAN_ID_HAKOSTATE = 0x50;
	static const uint32_t CAN_ID_GEAR_AND_POWER = 0x145;

	/*
	 * Frame ids on can 2
	 */
	static const uint32_t CAN_ID_ODOM = 0x100;
	static const uint32_t CAN_ID_STEERING_REPORT = 0x408;
	static const uint32_t CAN_ID_STEERING_ANGLE_ACK = 0x75A;
	static const uint32_t CAN_ID_STEERING_ACK = 0x758;
	static const uint32_t CAN_ID_CVT_ACK = 0x750;
	static const uint32_t CAN_ID_HORN_ACK = 0x740;
	static const uint32_t CAN_ID_SWITCH_ACK = 0x730;

}can_id_rx_t;

typedef struct
{
	static const uint32_t CAN_ID_HORN_CMD = 0x610;
	static const uint32_t CAN_ID_CVT_CONTROL_CMD = 0x080;
	static const uint32_t CAN_ID_STEERING_ANGLE_CMD = 0x142;
	static const uint32_t CAN_ID_STEERING_REPORT_REQ = 0x408;
	static const uint32_t CAN_ID_ENCODER_REQ = 0x110; // only for can device 2??
	static const uint32_t CAN_ID_RPM_CMD = 0x200;
	static const uint32_t CAN_ID_CURVATURE_CMD = 0x141;
	static const uint32_t CAN_ID_HEARTBEAT = 0x070;

}can_id_tx_t;

class EXSInterface
{
public:

	EXSInterface();
	virtual ~EXSInterface();

	void onCANMsg(const msgs::can::ConstPtr & msg);
	void onTimer(const ros::TimerEvent& e);

	void onSteeringAngle(const msgs::steering_angle_cmd::ConstPtr & msg);
	void onCmdVel(const geometry_msgs::Twist::ConstPtr& msg);
	void onJoy(const sensor_msgs::Joy::ConstPtr& msg);
	void onRPMCmd(const msgs::engine_rpm::ConstPtr& msg);

	ros::Publisher can_tx_pub;
	ros::Publisher encoder_pub;
	ros::Publisher angle_pub;

private:

	static const can_id_rx_t can_id_rx;
	static const can_id_tx_t can_id_tx;

	msgs::encoder enc_msg;
	msgs::encoder angle_msg;
	msgs::can can_msg;

	double steering_angle_rad;
	double steering_angle_to_int;
	double cmd_vel_ms;
	double cmd_vel_to_int;

	bool cmd_vel_updated;
	bool steering_angle_updated;

	ros::Time last_heartbeat_tx;
	ros::Time last_odom_poll_msg;

	double x_prev,y_prev,z_prev;

	unsigned int wii_watchdog_count;
	unsigned int wii_watchdog_limit;

	bool deadman_active;

	ros::Time last_wii;

	ros::Time last_steering;



};

#endif /* EXSINTERFACE_H_ */
