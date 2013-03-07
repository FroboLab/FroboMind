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
# File: ransac_extractor.cpp
# Purpose:
# Project:
# Author: Søren Hundevadt Nielsen <soeni05@gmail.com>
# Created: Jun 25, 2012 Søren Hundevadt Nielsen, Source written
****************************************************************************/


#include "ransac_extractor.h"
#include "math.h"
#include <pcl/point_types.h>
#include <pcl/filters/passthrough.h>

SafetyExtractor::SafetyExtractor(){


	marker_rg.header.stamp = ros::Time::now();
	marker_rg.ns = "Ransac";
	marker_rg.pose.orientation.w = 1.0;
	marker_rg.type = visualization_msgs::Marker::CUBE;

	marker_rg.id = 3;

	marker_rg.pose.position.y = 0;
	marker_rg.text = "marker text";

	marker_rg.scale.x = 0.1;
	marker_rg.scale.y = 0.1;
	marker_rg.scale.z = 0.1;

	marker_rg.color.b = 0;
	marker_rg.color.r = 0;
	marker_rg.color.g = 0;
	marker_rg.color.a = 0.6;

	headland = true;


}



double SafetyExtractor::getdistance(Eigen::Vector2d p0, Eigen::Vector2d p1)
{
	return (sqrt(pow((double)p0[0]-(double)p1[0],2)+pow((double)p0[1]-(double)p1[1],2)));
}

bool SafetyExtractor::processPointCloudInRow(sensor_msgs::PointCloud& pointcloud)
{
	Eigen::Vector2d p0(headland_box_min_x,-headland_box_lim_y);
	Eigen::Vector2d p1(headland_box_max_x,headland_box_lim_y);

	bool inrow = false;
	for(size_t i = 0; i < pointcloud.points.size(); i++){
		if(pointcloud.points[i].y > p0(1) && pointcloud.points[i].y < p1(1) ){
			if(pointcloud.points[i].x > p0(0) && pointcloud.points[i].x < p1(0) ){
				inrow = true;
				break;
			}
		}
	}
	return inrow;
}

void SafetyExtractor::processLaserScan(const sensor_msgs::LaserScan::ConstPtr& laser_scan) {

	tf::TransformListener listener_;
	sensor_msgs::PointCloud cloud;
	sensor_msgs::PointCloud cloud_filtered;
	sensor_msgs::PointCloud cloud_l;
	sensor_msgs::PointCloud cloud_r;


    try
    {
    	projector_.projectLaser(*laser_scan, cloud,this->laser_scan_max_distance);
    }
    catch (tf::TransformException& e)
    {
        std::cout << e.what();
        return;
    }

    cloud_filtered.header = cloud.header;
    cloud_filtered.channels = cloud.channels;

    for(size_t i = 0; i < cloud.points.size();i++){
    	if (cloud.points[i].x >0.10 && cloud.points[i].x < this->look_x && fabs(cloud.points[i].y) < this->look_y ) {
    		cloud_filtered.points.push_back(cloud.points[i]);
		}
    }

    double in_r = 1;
    double in_l = 1;
    valid_l = valid_r = false;

    if(cloud_filtered.points.size() > 10){
    	if(processPointCloudInRow(cloud_filtered))
    	{

    		for(int i=0; i < cloud_filtered.points.size(); i++)
    		{
    			if(cloud_filtered.points[i].y > 0.01)
    			{
    				cloud_l.points.push_back(cloud_filtered.points[i]);
    			}
    			else
    			{
    				cloud_r.points.push_back(cloud_filtered.points[i]);
    			}
    		}
    		if(cloud_r.points.size() > 5)
    		{
    			in_r = processPointCloudRight(cloud_r);
    		}
    		if(cloud_l.points.size() > 5)
    		{
    			in_l = processPointCloudLeft(cloud_l);
    		}
    		marker_rg.header.stamp = ros::Time::now();
    		marker_rg.color.g = 1;
    		marker_rg.color.r = 0;
    		headland = false;
    	}else{
    		marker_rg.header.stamp = ros::Time::now();
			marker_rg.color.g = 0;
			marker_rg.color.r = 1;
			headland = true;
    		ROS_DEBUG("NO INROW DATA");
    	}


    	marker_rg.scale.x = (headland_box_max_x - headland_box_min_x);
    	marker_rg.scale.y = headland_box_lim_y*2;
    	marker_rg.scale.z = 0.1;

    	marker_rg.pose.position.x = headland_box_min_x + (headland_box_max_x - headland_box_min_x)/2;

    	marker_rg.header.frame_id = this->frame_id;
        marker_publisher.publish(marker_rg);
    }



    rows_.leftvalid =  valid_l;
	rows_.leftangle = rho_l;
	rows_.leftdistance = dist_l;
	rows_.leftvar = in_l;

    rows_.rightvalid =  valid_r;
	rows_.rightangle = rho_r;
	rows_.rightdistance = dist_r;
	rows_.rightvar = in_r;


	rows_.var = in_l + in_r;

	rows_.headland = headland;

	rows_.header.stamp = ros::Time::now();
	rows_.header.frame_id = this->frame_id;
	row_publisher.publish(rows_);

	pc_publisher.publish(cloud_filtered);

}

