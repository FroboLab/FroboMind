#include <ros/ros.h>
#include "std_msgs/UInt32.h"
#include "jai_ad-080ge.h"

// double spray_time;
bool spray_l,spray_r;
ros::Time l_start,r_start;
JAI_AD080GE * camera1;
JAI_AD080GE * camera2;

void on_spray(const std_msgs::UInt32::ConstPtr& msg)
{
    ROS_INFO("Recevied message, data: %d",msg->data);
	if(msg->data == 1)
	{
		spray_l = true;
		spray_r = false;
	}
	if(msg->data == 2)
	{
		spray_r = true;
		spray_l = false;
	}
	if(msg->data == 0)
	{
		spray_l = spray_r = false;
	}
}

void on_timer(const ros::TimerEvent& e)
{
    ROS_INFO_THROTTLE(1,"Timer called");
	if(spray_l)
	{
	    ROS_INFO_THROTTLE(1,"Spray left activated");
		//camera1->setUserOutput0();
	}
	else
	{
	  // camera1->resetUserOutput0();
	}
	  

	if(spray_r)
	{
		ROS_INFO_THROTTLE(1,"Spray right activated");
		camera2->setUserOutput0();
	}
	else
	{
	        camera2->resetUserOutput0();
	}

}

int main(int argc, char **argv)
{

	//Initialize ros usage
	ros::init(argc, argv, "sprayer_implement");

	//Create Nodehandlers
	ros::NodeHandle nn("");
	ros::NodeHandle nh("~");

	std::string sub;

	nh.param<std::string>("cmd_subsscriber_topic",sub,"/fmControllers/spray");

    spray_l = spray_r = false;	

	ros::Subscriber s = nn.subscribe(sub, 10, &on_spray);

	ros::Timer t = nh.createTimer(ros::Duration(0.05),&on_timer);

	//camera1 = new JAI_AD080GE("X800452", JAI_AD080GE::NIR, 12.0);
	camera2 = new JAI_AD080GE("X800453", JAI_AD080GE::NIR, 12.0);
	//camera1->setOptOut1Output(JAI_AD080GE::UserOutput0);
	//camera1->setOptOut2Output(JAI_AD080GE::UserOutput1);
	camera2->setOptOut1Output(JAI_AD080GE::UserOutput0);
	camera2->setOptOut2Output(JAI_AD080GE::UserOutput1);

	t.start();
	//Handle callbacks
	ros::spin();

}
