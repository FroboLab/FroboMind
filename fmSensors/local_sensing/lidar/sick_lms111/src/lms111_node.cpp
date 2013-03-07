/****************************************************************************
# FroboMind template_cpp_node
# Copyright (c) 2011-2013, editor Kent Stark Olsen kent.stark.olsen@gmail.com
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
#include <csignal>
#include <cstdio>
#include <LMS1xx.h>
#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"

#define DEG2RAD M_PI/180.0
#define MAXRETRYES 120

int main(int argc, char **argv)
{

  /* laser data */
  LMS1xx laser;
  scanCfg cfg;
  scanDataCfg dataCfg;
  scanData data;

  /* published data */
  sensor_msgs::LaserScan scan_msg;

  /* parameters */
  std::string host;
  std::string topic_id;
  std::string frame_id;
  bool invert;

  /* variables */
  int retrycount = 0;

  ros::init(argc, argv, "LMS111_node");
  ros::NodeHandle nh;
  ros::NodeHandle n("~");

  n.param<std::string> ("host", host, "192.168.0.10");
  n.param<std::string> ("topic_id", topic_id, "scan_msg");
  n.param<std::string> ("frame_id", frame_id, "/map");
  n.param<bool> ("invert_output",invert,false);

  ros::Publisher scan_pub = nh.advertise<sensor_msgs::LaserScan> (topic_id, 1);

  ROS_INFO("Connecting to device at : %s", host.c_str());

  /* initialize hardware */
  laser.connect(host);

  if (laser.isConnected())
  {

    ROS_INFO("Connected to device at : %s", host.c_str());

    laser.login();
    cfg = laser.getScanCfg();

    scan_msg.header.frame_id = frame_id;

    scan_msg.range_min = 0.01;
    scan_msg.range_max = 6.0;

    scan_msg.scan_time = 100.0 / cfg.scaningFrequency;

    scan_msg.angle_increment = cfg.angleResolution / 10000.0 * DEG2RAD;
    scan_msg.angle_min = cfg.startAngle / 10000.0 * DEG2RAD - M_PI / 2;
    scan_msg.angle_max = cfg.stopAngle / 10000.0 * DEG2RAD - M_PI / 2;

    //std::cout << "res : " << cfg.angleResolution << " freq : " << cfg.scaningFrequency << std::endl;

    int num_values;
    if (cfg.angleResolution == 2500)
    {
      num_values = 1081;
    }
    else if (cfg.angleResolution == 5000)
    {
      num_values = 541;
    }
    else
    {
      ROS_ERROR("Unsupported resolution");
      return 0;
    }

    scan_msg.time_increment = scan_msg.scan_time / num_values;

    scan_msg.ranges.resize(num_values);
    scan_msg.intensities.resize(num_values);

    dataCfg.outputChannel = 1;
    dataCfg.remission = true;
    dataCfg.resolution = 1;
    dataCfg.encoder = 0;
    dataCfg.position = false;
    dataCfg.deviceName = false;
    dataCfg.outputInterval = 1;

    laser.setScanDataCfg(dataCfg);

    laser.startMeas();

    status_t stat;
    do // wait for ready status
    {
      retrycount++;
      stat = laser.queryStatus();
      ros::Duration(1.0).sleep();
    } while ((stat != ready_for_measurement) && retrycount < MAXRETRYES);

    if (stat == ready_for_measurement)
    {
      ROS_INFO("Device ready - receiving measurement");
      laser.scanContinous(1);

      while (ros::ok())
      {
        ros::Time start = ros::Time::now();

        scan_msg.header.stamp = start;
        ++scan_msg.header.seq;

        laser.getData(data);

        if(invert == false)
        {
			for (int i = 0; i < data.dist_len1; i++)
			{
			  scan_msg.ranges[i] = data.dist1[i] * 0.001;
			}

			for (int i = 0; i < data.rssi_len1; i++)
			{
			  scan_msg.intensities[i] = data.rssi1[i];
			}
        }
        else
        {
			for (int i = 0; i < data.dist_len1; i++)
			{
			  scan_msg.ranges[i] = data.dist1[data.dist_len1 - i - 1] * 0.001;
			}

			for (int i = 0; i < data.rssi_len1; i++)
			{
			  scan_msg.intensities[i] = data.rssi1[data.rssi_len1 - i- 1];
			}
        }

        scan_pub.publish(scan_msg);

        ros::spinOnce();
      }

      laser.scanContinous(0);
      laser.stopMeas();
      laser.disconnect();
    }
    else
    {
      ROS_ERROR("Device not ready for measurement");
      return 0;
    }
  }
  else
  {
    ROS_ERROR("Connection to device @ %s failed", host.c_str());
  }
  return 0;

}
