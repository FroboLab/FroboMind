/*************************************************************************************
# Copyright (c) 2011, Anders Bøgild
# All rights reserved.
# 
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
# 3. All advertising materials mentioning features or use of this software
#    must display the following acknowledgement:
#    This product includes software developed by the University of Southern Denmark.
# 4. Neither the name of the University of Southern Denmark nor the
#    names of its contributors may be used to endorse or promote products
#    derived from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY Anders Bøgild "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL Anders Bøgild BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
# SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**************************************************************************************
# File:     serial_node.cpp                                            
# Purpose:  Create a interface node to handle serial communication
# Project:  vic_interfaces
# Author:   Anders Bøgild <andb@mmmi.sdu.dk>
# Created:  Mar 01, 2011 Anders Bøgild, Source copied from ...
*************************************************************************************/
#include <string>


#include "ros/ros.h"
#include "msgs/serial.h"
#include "msgs/rtq.h"
#include "msgs/rtq_command.h"
#include "msgs/rtq_lamp_command.h"
#include "geometry_msgs/TwistStamped.h"
//#include "wiimote/State.h"
//#include "wiimote/LEDControl.h"
//#include "wiimote/RumbleControl.h"
//#include "wiimote/TimedSwitch.h"
//#include "joy/Joy.h"
#include "sensor_msgs/Joy.h"

//#include <actionlib/client/simple_action_client.h>
//#include <actionlib/client/terminal_state.h>
//#include <vic_estimators/rotateAction.h>

#include <boost/regex.hpp>

#include "rtq_node.hpp"


/* ros messages */
msgs::serial s_rx_msg_;
msgs::serial s_tx_msg_;
msgs::rtq rtq_msg;
geometry_msgs::TwistStamped rtq_twi_msg;
//wiimote::TimedSwitch::Message wmsg;
wiimote::LEDControl wii_led_msg;

ros::Timer command_callback_timer;
ros::Publisher my_command_publisher;            //Serial command to RoboteQ box
ros::Subscriber my_response_subscriber;         //Serial response from RoboteQ box
ros::Subscriber my_hl_command_subscriber;       //Speed commands from higher level to controller
ros::Publisher my_hl_response_publisher;        //Odometry, voltages from controller to higher level
ros::Subscriber my_hl_wiibuttons_subscriber;
ros::Subscriber my_deadmanbutton_subscriber;
ros::Publisher my_wiimote_leds_publisher;
ros::Publisher my_wiimote_rumble_publisher;
ros::Subscriber my_lamp_command_subscriber;



//actionlib::SimpleActionClient<vic_estimators::rotateAction> *ac;


bool vehicle_left_side = false;
bool rtq_configured = false;
unsigned int wii_repport_count = 0;
int rtq_com_timer_watchdog_count = 0;
int rtq_com_timer_watchdog_limit = 0;
unsigned int rtq_com_timer_slot = 0;
int motor_command_G = 0;


#define WII_AXIS_X 0
#define WII_AXIS_Y 1
#define WII_AXIS_Z 2

#define WII_BTN_1 0
#define WII_BTN_2 1
#define WII_BTN_A 2
#define WII_BTN_B 3
#define WII_BTN_Plus 4
#define WII_BTN_Minus 5
#define WII_BTN_Left 6
#define WII_BTN_Right 7
#define WII_BTN_Up 8
#define WII_BTN_Down 9
#define WII_BTN_HOME 10

// Disable RS232 echo from the RoboteQ box
void init_rtq(){
  s_tx_msg_.data = "^ECHOF 1\r";
  my_command_publisher.publish(s_tx_msg_);
}

void rtq_watchdog_reset(){
  rtq_com_timer_watchdog_count = 0;
}

void rtq_watchdog(){

  rtq_com_timer_watchdog_count++;

  if(rtq_com_timer_watchdog_count > rtq_com_timer_watchdog_limit){

    rtq_configured = false;

    if (vehicle_left_side)
      ROS_ERROR("Watchdog triggered for Roboteq controller [left] - declaring emergency stop!");
    else
      ROS_ERROR("Watchdog triggered for Roboteq controller [right] - declaring emergency stop!");

    //TODO: Raise emergency stop !

    // Reset watchdog so we are ready to re-activate
    rtq_watchdog_reset();
  }

}

