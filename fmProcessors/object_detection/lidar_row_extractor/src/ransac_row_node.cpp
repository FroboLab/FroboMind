/****************************************************************************
# Ransac row extractor
# Copyright (c) 2011 Søren Hundevadt Nielsen <shn@kbm.sdu.dk>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
# THE SOFTWARE.
#
*****************************************************************************
# File: ransac_row_node.cpp
# Purpose:
# Project:
# Author: Søren Hundevadt Nielsen <soeni05@gmail.com>
# Created: Jun 25, 2012 Søren Hundevadt Nielsen, Source written
****************************************************************************/

#include "ros/ros.h"
#include "sensor_msgs/LaserScan.h"

#include "ransac_extractor.h"
#include "dynamic_reconfigure/server.h"
#include "lidar_row_extractor/lidar_row_extractor_paramsConfig.h"

void callback(lidar_row_extractor::lidar_row_extractor_paramsConfig &config, uint32_t level,SafetyExtractor* r)
{
	ROS_ERROR("Received config");
  r->inliers_epsilon = config.inliers_epsilon;
  r->laser_scan_max_distance = config.laser_scan_max_distance;
  r->minimum_inliers = config.minimum_inliers;
  r->ransac_n = config.ransac_n;
  r->look_x = config.box_lim_x;
  r->look_y = config.box_lim_y;
  r->minimum_distance_random_points = config.ransac_minimum_distance_fit;
  r->maximum_distance_random_points = config.ransac_maximum_distance_fit;
  r->headland_box_min_x = config.headland_box_min_x;
  r->headland_box_max_x = config.headland_box_max_x;
  r->headland_box_lim_y = config.headland_box_lim_y;
}

int main(int argc, char **argv)
{

  ros::init(argc, argv, "row_extractor");

  ros::NodeHandle nh("~");
  ros::NodeHandle n;

  SafetyExtractor re;

  std::string subscriber_topic;

  nh.param<std::string>("subscriber_topic", subscriber_topic, "/lrs/laser_msgs_1");
  nh.param<std::string>("frame_id",re.frame_id,"laser_link");

  re.ls_subscriber = nh.subscribe<sensor_msgs::LaserScan> (subscriber_topic.c_str(), 1, &SafetyExtractor::processLaserScan, &re);
  re.pc_publisher = n.advertise<sensor_msgs::PointCloud>("/fmExtractors/point_cloud", 1);
  re.marker_publisher = n.advertise<visualization_msgs::Marker>("/fmExtractors/ransac_marker", 1);
  re.row_publisher = n.advertise<msgs::row>("/fmExtractors/rows",1);
  dynamic_reconfigure::Server<lidar_row_extractor::lidar_row_extractor_paramsConfig> server;

  dynamic_reconfigure::Server<lidar_row_extractor::lidar_row_extractor_paramsConfig>::CallbackType cb;

  cb = boost::bind(&callback, _1, _2,&re);
  server.setCallback(cb);

  ros::spin();

  return 0;
}
