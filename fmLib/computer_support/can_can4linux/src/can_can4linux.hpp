#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <boost/system/system_error.hpp>
#include <boost/thread.hpp>
#include "ros/ros.h"
#include <can4linux.h>
#include "msgs/can.h"

class VicCan
{

private:

  int fd_;

  canmsg_t rx_;
  msgs::can can_rx_msg_;

  ros::NodeHandle nh_;

  boost::asio::io_service ioService_;
  boost::asio::posix::stream_descriptor descriptor_;

  ros::Publisher can_rx_publisher_;
  ros::Subscriber can_tx_subscriber_;

  std::string device_;
  std::string publisher_topic_;
  std::string subscriber_topic_;

  void canHandler(const boost::system::error_code& error, size_t bytes_transferred)
  {
    can_rx_msg_.header.stamp = ros::Time::now();
    if (bytes_transferred)
    {
      can_rx_msg_.flags = rx_.flags;
      can_rx_msg_.cob = rx_.cob;
      can_rx_msg_.id = rx_.id;
      can_rx_msg_.length = rx_.length;
      for (int i = 0; i < 8; i++)
      {
        can_rx_msg_.data[i] = rx_.data[i];
      }
      can_rx_publisher_.publish(can_rx_msg_);
    }
    canReadSome();
  }

  void canReadSome()
  {
    descriptor_.async_read_some(boost::asio::buffer(&rx_, 1), boost::bind(&VicCan::canHandler, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
  }

public:

  VicCan() :  descriptor_(ioService_)
  {
  }

  ~VicCan()
  {
    close(fd_);
  }

  bool init(std::string device){

    ros::NodeHandle n("~");

    n.param<std::string>("device", device_, "/dev/can0");
    n.param<std::string>("publisher_topic", publisher_topic_, "can_rx");
    n.param<std::string>("subscriber_topic", subscriber_topic_, "can_tx");

    can_rx_publisher_ = nh_.advertise<msgs::can> (publisher_topic_.c_str(), 1);
    can_tx_subscriber_ = nh_.subscribe(subscriber_topic_.c_str(), 1, &VicCan::write, this);

    ROS_INFO("%s", device_.c_str());

    if ((fd_ = open(device_.c_str(), O_RDWR )) < 0)
    {
      ROS_ERROR("Error opening CAN device %s\n", device_.c_str());
      return 0;
    }
    descriptor_.assign(fd_);
    canReadSome();

    // run the IO service as a separatestd::string host; thread, so the main thread can do others
    boost::thread t(boost::bind(&boost::asio::io_service::run, &ioService_));

    return 1;
  }

  void write(const msgs::can::ConstPtr& msg)
  {
	//ROS_INFO("write");

    canmsg_t tx_;

    tx_.flags = msg->flags;
    tx_.cob = msg->cob;
    tx_.id = msg->id;
    tx_.length = msg->length;
    for (int i = 0; i < 8; i++)
    {
      tx_.data[i] = msg->data[i];
    }
    boost::asio::write(descriptor_, boost::asio::buffer(&tx_, 1));
  }
};