int count = 0;
bool deadManActive = false;
bool slowMoveActive = false;
bool turnModeActive = false;

void callbackHandlerDeadManBtn(const sensor_msgs::Joy::ConstPtr& joy)
{
  //ROS_INFO("W");

  if (!joy->buttons[WII_BTN_A]){ // Slow move switch on WiiMote button A
    slowMoveActive = true;
  }else{
    slowMoveActive = false;
  }

  if (joy->buttons[WII_BTN_B]){ // Dead man switch on WiiMote button B
    deadManActive = true;
  }else{
    deadManActive = false;
  }


  wii_watchdog(joy);


  //testing the gyro
  if ( (joy->buttons[WII_BTN_HOME]) && (turnModeActive==false) ){
    turnModeActive = true;

    //ROS_INFO("Action server started, sending goal.");
    // send a goal to the action
    //vic_estimators::rotateGoal goal;
    //goal.desired_rotation =M_PI;
    //ac->sendGoal(goal);
  }

  if (turnModeActive){
    turnModeActive=false;
    ROS_INFO_THROTTLE(0.25,"Action server started, waiting for goal.");


    s_tx_msg_.data = "!G 200\r";
    my_command_publisher.publish(s_tx_msg_);

    //wait for the action to return
    //bool finished_before_timeout = ac->waitForResult(ros::Duration(30.0));

    /*if (finished_before_timeout)
    {
      actionlib::SimpleClientGoalState state = ac->getState();
      ROS_INFO("Action finished: %s",state.toString().c_str());
    }else{
      ROS_INFO("Action did not finish before the time out.");
    }
    */
  }
}

// Wiimote watchdog
float x_prev,y_prev,z_prev;
int wii_watchdog_count = 0;
int wii_watchdog_limit = 30;

void wii_watchdog_reset(){
  wii_watchdog_count = 0;
}

void wii_watchdog(const sensor_msgs::Joy::ConstPtr& joy){

  // Detect if the wiimote link is active based on accelerometer noise
  // (the wiimote node is very slow at detecting if wiimote is out of range or has been lost)
  if ( ((joy -> axes[0]) == x_prev) && ((joy -> axes[1]) == y_prev) && ((joy -> axes[2]) == z_prev)){

    wii_watchdog_count++;

    if (wii_watchdog_count > wii_watchdog_limit){

      if (vehicle_left_side)
          ROS_ERROR_THROTTLE(1,"Wii watchdog triggered for Roboteq controller [left] - declaring emergency stop!");
        else
          ROS_ERROR_THROTTLE(1,"Wii Watchdog triggered for Roboteq controller [right] - declaring emergency stop!");

      deadManActive = false;

      //TODO: Raise emergency stop !

    }

  }else{
    wii_watchdog_reset();
  }

  x_prev = joy -> axes[0];
  y_prev = joy -> axes[1];
  z_prev = joy -> axes[2];

}


void callbackHandlerLampCommand(const msgs::rtq_lamp_command::ConstPtr& lamp_command_msg){



  return;
}

bool rtqLampRed = true; //assume dangerous
bool rtqLampYellow = false;
// Sends messages to serial node at a fixed rate
void rtqComTimerCallback(const ros::TimerEvent& e)
{
  //ROS_INFO("rtqComTimerCallback");
  std::stringstream ss;

  //Dont send anything if the roboteq is not yet configured
  if (rtq_configured == false ){
    if (vehicle_left_side)
      ROS_WARN_THROTTLE(2,"Roboteq controller [left] not configured (emergency stop pressed?)");
    else
      ROS_WARN_THROTTLE(2,"Roboteq controller [right] not configured (emergency stop pressed?)");
    return;
  }

  rtq_watchdog();

  // Put each roboteq command into its own slot pr. cycletime
  //ROS_INFO("rtqComTimerCallback %d", rtq_com_timer_slot);
  switch ((rtq_com_timer_slot++) % 5)
  {

    case 0: //set IO port 1 and 2
      //ss << "?CB";

      if (rtqLampRed){
        ss << "!D1 1";
      }else{
        ss << "!D0 1";
      }

      ss << "_";

      if (rtqLampYellow){
        ss << "!D1 2";
      }else{
        ss << "!D0 2";
      }
      break;

    case 1: //ask for relative number of commutations, a.k.a. speed
      ss << "?CBR";
      break;

    case 2: //ask for motor power (?)
      ss << "?M";
        break;

    case 3: //ask for battery voltage
      ss << "?V 2";
      break;

    case 4: //send motor command
      ss << "!G " << motor_command_G;
      break;

    }

    ss << "\r"; //rtq command footer

    s_tx_msg_.data = ss.str();
    my_command_publisher.publish(s_tx_msg_);


    //rtq_com_timer_slot = 0;

}


