/*
 * roboteq.cpp
 *
 *  Created on: May 15, 2012
 *      Author: morl
 */

#include <ros/ros.h>
#include <msgs/serial.h>
#include <msgs/encoder.h>
#include <msgs/motor_status.h>
#include <geometry_msgs/TwistStamped.h>
#include <sensor_msgs/Joy.h>

/*
 * Convenience structure for stooring the measured data from the motor controller.
 */
struct motor_data
{
	float motor_amps_in;
	float motor_amps_out;

	float motor_voltage_in;
	float motor_voltage_out;

	float aux_voltage;

	unsigned int motor_status;

};


/*! @brief Main class containing the interface to the RoboTeq controller
 *
 * */
class RoboTeq
{
private:

	// messages sent out are stored by the class to avoid reallocations.
	msgs::serial serial_out;
	msgs::encoder encoder_out;
	msgs::motor_status motor_out;


	sensor_msgs::Joy prev_joy;
	geometry_msgs::TwistStamped prev_cmd_vel;
	ros::Time last_serial_msg;

	// struct for stooring the motor data read from the controller.
	struct motor_data m;


	bool deadman_active,cmd_vel_active,joy_init;
	unsigned int deadman_counter;
	bool initialised;

	// the total number of hall ticks since the start of this node.
	int cbr_total;

	// counter used to schedule the commands sent to the roboteq controller.
	unsigned int cycle_counter;

	std::string motor_status_to_string(unsigned int status)
	{
		std::stringstream ss;

		if(status & 0x01)
		{
			ss << "Serial Mode, ";
		}
		if(status & 0x02)
		{
			ss << "Pulse mode, ";
		}
		if(status & 0x04)
		{
			ss << "Analog mode, ";
		}

		if(status & 0x08)
		{
			ss << "Power Stage Off, ";
		}

		if(status & 0x10)
		{
			ss << "Stall detected, ";
		}

		if(status & 0x20)
		{
			ss << "At limit, ";
		}

		if(status & 0x40)
		{
			ss << "Analog inputs uncalibrated, ";
		}
		if(status & 0x80)
		{
			ss << "Amps uncalibrated ";
		}

		return ss.str();
	}

	void publish_encoder()
	{
		encoder_out.header.stamp = ros::Time::now();
		encoder_out.encoderticks = cbr_total;

		encoder_publisher.publish(encoder_out);
	}

	void publish_status()
	{
		motor_out.header.stamp = ros::Time::now();
		motor_out.motor_amps_in = m.motor_amps_in / 10.0;
		motor_out.motor_amps_out = m.motor_amps_out / 10.0;
		motor_out.motor_voltage_in = m.motor_voltage_in / 10.0;
		motor_out.motor_voltage_out = m.motor_voltage_out / 10.0;
		motor_out.motor_status = motor_status_to_string(m.motor_status);

		status_publisher.publish(motor_out);
	}



public:

	//! holds the index of the deadman button in the buttons array of the joy message.
	unsigned int deadman_button_id;

	//! the maximum number of allowed accelerometer matches before declaring the wii controller has been lost.
	unsigned int max_deadman;

	//! factor for converting the cmd_vel into a motor command of +- 1000
	double twist_to_motor_velocity;

	double velocity;
	ros::Time last_joy_received;

	ros::Duration max_time_diff;

	ros::Publisher serial_publisher;
	ros::Publisher encoder_publisher;
	ros::Publisher status_publisher;

	RoboTeq()
	{
		deadman_active = false;
		cmd_vel_active = false;
		joy_init = false;
		initialised = false;
		deadman_counter = 0;
		cycle_counter = 0;
		cbr_total = 0;
		twist_to_motor_velocity = 500;
		last_joy_received = ros::Time::now();
		last_serial_msg = ros::Time::now();

		m.aux_voltage = 0;
		m.motor_voltage_in = 0;
		m.motor_voltage_out = 0;
		m.motor_amps_in = 0;
		m.motor_amps_out = 0;
		m.motor_status = 0;


	}


	void initController()
	{
		serial_out.data = "^ECHOF 1\r";
		serial_out.header.stamp = ros::Time::now();

		serial_publisher.publish(serial_out);

	}

	void transmitSelfInit()
	{
		serial_out.data = "?FID\r";
		serial_out.header.stamp = ros::Time::now();

		serial_publisher.publish(serial_out);
	}


	void onSerialMsgReceived(const msgs::serial::ConstPtr& msg)
	{

		double version=0;
		int cbr_rel = 0;

		ROS_DEBUG("Message received %s",msg->data.c_str());
		if(sscanf(msg->data.c_str(),"FID=Roboteq v%lf RCB200 09/04/2010",&version))
		{
			ROS_INFO("Detected roboteq with version %.1f",version);
			ROS_INFO("Initialising...");

			/* when a FID is received it is either because we requested it or because the controller has been rebooted.
			 * In either case the controller is configured again.
			 * */
			// give the controller some time
			sleep(1);
			initController();
			sleep(1);
			initialised = true;
		}
		else if(sscanf(msg->data.c_str(),"CBR=%d",&cbr_rel))
		{
			cbr_total += cbr_rel;
		}
		else if(sscanf(msg->data.c_str(),"V=%f:%f:%f",&m.motor_voltage_out,&m.motor_voltage_in,&m.aux_voltage))
		{

		}
		else if(sscanf(msg->data.c_str(),"A=%f",&m.motor_amps_out))
		{

		}
		else if(sscanf(msg->data.c_str(),"BA=%f",&m.motor_amps_in))
		{

		}
		else if(sscanf(msg->data.c_str(),"FS=%d",&m.motor_status))
		{
			publish_status();
		}
		else if(msg->data == "-\r")
		{
			ROS_WARN("Command was not accepted for cycle nr. %d",cycle_counter);
		}
		last_serial_msg = ros::Time::now();
	}

