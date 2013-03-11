/****************************************************************************
 # FroboMind frobit_interface.hpp
 # Copyright (c) 2011-2013, author Leon Bonde Larsen <leon@bondelarsen.dk>
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
#ifndef FROBIT_INTERFACE_H_
#define FROBIT_INTERFACE_H_

#include <ros/ros.h>
#include <string.h>
#include <sstream>
#include <boost/lexical_cast.hpp>
#include <msgs/nmea.h>
#include <msgs/encoder.h>
#include <std_msgs/Bool.h>
#include <geometry_msgs/TwistStamped.h>

/**
 * \mainpage
 *
 * \ref Node
 *
 * \ref Params
 *
 * \ref Topics
 *
 *
 *
 */

/**
 *
 * @addtogroup Node
 * @{
 * Class for controlling the Frobit
 *
 */
class FrobitInterface
{
private:
  double left_vel, right_vel, meters_to_ticks;

  bool active, deadman;

  ros::Time last_deadman_received;

  struct
  {
    /** @addtogroup Node
     * @{
     * @addtogroup Params
     * @{
     */
    double max_velocity; //!< maximum possible velocity[m/s] of the robot from global parameter "robot_max_velocity"
    double wheel_radius; //!< radius[m] of the wheels from global parameter "diff_steer_wheel_radius"
    double ticks_pr_rev; //!< conversion factor[ticks/round] from global parameter "diff_steer_wheel_ticks_per_rev"
    double interval; //!< interval[ms] between publishing motor commands from local parameter "nmea_to_frobit_interval"
    double timeout; //!< maximum allowed interval[s] between cmd_vel messages from local parameter "cmd_vel_timeout"
    bool castor_front; //!< Is the castor wheel in front? from local parameter "castor_front"
    /**@} @}*/
  } parameters;

  struct
  {
    /** @addtogroup Node
     * @{
     * @addtogroup Params
     * @{
     */
    geometry_msgs::TwistStamped cmd_vel_left;
    geometry_msgs::TwistStamped cmd_vel_right;
    msgs::nmea motor_command;
    msgs::encoder encoder;
    /** @} */
  } messages;

  struct
  {
    /** @addtogroup Topics
     * @{
     *
     * Node topics
     */
    std::string cmd_vel_left; //!< the commanded velocity input from higher layers @type \ref TwistStamped
    std::string cmd_vel_right; //!< the commanded velocity input from higher layers @type \ref TwistStamped
    std::string deadman; //!< the deadman input from higher layers @type \ref StringStamped
    std::string nmea_sub; //!< the nmea input from lower layers @type \ref nmea
    std::string encoder_left; //!< the encoder input from lower layers @type \ref encoder
    std::string encoder_right; //!< the encoder input from lower layers @type \ref encoder
    std::string nmea_pub; //!< the nmea output to lower layers @type \ref nmea
    /** @} */
  } topics;

  struct
  {
    /** @addtogroup Node
     * @{
     * @addtogroup Params
     * @{
     */
    ros::Subscriber cmd_vel_left;
    ros::Subscriber cmd_vel_right;
    ros::Subscriber deadman;
    ros::Subscriber nmea;
    /** @} */
  } subscribers;

public:
  ros::NodeHandle global_node_handler;
  ros::NodeHandle local_node_handler;

  struct
  {
    /** @addtogroup Node
     * @{
     * @addtogroup Params
     * @{
     */
    ros::Publisher nmea;
    ros::Publisher encoder_left;
    ros::Publisher encoder_right;
    /** @} */
  } publishers;

  FrobitInterface();

  void makeItSpin(void);
  void on_vel_msg_left(const geometry_msgs::TwistStamped::ConstPtr&);
  void on_vel_msg_right(const geometry_msgs::TwistStamped::ConstPtr&);
  void on_deadman(const std_msgs::Bool::ConstPtr&);
  void on_encoder(const msgs::nmea::ConstPtr&);
  void on_timer(const ros::TimerEvent&);
};
/** @}*/
#endif /*FROBIT_INTERFACE_H_*/
