#include <csignal>
#include <cstdio>
#include "ros/ros.h"
#include "Bluetooth.h"


int main(int argc, char **argv)
{
  ros::init(argc, argv, "bluetooth_node");

  ros::NodeHandle n("~");
  ros::NodeHandle nh;

  std::string device,publisher_topic,subscriber_topic,addr;
  int term_rx,term_tx;

  n.param<std::string>("device", device, "bluetooth");
  n.param<std::string>("publisher_topic", publisher_topic, "bluetooth_rx");
  n.param<std::string>("subscriber_topic", subscriber_topic, "bluetooth_tx");
  n.param<std::string>("bluetooth_address",addr,"00:06:66:04:9E:1E");
  n.param<int> ("termination_character", term_rx,10);
  n.param<int> ("termination_character_tx", term_tx,10);

  BluetoothSerial bluetooth;

  bluetooth.term_char = (char)term_rx;
  bluetooth.term_char_tx = (char)term_tx;
  bluetooth.addr_str = addr;



  if(bluetooth.initInterface(device)!=0)
  {
	  ROS_ERROR("Could not open device: %s",device.c_str());
	  return -1;
  }
  else
  {
	  bluetooth.serial_rx_publisher_ = nh.advertise<msgs::serial>(publisher_topic.c_str(),1);
	  bluetooth.serial_tx_subscriber_ = nh.subscribe<msgs::serial>(subscriber_topic.c_str(),20,&BluetoothSerial::processSerialTxEvent,&bluetooth);
	  ros::spin();
  }

  return 0;
}