	void onCmdVelReceived(const geometry_msgs::TwistStamped::ConstPtr& msg)
	{
		prev_cmd_vel = *msg;
		cmd_vel_active = true;
	}

	void onJoy(const sensor_msgs::Joy::ConstPtr& msg)
	{
		if(joy_init == false)
		{
			joy_init = true;
		}
		else
		{
			if(msg->buttons[deadman_button_id] != 1)
			{
				deadman_active = false;
			}
			else
			{
				deadman_active = true;
			}


			if(msg->axes[0] == prev_joy.axes[0] &&
					msg->axes[1] == prev_joy.axes[1] &&
					msg->axes[2] == prev_joy.axes[2])
			{
				deadman_counter++;

			}
			else
			{
				deadman_counter = 0;
			}


			if(deadman_counter > max_deadman)
			{
				deadman_active = false;
			}
		}
		prev_joy = *msg;
		last_joy_received = ros::Time::now();
	}

	void spin(const ros::TimerEvent& e)
	{
		if(initialised)
		{
			// check joy activity flag
			if(deadman_active && cmd_vel_active)
			{
				if((ros::Time::now() - last_joy_received) > ros::Duration(1) )
				{
					ROS_WARN_THROTTLE(1,"A new joy message has not been recevied for 1 second, shutting down");
					deadman_active = false;
					velocity = 0;
				}
				else
				{
					if(( ros::Time::now() -prev_cmd_vel.header.stamp) > max_time_diff)
					{
						ROS_WARN_THROTTLE(1,"Shutting down due to out of date cmd_vel");
						velocity = 0;
						cmd_vel_active = false;
					}
					else
					{
						velocity = prev_cmd_vel.twist.linear.x;
						if(velocity > 1000)
						{
							velocity = 1000;
						}
						else if(velocity < -1000)
						{
							velocity = -1000;
						}
					}
				}
			}
			else
			{
				ROS_WARN_THROTTLE(5,"Shutting down due to deadman");
				velocity = 0;
			}

			if(ros::Time::now() - last_serial_msg > ros::Duration(5))
			{
				ROS_WARN("Have not heard from Roboteq in 5 seconds shutting down");
				initialised = false;
			}


			std::stringstream ss;

			//send command based on cycle counter
			switch(cycle_counter)
			{
			case 0:
				ss << "!G " << (int)(velocity * twist_to_motor_velocity) << "\r";
				break;
			case 1:
				ss << "?CBR\r";
				break;
			case 2:
				ss << "?A\r";
				break;
			case 3:
				ss << "?BA\r";
				break;
			case 4:
				ss << "?V\r";
				break;
			case 5:
				ss << "?FS\r";
				break;
			default:
				ss << "!G 0\r";
				break;
			}

			serial_out.data = ss.str();
			serial_out.header.stamp = ros::Time::now();
			serial_publisher.publish(serial_out);

			cycle_counter++;
			if(cycle_counter > 5)
			{
				publish_encoder();
				cycle_counter = 0;
			}
		}
		else
		{
			ROS_WARN_THROTTLE(1,"Controller not initialised");
		}
	}
};


int main(int argc,char** argv)
{

	ros::init(argc,argv,"roboteq_controller");

	ros::NodeHandle n;
	ros::NodeHandle nh("~");

	ros::Subscriber s1,s2,s3;

	std::string joy_topic,cmd_vel_topic,serial_tx_topic,serial_rx_topic,encoder_topic,status_topic;
	double max_time_diff;
	int index;
	int max_missing;

	nh.param<std::string>("cmd_vel_topic",cmd_vel_topic,"/fmActuators/cmd_vel");
	nh.param<std::string>("serial_rx_topic",serial_rx_topic,"/fmCSP/S0_rx");
	nh.param<std::string>("serial_tx_topic",serial_tx_topic,"/fmCSP/S0_tx");
	nh.param<std::string>("deadman_joy_topic",joy_topic,"/fmHMI/joy");
	nh.param<std::string>("encoder_topic",encoder_topic,"/fmSensors/encoder");
	nh.param<std::string>("status_topic",status_topic,"/fmActuators/status");


	nh.param<double>("max_time_diff",max_time_diff,0.5);
	nh.param<int>("deadman_joy_button_index",index,3);
	nh.param<int>("deadman_joy_max_missing",max_missing,30);


	RoboTeq controller;

	controller.deadman_button_id = index;
	controller.max_deadman = max_missing;
	controller.max_time_diff = ros::Duration(max_time_diff);

	controller.serial_publisher = nh.advertise<msgs::serial>(serial_tx_topic,10);
	controller.encoder_publisher = nh.advertise<msgs::encoder>(encoder_topic,10);
	controller.status_publisher = nh.advertise<msgs::motor_status>(status_topic,10);

	s1 = nh.subscribe<msgs::serial>(serial_rx_topic,10,&RoboTeq::onSerialMsgReceived,&controller);
	s2 = nh.subscribe<geometry_msgs::TwistStamped>(cmd_vel_topic,10,&RoboTeq::onCmdVelReceived,&controller);
	s3 = nh.subscribe<sensor_msgs::Joy>(joy_topic,10,&RoboTeq::onJoy,&controller);

	ros::Rate r(5);
	while(controller.serial_publisher.getNumSubscribers() != 0)
	{
		ROS_INFO_THROTTLE(1,"Waiting for serial node to subscribe");
		r.sleep();
	}

	controller.transmitSelfInit();

	r.sleep();

	ros::Timer t = nh.createTimer(ros::Duration(0.05),&RoboTeq::spin,&controller);



	ros::spin();

	return 0;
}
