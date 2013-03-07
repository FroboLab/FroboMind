#include <csignal>
#include <cstdio>
#include "ros/ros.h"
#include <boost/asio.hpp>
#include <sstream>
#include <string>
#include "geometry_msgs/TwistStamped.h"

// published data
geometry_msgs::TwistStamped rtq_command_msg_left;
geometry_msgs::TwistStamped rtq_command_msg_right;

ros::Subscriber hl_subscriber;
ros::Publisher ll_publisher_left;
ros::Publisher ll_publisher_right;

double W = 0.755; //length from center to meter

// Input is a twist message from highlevel
// Output is two rtq command messages, one for each wheel
void callbackHandlerHlSubscriber(const geometry_msgs::TwistStamped::ConstPtr& twi)
{
  // Forward Kinematics

  double vel_right = twi->twist.linear.x + ( W * twi->twist.angular.z );
  double vel_left = (twi->twist.linear.x - ( W * twi->twist.angular.z ));

  rtq_command_msg_left.twist.linear.x = vel_left;
  rtq_command_msg_left.header.stamp = ros::Time::now();
  ll_publisher_left.publish(rtq_command_msg_left);

  rtq_command_msg_right.twist.linear.x = vel_right;
  rtq_command_msg_right.header.stamp = ros::Time::now();
  ll_publisher_right.publish(rtq_command_msg_right);
}


int main(int argc, char **argv){

  ros::init(argc, argv, "armadillo_ifk");
  ros::NodeHandle nh("~"); //local nh

  std::string hl_subscriber_topic;
  std::string ll_publisher_topic_left;
  std::string ll_publisher_topic_right;
  nh.param<std::string>("hl_subscriber_topic", hl_subscriber_topic, "hl_subscriber_topic");
  nh.param<std::string>("ll_publisher_topic_left", ll_publisher_topic_left, "ll_publisher_topic_left");
  nh.param<std::string>("ll_publisher_topic_right", ll_publisher_topic_right, "ll_publisher_topic_right");
  nh.param<double>("distance_center_to_wheel",W,0.755);

  hl_subscriber = nh.subscribe<geometry_msgs::TwistStamped> (hl_subscriber_topic.c_str(), 1, &callbackHandlerHlSubscriber); //seneste msg ligger klar i topic'en til fremtidige sucscribers
  ll_publisher_left = nh.advertise<geometry_msgs::TwistStamped> (ll_publisher_topic_left.c_str(), 1);
  ll_publisher_right = nh.advertise<geometry_msgs::TwistStamped> (ll_publisher_topic_right.c_str(), 1);


  ROS_INFO("armadillo_ifk running");

  ros::spin(); //spinner på events der har noget at gøre med denne node (f.eks. timeren)

  return 0;
}