// Incoming commands from the vehicle inverse kinemaics node
void callbackHandlerHlCmd(const msgs::rtq_command::ConstPtr &cmd){

  //ROS_INFO_THROTTLE(0.5,"RTQ got TrackSpeed %lf",cmd->TrackSpeed);

  if (deadManActive == false){
    motor_command_G = 0;
    return;
  }else{


    // Trackspeed of 1 m/s is roughly corresponding to a motor command of (1000G)/(2.7m/s) -> 370G
    motor_command_G = (int)(cmd->TrackSpeed * 500.0);

    // Bound values
    if ( motor_command_G > 999 ){
      motor_command_G = 1000;
    }else if ( motor_command_G < -999 ){
      motor_command_G = -1000;
    }

    if (slowMoveActive == true)
      motor_command_G /= 4;

  }

  //ROS_INFO_THROTTLE(0.5,"RTQ G cmd [%d]",motor_command_G);

}
// Handles incoming messages from the Wiimote (~100hz)
/*void callbackHandlerHlCmd(const joy::Joy::ConstPtr& joy)
{

  if (joy->buttons[WII_BTN_B])
  { // Dead-man active ( button B )

    //ROS_INFO_ONCE("Active");

    float m_left = -(100.0 * (joy->axes[WII_AXIS_Y] + (0.5 * joy->axes[WII_AXIS_X])));
    float m_right = (100.0 * (joy->axes[WII_AXIS_Y] - (0.5 * joy->axes[WII_AXIS_X])));



    if (joy->buttons[WII_BTN_A])
    { // Slow-manoeuvre switch on wiimote button A
      m_left /= 10.0;
      m_right /= 10.0;
    }
    else
    {
      m_left /= 1.0;
      m_right /= 1.0;
    }

    if (vehicle_left_side)
    {
      motor_command_G_left = (int)m_left;
    }
    else
    {
      motor_command_G_right = (int)m_right;
    }

  }
  else
  { // Dead-man in-active ( button B )

    //ROS_INFO_ONCE("Not active");
    motor_command_G_left = 0;
    motor_command_G_right = 0;

  }

}*/

