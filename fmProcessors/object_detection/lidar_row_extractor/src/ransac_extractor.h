#ifndef RansacRowExtractor_H_
#define RansacRowExtractor_H_

#define EIGEN2_SUPPORT

#include "sensor_msgs/LaserScan.h"
#include "geometry_msgs/Point32.h"
#include "laser_geometry/laser_geometry.h"

#include "ros/ros.h"
#include "msgs/row.h"

#include "tf/transform_listener.h"

#include "visualization_msgs/Marker.h"

#include <Eigen/Geometry>
#include <Eigen/LeastSquares>



class SafetyExtractor
{
private:

  msgs::row rows_;
  laser_geometry::LaserProjection projector_;

  visualization_msgs::Marker marker;
  visualization_msgs::Marker marker_rg;

  double rho_l,rho_r,dist_l,dist_r;
  bool headland,valid_l,valid_r;


  double processPointCloudRight(sensor_msgs::PointCloud& pointcloud);
  double processPointCloudLeft(sensor_msgs::PointCloud& pointcloud);
  bool processPointCloudInRow(sensor_msgs::PointCloud& pointcloud);

  double getdistance(Eigen::Vector2d p0,Eigen::Vector2d p1);
public:

  ros::Subscriber ls_subscriber;
  ros::Publisher pc_publisher;
  ros::Publisher marker_publisher;
  ros::Publisher row_publisher;

  double laser_scan_max_distance;
  double inliers_epsilon;
  int ransac_n;
  double minimum_inliers;
  std::string frame_id;

  double look_x,look_y;
  double minimum_distance_random_points;
  double maximum_distance_random_points;

  double headland_box_min_x,headland_box_max_x,headland_box_lim_y;

  SafetyExtractor();
  void processLaserScan(const sensor_msgs::LaserScan::ConstPtr& laser_scan);

};

#endif /* RansacRowExtractor_H_ */
