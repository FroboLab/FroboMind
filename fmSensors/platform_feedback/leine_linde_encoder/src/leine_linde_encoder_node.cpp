/*
 * ll_encoder_node.cpp
 *
 *  Created on: Feb 22, 2012
 *      Author: molar
 */

#include "LeineLindeEncoder.h"

#include <ros/ros.h>
#include <ros/console.h>
#include <msgs/can.h>
#include <msgs/encoder.h>

int main(int argc, char **argv)
{

  ros::init(argc, argv, "encoder_node");
  ros::NodeHandle nh("~");
  ros::NodeHandle n;

  ROS_INFO("Started!");

  LeineLindeEncoder ll;

  //returned objects needs to be stored
  ros::Timer t;
  ros::Subscriber sub;
  bool invert,read_offset;

  std::string enc_publisher_topic;
  std::string publisher_topic;
  std::string subscriber_topic;

  int encoder_id,publish_rate,poll_interval_ms;

  nh.param<std::string>("publisher_topic", publisher_topic, "/fmSensors/can0_tx");
  nh.param<std::string>("subscriber_topic", subscriber_topic, "/fmSensors/can0_rx");
  nh.param<std::string>("enc_publisher_topic", enc_publisher_topic, "/fmSensors/encoder");

  nh.param<int>("encoder_id", encoder_id, 11);
  nh.param<int>("publish_rate",publish_rate,10);
  nh.param<int>("poll_interval_ms",poll_interval_ms,20);
  nh.param<bool>("invert_output",invert,false);
  nh.param<bool>("use_current_position_as_offset",read_offset,true);

  ll.setID(encoder_id);

  ll.setPollInterval(poll_interval_ms);

  ll.invert = invert;
  ll.read_encoder_offset = read_offset;

  ll.setCanPub(nh.advertise<msgs::can> (publisher_topic.c_str(), 5));
  ll.setEncoderPub(nh.advertise<msgs::encoder> (enc_publisher_topic.c_str(), 5));
  sub = nh.subscribe<msgs::can> (subscriber_topic.c_str(), 100, &LeineLindeEncoder::processRXEvent, &ll);

  t= nh.createTimer(ros::Duration(1.0/publish_rate),&LeineLindeEncoder::processStateMachine,&ll);


  ROS_INFO("Running with: rate %d interval %d, enc_id: %d",publish_rate,poll_interval_ms,encoder_id);

  ros::spin();

  return 0;
}


