#include "roboteq/hbl2350.hpp"

int main (int argc, char** argv)
{
	ros::init(argc,argv,"roboteq_controller");
	hbl2350 controller;
	controller.spin();

	return 0;
}