double SafetyExtractor::processPointCloudLeft(sensor_msgs::PointCloud & pointcloud){
	double ratio = 0;
	if(pointcloud.points.size()){

		Eigen::Vector2d points[1000];
		Eigen::Vector2d coeffs;
		std::vector<Eigen::Vector2d* > points_ptrs(1000);

		int p0i;
		int p1i;
		int inliers;
		int tryes;



		for(int n = 0; n < this->ransac_n; n++){

			inliers = 0;
			tryes = 0;

			p0i = rand () % pointcloud.points.size() ;
			while(pointcloud.points[p0i].y < 0 && tryes < 10){
				p0i = rand () % pointcloud.points.size() ;
				tryes++;
			}

			if(tryes>9){
				break;
			}

			p1i = rand () % (pointcloud.points.size() - 1);

			if (p1i >= p0i)
			 ++p1i;

			Eigen::Vector2d p0(pointcloud.points[p0i].x,pointcloud.points[p0i].y);
			Eigen::Vector2d p1(pointcloud.points[p1i].x,pointcloud.points[p1i].y);
			double distance = getdistance(p0,p1);
			while((distance < this->minimum_distance_random_points || distance > this->maximum_distance_random_points) && p1i < pointcloud.points.size()-1)
			{
				++p1i;
				p1 = Eigen::Vector2d(pointcloud.points[p1i].x,pointcloud.points[p1i].y);
				distance = getdistance(p0,p1);
			}

			Eigen::ParametrizedLine<double, 2> line;
			line = Eigen::ParametrizedLine<double, 2>::Through(p0, p1);

			for(size_t i = 0; i < pointcloud.points.size(); i++){
				Eigen::Vector2d pTest(pointcloud.points[i].x,pointcloud.points[i].y);
				double dist = line.distance(pTest);
				if(dist < this->inliers_epsilon){
					inliers++;
				}
			}
			ROS_INFO("Inliers left %d ratio %.4f", inliers,(double)inliers/pointcloud.points.size());

			ratio = (double)inliers/pointcloud.points.size();

			if(((double)inliers/pointcloud.points.size()) >= this->minimum_inliers){
				inliers = 0;
				for(size_t i = 0; i < pointcloud.points.size();i++){
					Eigen::Vector2d pTest(pointcloud.points[i].x,pointcloud.points[i].y);

					double dist = line.distance(pTest);
					if(dist < this->inliers_epsilon){
						for(int i = 0; i < exp((double)pTest[0]/3.0)+1; i += 1 ){
							if(inliers< 999){
								points[inliers] = pTest;
								points_ptrs[inliers] = &points[inliers];
								inliers++;
							}
						}
						//std::cout << pTest[0] << " " << pTest[1] << std::endl;
					}
				}
				ROS_INFO("Left n_inliers %d",inliers);

				Eigen::linearRegression(inliers,&(points_ptrs[0]), &coeffs,1 );

				if(0,coeffs[0]*0 + coeffs[1] > 0){

					Eigen::Vector2d pl0(0,coeffs[0]*0 + coeffs[1]);
					Eigen::Vector2d pl1(2,coeffs[0]*2 + coeffs[1]);

					Eigen::ParametrizedLine<double, 2> left_line;
					left_line = Eigen::ParametrizedLine<double, 2>::Through(pl0,pl1);
					Eigen::Vector2d pv(0,0);

					dist_l = left_line.distance(pv);
					Eigen::Vector2d dxdy;

					dxdy=left_line.direction();
					rho_l = atan2((double)dxdy[1],(double)dxdy[0]);

					valid_l = true;
				}


				// *****************  BEGIN MARKER
				marker.header.frame_id = this->frame_id;
				marker.header.stamp = ros::Time::now();
				marker.ns = "Ransac";
				marker.pose.orientation.w = 1.0;
				marker.type = visualization_msgs::Marker::LINE_STRIP;

				marker.id = 1;

				marker.scale.x = ((double)inliers / 1000);

				marker.color.b = 1.0;
				marker.color.r = 0;
				marker.color.a = 0.6;

				geometry_msgs::Point p;
				marker.points.clear();

				p.x = 0; p.y = coeffs[0]*p.x + coeffs[1]; p.z = 0;

				marker.points.push_back(p);

				p.x = this->look_x; p.y = coeffs[0]*p.x + coeffs[1];

				marker.points.push_back(p);
				marker_publisher.publish(marker);

				// ******************* END MARKER



				break;
			}//else if(n >18){

			//}
		}
	}
	return ratio;
}
double SafetyExtractor::processPointCloudRight(sensor_msgs::PointCloud & pointcloud){
	double ratio = 0;
	if(pointcloud.points.size()){

		Eigen::Vector2d points[1000];
		Eigen::Vector2d coeffs;
		std::vector<Eigen::Vector2d* > points_ptrs(1000);

		int p0i;
		int p1i;
		int inliers;
		int tryes;


		for(int n = 0; n < this->ransac_n; n++){

			inliers = 0;
			tryes = 0;

			p0i = rand () % pointcloud.points.size() ;
			while(pointcloud.points[p0i].y > 0 && tryes < 10){
				p0i = rand () % pointcloud.points.size() ;
				tryes++;
			}

			if(tryes>9){
				break;
			}

			p1i = rand () % (pointcloud.points.size() - 1);


			if (p1i >= p0i)
			 ++p1i;

			Eigen::Vector2d p0(pointcloud.points[p0i].x,pointcloud.points[p0i].y);
			Eigen::Vector2d p1(pointcloud.points[p1i].x,pointcloud.points[p1i].y);
			double distance = getdistance(p0,p1);
			while((distance < this->minimum_distance_random_points || distance > this->maximum_distance_random_points) && p1i < pointcloud.points.size()-1)
			{
				++p1i;
				p1 = Eigen::Vector2d(pointcloud.points[p1i].x,pointcloud.points[p1i].y);
				distance = getdistance(p0,p1);
			}

			Eigen::ParametrizedLine<double, 2> line;
			line = Eigen::ParametrizedLine<double, 2>::Through(p0, p1);

			for(size_t i = 0; i < pointcloud.points.size();i++){
				Eigen::Vector2d pTest(pointcloud.points[i].x,pointcloud.points[i].y);
				double dist = line.distance(pTest);
				if(dist < this->inliers_epsilon){
					inliers++;
				}
			}
			ROS_INFO("Inliers right %d", inliers);
			ratio = (double)inliers/pointcloud.points.size();
			if(((double)inliers/pointcloud.points.size()) >= this->minimum_inliers){
				inliers = 0;
				for(size_t i = 0; i < pointcloud.points.size();i++){
					Eigen::Vector2d pTest(pointcloud.points[i].x,pointcloud.points[i].y);

					double dist = line.distance(pTest);
					if(dist < this->inliers_epsilon){
						for(int i = 0; i < exp((double)pTest[0]/3.0)+1; i += 1 ){
							if(inliers< 999){
								points[inliers] = pTest;
								points_ptrs[inliers] = &points[inliers];
								inliers++;
							}
						}
						//std::cout << pTest[0] << " " << pTest[1] << std::endl;
					}
				}
				ROS_INFO("Right n_inliers %d",inliers);
				Eigen::linearRegression(inliers,&(points_ptrs[0]), &coeffs,1 );

				if(0,coeffs[0]*0 + coeffs[1] < 0){

					Eigen::Vector2d pl0(0,coeffs[0]*0 + coeffs[1]);
					Eigen::Vector2d pl1(2,coeffs[0]*2 + coeffs[1]);

					Eigen::ParametrizedLine<double, 2> right_line;
					right_line = Eigen::ParametrizedLine<double, 2>::Through(pl0,pl1);
					Eigen::Vector2d pv(0,0);

					dist_r = right_line.distance(pv);
					Eigen::Vector2d dxdy;

					dxdy=right_line.direction();
					rho_r = atan2((double)dxdy[1],(double)dxdy[0]);

					valid_r = true;

				}

				// *****************  BEGIN MARKER
				marker.header.frame_id = this->frame_id;
				marker.header.stamp = ros::Time::now();
				marker.ns = "Ransac";
				marker.pose.orientation.w = 1.0;
				marker.type = visualization_msgs::Marker::LINE_STRIP;

				marker.id = 2;

				marker.scale.x = ((double)inliers / 1000);
				marker.color.b = 1.0;
				marker.color.a = 0.6;

				geometry_msgs::Point p;
				marker.points.clear();

				p.x = 0; p.y = coeffs[0]*p.x + coeffs[1]; p.z = 0;

				marker.points.push_back(p);

				p.x = this->look_x; p.y = coeffs[0]*p.x + coeffs[1];

				marker.points.push_back(p);
				marker_publisher.publish(marker);

				// ******************* END MARKER

				break;
			}//else if(n >18){

			//}
		}
	}
	return ratio;
}