//Handles strings (ended by newline) received from the serial port
void callbackHandlerRtqResponse(const msgs::serial::ConstPtr& msg)
{
  // simple attempt to parse with scanf
  int counter = 0;
  int counter_relative = 0;
  int ampere = 0 ;
  double battery_volts = 0;
  double rtq_ver = 0;

  /*
  if (vehicle_left_side)
    ROS_INFO("Roboteq [L]: '%s'",msg->data.c_str());
  else
    ROS_INFO("Roboteq [R]: '%s'",msg->data.c_str());
*/

  if (rtq_configured == true){
    if (sscanf(msg->data.c_str(), "CB=%d", &counter) == 1)
    {
      if (vehicle_left_side) counter=-counter;
      rtq_msg.BrushlessCounter = counter;
      rtq_watchdog_reset();
    }

    else if (sscanf(msg->data.c_str(), "CBR=%d", &counter_relative) == 1)
    {
      if (vehicle_left_side) counter_relative=-counter_relative;
      rtq_msg.BrushlessCounterRelative = counter_relative;
      rtq_watchdog_reset();
    }

    else if (sscanf(msg->data.c_str(), "M=%d", &ampere) == 1)
    {
      if (vehicle_left_side) ampere=-ampere;
      rtq_msg.BatteryAmpere = ampere;
      rtq_watchdog_reset();
    }

    else if (sscanf(msg->data.c_str(), "V=%lf", &battery_volts) == 1)
    {
      //ROS_INFO("Got volts %f", battery_volts);
      rtq_msg.BatteryVoltage = battery_volts / 10.0;
      rtq_watchdog_reset();
    }
  }
  else if (rtq_configured == false)
  {
    int ecoff_val = 0;
    if (sscanf(msg->data.c_str(), "FID=Roboteq v%lf RCB200 09/04/2010", &rtq_ver) == 1)
    {
      if (rtq_ver == 1.2){
        if (vehicle_left_side)
          ROS_INFO("Roboteq controller [left] online, initializing");
        else
          ROS_INFO("Roboteq controller [right] online, initializing");
        sleep(1);
        init_rtq();
      }else{
        ROS_ERROR("Unsupported firmware version '%lf' found on Roboteq controller",rtq_ver);
      }
    }else if(sscanf(msg->data.c_str(), "^ECHOF %d", &ecoff_val) == 1){
      if (ecoff_val == 1){
        rtq_configured = true;
        rtq_watchdog_reset();
        if (vehicle_left_side)
          ROS_INFO("Roboteq controller [left] online, initializing done");
        else
          ROS_INFO("Roboteq controller [right] online, initializing done");
      }
    }
  }

  // Maybe a bad idea, data can be 3 iterations old..
  rtq_msg.header.stamp = ros::Time::now();

  my_hl_response_publisher.publish(rtq_msg);

}

