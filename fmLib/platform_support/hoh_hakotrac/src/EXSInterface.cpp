/*
 * EXSInterface.cpp
 *
 *  Created on: Sep 23, 2012
 *      Author: morl
 */

#include "EXSInterface.h"
#include <cmath>

EXSInterface::EXSInterface()
{
	// TODO Auto-generated constructor stub
	steering_angle_updated = false;
	cmd_vel_updated = false;

	steering_angle_rad = 0;
	steering_angle_to_int = 20;
	cmd_vel_ms = 0;
	cmd_vel_to_int = 20;

	last_heartbeat_tx = ros::Time::now();

	deadman_active =false;
	wii_watchdog_count = 0;
	wii_watchdog_limit = 30;

	x_prev = y_prev = z_prev = 0 ;

	last_wii = ros::Time::now();
	last_steering = ros::Time::now();
}

EXSInterface::~EXSInterface()
{
	// TODO Auto-generated destructor stub
}

void EXSInterface::onCANMsg(const msgs::can::ConstPtr& msg)
{
	int16_t angle = 0;
	switch (msg->id) {
		case can_id_rx_t::CAN_ID_HAKO_IO:
			break;
		case can_id_rx_t::CAN_ID_HAKOSTATE:
			break;
		case can_id_rx_t::CAN_ID_GEAR_AND_POWER:
			break;
		case can_id_rx_t::CAN_ID_HORN_ACK:
			ROS_DEBUG_NAMED("HORN","Received ack msg");
			break;
		case can_id_rx_t::CAN_ID_CVT_ACK:
			break;
		case can_id_rx_t::CAN_ID_ODOM:
			// XXX: unpack info from can msg
			enc_msg.encoderticks = (msg->data[0] | (msg->data[1] << 8) | (msg->data[2] << 16) | (msg->data[3] << 24));
			enc_msg.header.stamp = msg->header.stamp;
			encoder_pub.publish(enc_msg);
			break;
		case can_id_rx_t::CAN_ID_STEERING_ACK:
			break;
		case can_id_rx_t::CAN_ID_STEERING_ANGLE_ACK:
			angle = msg->data[0] | (msg->data[1] << 8);
			angle_msg.encoderticks = angle;
			angle_msg.header.stamp = msg->header.stamp;
			angle_pub.publish(angle_msg);
			break;
		case can_id_rx_t::CAN_ID_STEERING_REPORT:
			break;
		case can_id_rx_t::CAN_ID_SWITCH_ACK:
			break;
		default:
			ROS_DEBUG_NAMED("UnknownCan","Received unknown frame with id: %d",msg->id);
			break;
	}
}

void EXSInterface::onTimer(const ros::TimerEvent& e)
{
	ros::Time t = ros::Time::now();

	if((t - last_wii) > ros::Duration(1))
	{
		deadman_active = false;
	}

	if(deadman_active == false)
	{
		ROS_ERROR_THROTTLE(1,"Deadman not active for Hako sending safe cvt and zero steering");
	}

	if(steering_angle_updated && (t - last_steering) > ros::Duration(0.2))
	{
		last_steering = t;
		steering_angle_updated =  false;

		can_msg.id = can_id_tx.CAN_ID_STEERING_ANGLE_CMD;
		can_msg.length = 2;

		/* transimt angle in deci-degrees */
		int16_t angle = (steering_angle_rad/(2*M_PI) * (360)) * 10;
		if(deadman_active)
		{
			can_msg.data[0] = angle;
			can_msg.data[1] = angle >> 8;
		}
		else
		{
			can_msg.data[0] = 0;
			can_msg.data[1] = 0;
		}

		can_tx_pub.publish(can_msg);
	}

	if(cmd_vel_updated)
	{
		cmd_vel_updated = false;

		can_msg.id = can_id_tx.CAN_ID_CVT_CONTROL_CMD;
		can_msg.length = 4;

		// convert m/s to um/s (micrometer per second)
		if(deadman_active)
		{
			int32_t speed_ums = cmd_vel_ms * 1000000;
			can_msg.data[0] = speed_ums;
			can_msg.data[1] = speed_ums >> 8;
			can_msg.data[2] = speed_ums >> 16;
			can_msg.data[3] = speed_ums >> 24;

			can_msg.data[3] &= 0x3f;
			can_msg.data[3] |= 0xc0;
		}
		else
		{
			can_msg.data[0] = 0;
			can_msg.data[1] = 0;
			can_msg.data[2] = 0;
			can_msg.data[3] = 0x40;
		}

		can_tx_pub.publish(can_msg);
	}





	if((t - last_odom_poll_msg).toSec() > 0.1)
	{
		last_odom_poll_msg = t;
		can_msg.id = can_id_tx.CAN_ID_ENCODER_REQ;
		can_msg.length = 2;
		can_msg.data[0] = can_msg.data[1] = 0;

		can_tx_pub.publish(can_msg);
	}


	if( (t - last_heartbeat_tx).toSec() > 0.1 )
	{
		last_heartbeat_tx = t;
		// transmit heartbeat
		can_msg.id = can_id_tx.CAN_ID_HEARTBEAT;
		can_msg.length = 1;
		can_msg.data[0] = 0xA5;
		can_tx_pub.publish(can_msg);
	}
}

void EXSInterface::onSteeringAngle(
		const msgs::steering_angle_cmd::ConstPtr& msg)
{
	steering_angle_updated = true;
	steering_angle_rad = msg->steering_angle;

}

void EXSInterface::onCmdVel(const geometry_msgs::Twist::ConstPtr& msg)
{
	cmd_vel_updated = true;
	cmd_vel_ms = msg->linear.x;
}

void EXSInterface::onJoy(const sensor_msgs::Joy::ConstPtr& msg)
{
	if ( ((msg-> axes[0]) == x_prev) && ((msg -> axes[1]) == y_prev) && ((msg -> axes[2]) == z_prev))
	{

		wii_watchdog_count++;

		if (wii_watchdog_count > wii_watchdog_limit)
		{
			  deadman_active = false;
		}
	}
	else
	{
		wii_watchdog_count = 0;
		if(msg->buttons[3])
		{
			deadman_active = true;
		}
		else
		{
			deadman_active = false;
		}
	}

	x_prev = msg -> axes[0];
	y_prev = msg -> axes[1];
	z_prev = msg -> axes[2];

	last_wii = msg->header.stamp;
}

void EXSInterface::onRPMCmd(const msgs::engine_rpm::ConstPtr& msg)
{


	uint32_t rpm_mhz = ((double)msg->rpm)/60.0 * 1000;

	can_msg.id = can_id_tx.CAN_ID_RPM_CMD;
	can_msg.length = 8;
	can_msg.data[0] = 0xC5;
	can_msg.data[1] = can_msg.data[2] = can_msg.data[3] = 0;
	can_msg.data[4] = rpm_mhz;
	can_msg.data[5] = rpm_mhz >> 8;
	can_msg.data[6] = rpm_mhz >> 16;
	can_msg.data[7] = rpm_mhz >> 24;

	can_tx_pub.publish(can_msg);
}
