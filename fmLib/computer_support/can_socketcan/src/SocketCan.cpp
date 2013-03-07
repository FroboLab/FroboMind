/*
 * SocketCan.cpp
 *
 *  Created on: Mar 26, 2012
 *      Author: morl
 */

#include "SocketCan.h"

#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>


SocketCan::SocketCan() : descriptor_(ioService_)
{

}

SocketCan::~SocketCan()
{
	// TODO Auto-generated destructor stub
	close(s);
}

int SocketCan::initInterface(std::string interface)
{
	struct sockaddr_can addr;
	struct ifreq ifr;
	int ret;

	/* open socket */
	if ((s = socket(PF_CAN, SOCK_RAW, CAN_RAW)) < 0) {
		ROS_ERROR("Cannot create CAN socket err: %d",s);
		return -1;
	}

	addr.can_family = AF_CAN;

	strcpy(ifr.ifr_name, interface.c_str());
	if ((ret = ioctl(s, SIOCGIFINDEX, &ifr)) < 0) {
		ROS_ERROR("Error setting interface name: %d",ret);
		return -1;
	}
	addr.can_ifindex = ifr.ifr_ifindex;

	if ((ret = bind(s, (struct sockaddr *)&addr, sizeof(addr))) < 0) {
		ROS_ERROR("Error Binding to socket: %d",ret);
		return -1;
	}

	descriptor_.assign(s);

	canReadSome();

    boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService_));

	return 0;
}

void SocketCan::processCanTxEvent(const msgs::can::ConstPtr& msg)
{
	can_frame tx;

	tx.can_id = msg->id;
	// XXX
	// Compatability with can4linux flags which are used in msgs::can
	// bit 2 is EFF which means we have to set bit 31 in the id flag
	// other flags might become relevant also but currently only this flag is supported.
	// XXX
	if(msg->flags & 0x04)
	{
		tx.can_id = msg->id;
	}
	tx.can_dlc = msg->length;

	for(int i=0; i<msg->length; i++)
	{
		tx.data[i] = msg->data[i];
	}
	try
	{
	 boost::asio::write(descriptor_, boost::asio::buffer(&tx, sizeof(tx)));
	}
	catch (boost::system::system_error &e)
	{
		ROS_ERROR(e.what());
	}
}

void SocketCan::canRxHandler(const boost::system::error_code& error, size_t bytes_transferred)
{
	// XXX
	// Compatability with msgs::can structure which means that the id in the can msg does not contain eff flags
	// this bit is removed from the id before publishing this message, instead bit 2 is set in flags
	// XXX
    can_rx_msg_.header.stamp = ros::Time::now();
    if (bytes_transferred)
    {
      // move bit 31 of id downto bit 2 of flags
      can_rx_msg_.flags = (rx_.can_id & (1 << 31)) >> 29;
      can_rx_msg_.cob = rx_.can_id & 0x1FFFFFFF; // mask out EFF/RTR/ERR flags from id
      can_rx_msg_.id = rx_.can_id & 0x1FFFFFFF;
      can_rx_msg_.length = rx_.can_dlc;
      for (int i = 0; i <rx_.can_dlc; i++)
      {
        can_rx_msg_.data[i] = rx_.data[i];
      }
      can_rx_publisher_.publish(can_rx_msg_);
    }
    canReadSome();
}

void SocketCan::canReadSome()
{
	descriptor_.async_read_some(boost::asio::buffer(&rx_, sizeof(rx_)),
			boost::bind(&SocketCan::canRxHandler, this,
					boost::asio::placeholders::error,
					boost::asio::placeholders::bytes_transferred));
}


