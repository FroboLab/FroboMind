/*
 * RoboTeQ.hpp
 *
 *  Created on: Jan 26, 2013
 *      Author: leonbondelarsen
 */

#ifndef ROBOTEQ_HPP_
#define ROBOTEQ_HPP_

#include <ros/ros.h>
#include <iostream>
#include <msgs/serial.h>
#include <msgs/IntStamped.h>
#include <msgs/StringStamped.h>

class RoboTeQ
{
public:
	bool					online;

	unsigned int			ff,fs;

	int						cb1,cb2,
							cbr1,cbr2,
							f1,f2,
							a1,a2,
							ba1,ba2,
							bs1,bs2,
							bsr1,bsr2,
							e1,e2,
							p1,p2,
							t1,t2,t3,
							v1,v2,v3;

	msgs::serial 			serial_out;

	msgs::IntStamped 		encoder_out,
							power_out,
							temperature_out;

	msgs::StringStamped	status_out;

	ros::Time 				last_serial_msg;

	ros::Publisher 			serial_publisher,
							status_publisher,
							power_ch1_publisher,
							power_ch2_publisher,
							encoder_ch1_publisher,
							encoder_ch2_publisher,
							temperature_publisher;

	std::string 			statusFlagsToString(unsigned int);
	std::string 			faultFlagsToString(unsigned int);

							RoboTeQ();

	void 					serialCallback(const msgs::serial::ConstPtr&);

	void					transmit(int, std::string , ...);
};

#endif /* ROBOTEQ_HPP_ */
