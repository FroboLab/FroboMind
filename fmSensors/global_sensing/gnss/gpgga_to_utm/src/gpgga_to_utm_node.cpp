/****************************************************************************
# FroboMind gpgga_to_utm
# Copyright (c) 2011-2013, editor Kjeld Jensen <kjeld@frobolab.org>
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#	* Redistributions of source code must retain the above copyright
#  	notice, this list of conditions and the following disclaimer.
#	* Redistributions in binary form must reproduce the above copyright
#  	notice, this list of conditions and the following disclaimer in the
#  	documentation and/or other materials provided with the distribution.
#	* Neither the name FroboMind nor the
#  	names of its contributors may be used to endorse or promote products
#  	derived from this software without specific prior written permission.
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
****************************************************************************/

#include <stdio.h>
#include <string.h>
#include "ros/ros.h"
#include "msgs/gpgga.h"
#include "msgs/gpgga_utm.h"
#include "boost/tokenizer.hpp"
#include "boost/lexical_cast.hpp"
#include <boost/algorithm/string.hpp>
#include <gps_conversions_lib/gps_conversions_lib.h>

ros::Publisher gpgga_utm_pub;
std::string frame_id;
msgs::gpgga_utm gpgga_utm_msg;


void gpsStateCallback(const msgs::gpgga::ConstPtr& msg)
{
	// save current time
	gpgga_utm_msg.header.stamp = ros::Time::now();

	// import data from the gpgga topic
	gpgga_utm_msg.time_recv = msg->header.stamp;
	gpgga_utm_msg.fix = msg->fix;
	gpgga_utm_msg.sat = msg->sat;

	if (msg->fix >= 1) // if the GPS currently has a valid satellite fix
	{
		gpgga_utm_msg.time = msg->time;
		gpgga_utm_msg.lat = msg->lat;
		gpgga_utm_msg.lon = msg->lon;
		gpgga_utm_msg.alt = msg->alt;
		gpgga_utm_msg.hdop = msg->hdop;			
		gpgga_utm_msg.geoid_height = msg->geoid_height;

		// convert lat/lon to UTM coordinates
		double n, e;
		int znum;
		char zlet;
		
		latlon2utm (gpgga_utm_msg.lat, gpgga_utm_msg.lon, &znum, &zlet, &n, &e);
		gpgga_utm_msg.utm_zone_num = znum;
		gpgga_utm_msg.utm_zone_let = zlet;
		gpgga_utm_msg.utm_n = n;
		gpgga_utm_msg.utm_e = e;
	}
	gpgga_utm_pub.publish (gpgga_utm_msg); // publish the message
}

int main(int argc, char **argv)
{
	ros::init(argc, argv, "gpgga_utm");
	ros::NodeHandle nh;
	ros::NodeHandle n("~");

	std::string subscribe_topic_id;
	std::string publish_topic_id;

	n.param<std::string> ("subscribe_topic_id", subscribe_topic_id, "fmInformation/gpgga_msg");
	n.param<std::string> ("publish_topic_id", publish_topic_id, "fmInformation/gpgga_utm_msg");
//	n.param<std::string> ("frame_id", frame_id, "/base");

	ros::Subscriber sub = n.subscribe(subscribe_topic_id, 1, gpsStateCallback);
	gpgga_utm_pub = n.advertise<msgs::gpgga_utm> (publish_topic_id, 1);

	ros::spin();
	return 0;
}

