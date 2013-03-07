/*****************************************************************************
# NMEA node
# Copyright (c) 2012-2013,
#	Leon Bonde Larsen <leon@bondelarsen.d>
#	Kjeld Jensen <kjeld@frobomind.org>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#    * Redistributions of source code must retain the above copyright
#      notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above copyright
#      notice, this list of conditions and the following disclaimer in the
#      documentation and/or other materials provided with the distribution.
#    * Neither the name FroboMind nor the
#      names of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#*****************************************************************************
#
# This node reads NMEA messages from a serial stream, decodes the information
# and verifys the checksum.
#
# 2012-xx-xx lelar original code
# 2013-03-04 kjen  bugfix, more robust error handling,
#                  added length to nmea msg, checksum is validated on
#                  incoming msgs regardles of use_nmea_checksum value
#                  OUTGOING CHECKSUM NOT YET IMPLEMENTED
#
#****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <sstream>
#include <vector>
#include "ros/ros.h"
#include "msgs/nmea.h"
#include "msgs/serial.h"
#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/algorithm/string.hpp"

#define ASCII_OFFSET '0'

ros::Publisher str_to_msg_pub;
ros::Publisher msg_to_str_pub;
msgs::nmea nmea_msg;
bool use_checksum;


typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

/* Parses from a string into two hex characters */
int atox(const char *s) {
	int ret = 0;
	int cnt = 8;
	while (cnt) {
		if ((*s >= '0') && (*s <= '9')) {
			ret <<= 4;
			ret += (*s - '0');
			cnt--;
			s++;
			continue;
		}
		if ((*s >= 'a') && (*s <= 'f')) {
			ret <<= 4;
			ret += (*s - 'a' + 10);
			cnt--;
			s++;
			continue;
		}
		if ((*s >= 'A') && (*s <= 'F')) {
			ret <<= 4;
			ret += (*s - 'A' + 10);
			cnt--;
			s++;
			continue;
		}
		break;
	}
	return ret;
}

/* Calculates expected checksum and compares to contained checksum */
bool verify_checksum(std::vector<std::string> tokens) {
	//Setup proper string
	std::string temp_str = "0x";
	temp_str.append(tokens.at(tokens.size() - 1).c_str());

	unsigned char expected_checksum = atox(
			tokens.at(tokens.size() - 1).c_str());
	unsigned char checksum = 0;
	unsigned char *string_breakdown;

	//Calculate check sum
	for (unsigned int i = 0; i < tokens.size() - 1; i++) {
		string_breakdown = (unsigned char*) tokens.at(i).c_str();

		for (unsigned int j = 0; j < tokens.at(i).length(); j++)
			if (string_breakdown[j] != '$')
				checksum ^= string_breakdown[j];

		//Remember the ','
		if (i < (tokens.size() - 2))
			checksum ^= ',';
	}

	return (checksum == expected_checksum);
}

/* Parses from serial message to nmea message */
void str_to_msg_callback(const msgs::serial::ConstPtr& msg) {
	std::string nmeastr(msg->data);

	if (msg->data[0] == '$')
	{	
		//build NMEA message data type
		nmea_msg.header.stamp = ros::Time::now();
		nmea_msg.type.clear();
		nmea_msg.data.clear();
		nmea_msg.length = 0;
		nmea_msg.valid = false;

		// test if checksum is appended to the received message
		bool checksum_appended = nmeastr.find("*");

		boost::char_separator<char> sep("$,*\r");
		tokenizer tokens(nmeastr, sep);
		std::vector<std::string> nmea;
		nmea.assign(tokens.begin(), tokens.end());
		if (nmea.size() >= 3)
		{
			nmea_msg.type = nmea.at(0);
			if(checksum_appended == true)
			{
				int i;
				for (i = 1 ; i < nmea.size()-1 ; i++ )
					nmea_msg.data.push_back( nmea.at(i).c_str() );
				nmea_msg.length = i-1; // number of data values

				if (!verify_checksum(nmea))
				{
					ROS_WARN("NMEA string discarded due to faulty checksum");
				}
				else
					nmea_msg.valid = true;
			}
			else
			{
				int i;
				for (i = 1 ; i < nmea.size() ; i++ )
					nmea_msg.data.push_back( nmea.at(i).c_str() );
				nmea_msg.length = i; // number of data values
				nmea_msg.valid = true;
			}
		}
		else
			ROS_WARN("NMEA string discarded due to bogus content");

		str_to_msg_pub.publish(nmea_msg); //publish message
	}
	else
		ROS_WARN("NMEA string does not contain $");
}

/* Parses from  nmea message to serial message */
void msg_to_str_callback(const msgs::nmea::ConstPtr& msg) {
	std::vector<std::string> data_list;
	std::vector<std::string> nmea_list;
	std::string data_string;
	msgs::serial nmea_string;

	//construct data string
	for(int i = 0 ; i < msg->data.size() ; i++ )
		data_list.push_back(msg->data[i]);
	data_string = boost::algorithm::join(data_list, ",");

	//construct nmea string
	nmea_list.push_back("$");
	nmea_list.push_back(msg->type);
	nmea_list.push_back(",");
	nmea_list.push_back(data_string);
	
	if (use_checksum == true)
	{
		// TODO: Add support for optional checksum on transmitted NMEA messages

	//	nmea_list.push_back("*"); 
	//	nmea_list.push_back("00");
	}
	nmea_list.push_back("\r\n");

	nmea_string.header.stamp = msg->header.stamp;
	nmea_string.data = boost::algorithm::join(nmea_list,"");

	msg_to_str_pub.publish(nmea_string);
}


/* sets up nmea parser node */
int main(int argc, char **argv) {
	ros::init(argc, argv, "nmea_parser");
	ros::NodeHandle nh;
	ros::NodeHandle n("~");

	std::string str_to_msg_sub_id;
	std::string str_to_msg_pub_id;
	std::string msg_to_str_sub_id;
	std::string msg_to_str_pub_id;

	n.param<std::string>("str_to_msg_sub", str_to_msg_sub_id,
			"/fmCSP/S0_rx_msg");
	n.param<std::string>("str_to_msg_pub", str_to_msg_pub_id,
			"/nmea_in");
	n.param<std::string>("msg_to_str_sub", msg_to_str_sub_id,
			"/nmea_out");
	n.param<std::string>("msg_to_str_pub", msg_to_str_pub_id,
			"/fmCSP/S0_tx_msg");
	n.param<bool>("use_nmea_checksum" , use_checksum , false);

	ros::Subscriber str_to_msg_sub = nh.subscribe(str_to_msg_sub_id, 10, str_to_msg_callback);
	str_to_msg_pub = nh.advertise<msgs::nmea>(str_to_msg_pub_id, 1);

	ros::Subscriber msg_to_str_sub = nh.subscribe(msg_to_str_sub_id, 10, msg_to_str_callback);
	msg_to_str_pub = nh.advertise<msgs::serial>(msg_to_str_pub_id, 1);

	ros::spin();
	return 0;
}

