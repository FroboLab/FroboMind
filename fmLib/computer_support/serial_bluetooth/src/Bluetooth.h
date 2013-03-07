/*
 * Bluetooth.h
 *
 *  Created on: Mar 26, 2012
 *      Author: morl
 */

#ifndef BLUETOOTH_H_
#define BLUETOOTH_H_

#include <ros/ros.h>
#include <ros/console.h>

#include <msgs/serial.h>

#include <iostream>
#include <string>
#include <stdint.h>
#include <boost/thread.hpp>
#include <boost/asio.hpp>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>


class BluetoothSerial
{
public:
	BluetoothSerial();
	virtual ~BluetoothSerial();

	int initInterface(std::string interface);
	void processSerialTxEvent(const msgs::serial::ConstPtr& msg);

	ros::Publisher serial_rx_publisher_;
	ros::Subscriber serial_tx_subscriber_;

	char term_char;
	char term_char_tx;

	std::string addr_str;


private:
	void readSome();
	void RxHandler(const boost::system::error_code& error, size_t bytes_transferred);

	boost::asio::io_service ioService_;
	boost::asio::posix::stream_descriptor descriptor_;

	msgs::serial serial_rx_msg_;

	boost::asio::streambuf readbuffer;


	int s;
};

#endif /* BLUETOOTH_H_ */
