#ifndef HBL2350_HPP_
#define HBL2350_HPP_

#include <ros/ros.h>
#include "roboteq/roboteq.hpp"
#include <geometry_msgs/TwistStamped.h>
#include <std_msgs/Bool.h>
#include <msgs/StringStamped.h>
#include <msgs/IntStamped.h>
#include <cstdarg>

class hbl2350 : public RoboTeQ
{
private:
	ros::NodeHandle 		local_node_handler;
	ros::NodeHandle 		global_node_handler;

	bool 					deadman_active,
							cmd_vel_active,
							initialised,
							idle;

	int 					velocity_ch1,
							velocity_ch2;

	std_msgs::Bool 			prev_joy;

	ros::Time 				last_deadman_received;
	ros::Time 				last_twist_received_ch1;
	ros::Time 				last_twist_received_ch2;

public:
	int						p_gain_ch1,
							i_gain_ch1,
							d_gain_ch1,
							p_gain_ch2,
							i_gain_ch2,
							d_gain_ch2,
							max_rpm,
							anti_windup_percent,
							max_acceleration,
							max_deceleration,
							velocity_max;

	double 					mps_to_rpm;

	ros::Subscriber 		serial_sub,
							cmd_vel_ch1_sub,
							cmd_vel_ch2_sub,
							deadman_sub;

	ros::Duration 			max_time_diff;

							hbl2350();
	void					spin(void);
	void 					initController(void);

	void					onCmdVelCh2(const geometry_msgs::TwistStamped::ConstPtr&);
	void					onCmdVelCh1(const geometry_msgs::TwistStamped::ConstPtr&);
	void					onDeadman(const std_msgs::Bool::ConstPtr&);
	void					onTimer(const ros::TimerEvent&);
	void 					onSerial(const msgs::serial::ConstPtr& msg){serialCallback(msg);}

	void					setSerialPub(ros::Publisher pub){serial_publisher = pub;}
	void					setStatusPub(ros::Publisher pub){status_publisher = pub;}
	void					setTemperaturePub(ros::Publisher pub){temperature_publisher = pub;}
	void					setPowerCh1Pub(ros::Publisher pub){power_ch1_publisher = pub;}
	void					setPowerCh2Pub(ros::Publisher pub){power_ch2_publisher = pub;}
	void					setEncoderCh1Pub(ros::Publisher pub){encoder_ch1_publisher = pub;}
	void					setEncoderCh2Pub(ros::Publisher pub){encoder_ch2_publisher = pub;}

	int						subscribers(){return serial_publisher.getNumSubscribers();}



};

#endif /* HBL2350_HPP_ */
