/*
 * Bluetooth.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: morl
 *
 *      needs: sudo apt-get install libbluetooth-dev
 */

#include "Bluetooth.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include <bluetooth/bluetooth.h>
#include <bluetooth/rfcomm.h>


BluetoothSerial::BluetoothSerial() : descriptor_(ioService_)
{

}

BluetoothSerial::~BluetoothSerial()
{
	// TODO Auto-generated destructor stub
	close(s);
}

int BluetoothSerial::initInterface(std::string interface)
{
	struct sockaddr_rc addr;



	// allocate a socket
	s = socket(AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM);
	std::cout << "using address " << addr_str << std::endl;
	// set the connection parameters (who to connect to)
	addr.rc_family = AF_BLUETOOTH;
	addr.rc_channel = (uint8_t) 1;
	str2ba( addr_str.c_str(), &addr.rc_bdaddr );

	// connect to server
	int status = connect(s, (struct sockaddr *)&addr, sizeof(addr));
	if(status == 0)
	{
		descriptor_.assign(s);
		readSome();

		boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService_));
		return 0;
	}
	else
	{
		perror("aw.. snap");
		std::cout << "got status" << status << std::endl;
		return 1;
	}
}

void BluetoothSerial::processSerialTxEvent(const msgs::serial::ConstPtr& msg)
{

//    if (descriptor_.is_open())
//    {
    	descriptor_.write_some(
                            boost::asio::buffer( msg->data.c_str() , msg->data.length()));
//            if ( term_char_tx )
//            {
            	descriptor_.write_some( boost::asio::buffer( &term_char_tx , 1 ) );
//            }
//    }
}

void BluetoothSerial::RxHandler(const boost::system::error_code& error, size_t bytes_transferred)
{

	if (bytes_transferred)
	{

		std::istream is(&readbuffer);
		char line[128];

		// only read until bytes_transferred to avoid reading after \n
		// if the streambuffer contains a second line boost should call us again
		is.getline(line,bytes_transferred,term_char);

		/* publish data ro ros */
		++serial_rx_msg_.header.seq;
		serial_rx_msg_.data = line;
		serial_rx_msg_.header.stamp = ros::Time::now();
		serial_rx_publisher_.publish(serial_rx_msg_);
	}
	BluetoothSerial::readSome();
}

void BluetoothSerial::readSome()
{
	boost::asio::async_read_until(descriptor_, readbuffer, term_char,
			boost::bind(&BluetoothSerial::RxHandler, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}
