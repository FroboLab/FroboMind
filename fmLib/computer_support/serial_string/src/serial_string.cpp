/*************************************************************************************
# Copyright (c) 2011, Søren Hundevadt Nielsen
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#    This product includes software developed by the University of Southern Denmark.
# 4. Neither the name of the University of Southern Denmark nor the
#    names of its contributors may be used to endorse or promote products
#    derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY SØREN HUNDEVADT NIELSEN "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL SØREN HUNDEVADT NIELSEN BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************************
# File:     serial_node.cpp                                            
# Purpose:  Create a interface node to handle serial communication
# Project:  vic_interfaces
# Author:   Søren Hundevadt Nielsen <soeni05@gmail.com>
# Created:  Apr 29, 2011 Søren Hundevadt Nielsen, Source written
*************************************************************************************/
#include <string>

#include "ros/ros.h"
#include "msgs/serial.h"

#include "serialInterface.h"

int main(int argc, char **argv)
{
  /* ros messages */
	msgs::serial s_rx_msg_;
	msgs::serial s_tx_msg_;

  /* parameters */
  std::string device;
  std::string publisher_topic;
  std::string subscriber_topic;
  int baudrate;
  int term;

  /* initialize ros usage */
  ros::init(argc, argv, "vic_serial_interface");
  ros::Publisher s_publisher;
  ros::Subscriber s_subscriber;

  /* private nodehandlers */
  ros::NodeHandle nh;
  ros::NodeHandle n("~");

  /* read parameters from ros parameter server if available otherwise use default values */
  n.param<std::string> ("device", device, "/dev/ttyS0");
  n.param<std::string> ("publisher_topic", publisher_topic, "S0_rx_msg");
  n.param<std::string> ("subscriber_topic", subscriber_topic, "S0_tx_msg");
  n.param<int> ("termination_character", term,10);
  n.param<int> ("baudrate", baudrate, 115200);

  s_publisher = nh.advertise<msgs::serial> (publisher_topic.c_str(), 20,1);

  serialInterface serialInterface(s_publisher);
  serialInterface.term_char = (char)term;
  serialInterface.openDevice(device, baudrate);


  s_subscriber = nh.subscribe<msgs::serial> (subscriber_topic.c_str(), 20, &serialInterface::writeHandler, &serialInterface);

  ros::spin();

  return 0;
}