int main(int argc, char **argv)
{

  /* parameters */
  std::string device;
  std::string rtq_command_topic;
  std::string rtq_hl_command_topic;
  std::string rtq_hl_response_topic;
  std::string rtq_response_topic;
  std::string rtq_vehicle_side;
  double rtq_com_cycletime;
  std::string deadmanbutton_topic;
  std::string rtq_lamp_command_topic;

  /* initialize ros usage */
  ros::init(argc, argv, "Roboteq node");

  /* nodehandlers */
  ros::NodeHandle nh; //global
  ros::NodeHandle n("~"); //private

  /* read parameters from ros parameter server if available otherwise use default values */
  //n.param<std::string> ("device", device, "/dev/ttyS0");
  n.param<std::string>("rtq_command_topic", rtq_command_topic, "S0_tx_msg"); //we publish to the serial interface node
  n.param<std::string>("rtq_hl_command_topic", rtq_hl_command_topic, "rtq_msg"); //we subscribe to some output from a kinematics node
  n.param<std::string>("rtq_response_topic", rtq_response_topic, "S0_rx_msg"); //we subscribe to the response from the Roboteq controller box
  n.param<std::string>("rtq_hl_response_topic", rtq_hl_response_topic, "rtq_response_msg"); //we publish the response from the Rooteq
  n.param<std::string>("rtq_vehicle_side", rtq_vehicle_side, "none"); //we subscribe to the response from the rooteq controller
  n.param<double>("rtq_com_cycletime",rtq_com_cycletime, 1.0); //cycle time in seconds
  n.param<int>("rtq_com_timer_watchdog_limit",rtq_com_timer_watchdog_limit, 10); //cycle maximum number of com cycles without data from the roboteq controller, before emergencystop is raised
  n.param<std::string>("deadmanbutton_topic", deadmanbutton_topic, "deadmanbutton_topic");
  n.param<std::string>("rtq_lamp_command_topic",rtq_lamp_command_topic,"rtq_lamp_command_topic");
  //n.param<std::string>("wiimote_rumble_led_topic",wiimote_rumble_led_topic,"wiimote_rumble_led_topic");

  // Layer below
  my_command_publisher = nh.advertise<msgs::serial> (rtq_command_topic.c_str(), 100);
  my_response_subscriber = nh.subscribe<msgs::serial> (rtq_response_topic.c_str(), 1000, &callbackHandlerRtqResponse);

  // Layer above
  //my_hl_command_subscriber = nh.subscribe<joy::Joy> (rtq_hl_command_topic.c_str(), 1, &callbackHandlerHlCmd); //from rtq_testnode
  my_hl_command_subscriber = nh.subscribe<msgs::rtq_command> (rtq_hl_command_topic.c_str(), 1, &callbackHandlerHlCmd); //from rtq_testnode
  my_hl_response_publisher = nh.advertise<msgs::rtq>(rtq_hl_response_topic.c_str(),100);

  //Security
  my_deadmanbutton_subscriber = nh.subscribe<sensor_msgs::Joy> (deadmanbutton_topic.c_str(), 1, &callbackHandlerDeadManBtn);
  my_lamp_command_subscriber = nh.subscribe<msgs::rtq_lamp_command> (rtq_lamp_command_topic.c_str(), 1, &callbackHandlerLampCommand);

  //LED+Rumble on wii
  my_wiimote_leds_publisher = nh.advertise<wiimote::LEDControl> ("/wiimote/leds",1);
  my_wiimote_rumble_publisher = nh.advertise<wiimote::RumbleControl> ("/wiimote/rumble",1);

  //Com is handeled from a timer so RS232 bandwidth to RoboTeQ can be controlled
  command_callback_timer = n.createTimer(ros::Duration(rtq_com_cycletime), rtqComTimerCallback);

  ROS_INFO("Roboteq node rtq_com_cycletime is %f", rtq_com_cycletime);

  if (rtq_vehicle_side == "left"){
  	vehicle_left_side = true;
  	//ROS_INFO("RTQ_node is left side");
  }else if(rtq_vehicle_side == "right"){
  	vehicle_left_side = false;
  	//ROS_INFO("RTQ_node is right side");
  }else{
  	vehicle_left_side = false;
  	ROS_WARN("RTQ_node don't know what side it is, invalid parameter [%s]",rtq_vehicle_side.c_str());
  }

  // Ensure that serial node is up at running so the initialization string i sent to RTQ controller
  while ( ! my_command_publisher.getNumSubscribers() ){
  	ROS_WARN_THROTTLE(1,"Waiting for serial_node to subscribe");
  }
  //init_rtq(); // in case we dumped in on a freshly started node and dont catch the init string

  // Look at relevant actions
  //ac = new actionlib::SimpleActionClient<vic_estimators::rotateAction>("/rotate_base", true);
  //ROS_INFO("Waiting for action server to start.");
  // wait for the action server to start
  //ac->waitForServer(); //will wait for infinite time


  // Play with detecting if wiimote has been lost

  // Play with the WII LED's
/*
  while ( ! my_wiimote_leds_publisher.getNumSubscribers() ){
          ROS_WARN_THROTTLE(1,"Waiting for to WiiMote to get online");
    }

  // --- static ---
  wiimote::TimedSwitch ts_on;
  ts_on.switch_mode=wiimote::TimedSwitch::ON;
  wiimote::TimedSwitch ts_off;
  ts_off.switch_mode=wiimote::TimedSwitch::OFF;
  wiimote::TimedSwitch ts_nochange;
  ts_off.switch_mode=wiimote::TimedSwitch::NO_CHANGE;


  //std::vector<wiimote::TimedSwitch> switchvec;
  //switchvec.push_back(ts_on);
  //switchvec.push_back(ts_off);
  //switchvec.push_back(ts_on);
  //switchvec.push_back(ts_off);
  //wii_led_msg.set_timed_switch_array_vec(switchvec);

  // --- pulsed ---
  wiimote::TimedSwitch ts_pulsed;
  ts_pulsed.switch_mode=wiimote::TimedSwitch::FOREVER;
  std::vector<float> patternvec;
  patternvec.push_back(1.0);
  patternvec.push_back(0.5);
  patternvec.push_back(1.0);
  patternvec.push_back(0.5);
  ts_pulsed.set_pulse_pattern_vec(patternvec);

  std::vector<wiimote::TimedSwitch> switchvec;
  switchvec.push_back(ts_on);
  switchvec.push_back(ts_off);
  switchvec.push_back(ts_pulsed);
  switchvec.push_back(ts_on);

  wii_led_msg.set_timed_switch_array_vec(switchvec);

  // publish msg
  my_wiimote_leds_publisher.publish(wii_led_msg);
*/
  ros::spin();

  return 0;
}

