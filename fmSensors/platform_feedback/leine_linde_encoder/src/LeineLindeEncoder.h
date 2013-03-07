/*
 * LeineLindeEncoder.h
 *
 *  Created on: Feb 21, 2012
 *      Author: molar
 */

#ifndef LEINELINDEENCODER_H_
#define LEINELINDEENCODER_H_

#include <ros/ros.h>
#include <ros/console.h>

#include <msgs/encoder.h>
#include <msgs/can.h>

#include <stdint.h>

typedef enum {	LL_CONFIG_PREOP,
				LL_CONFIG_WAIT_PREOP,
				LL_CONFIG_CYCLE_TIME,
				LL_CONFIG_DISABLE1,
				LL_CONFIG_DISABLE2,
				LL_CONFIG_MAPP1,
				LL_CONFIG_MAPP2,
				LL_CONFIG_ENABLE1,
				LL_CONFIG_ENABLE2,
				LL_CONFIG_GET_START,
				LL_CONFIG_OP
}ll_config_state_t;

typedef enum {LL_STATE_INIT, LL_STATE_CONFIGURE, LL_STATE_READY ,LL_STATE_ERROR} ll_state_t;

typedef enum {SDO_WAITING,SDO_OK,SDO_ERROR,SDO_TIMEOUT} ll_sdo_return_t;

typedef enum {NMT_OP=1,NMT_STOPPED=2,NMT_PREOP=128,NMT_RESET=129,NMT_RESET_COMM=130} ll_canopen_nmt;
typedef enum {S_NMT_BOOT=0,S_NMT_OP=5,S_NMT_STOPPED=4,S_NMT_PREOP=127,S_NMT_UNKNOWN=255} ll_canopen_nmt_state;

#define LL_DEF_TPDO1 384 // BASE COB ID for TPDO1 of encoder
#define LL_DEF_SDO_RX 1536 // BASE COB ID for SDO receive (seen from encoder)
#define LL_DEF_SDO_RX_RESPONSE 0x580 // BASE COB ID for SDO receive reply (seen from encoder)
#define LL_DEF_SDO_TX 0x600
#define LL_DEF_SDO_TX_RESPONSE 0x580
#define LL_DEF_NODE_GUARD 1792 // BASE COB ID for node guard msg
#define LL_DEF_NMT_COB 0
#define LL_DEF_HEARTBEAT 0x700

#define LL_DEF_CYCLIC_TIMER_INDEX 0x6200

#define LL_DEF_TPDO1_COMM_INDEX 0x1801
#define LL_DEF_TPDO1_MAPP_INDEX 0x1a01




class LeineLindeEncoder {
public:
	LeineLindeEncoder();
	virtual ~LeineLindeEncoder();

	void processRXEvent(const msgs::can::ConstPtr& msg);

	void setEncoderPub(ros::Publisher p);
	void setCanPub(ros::Publisher p);
	void setID(int id);
	void setPollInterval(int interval);

	ll_sdo_return_t transmitSDOWriteRequest(uint16_t index,uint8_t subindex,uint8_t* data,uint8_t data_length,uint32_t& err_code);
	ll_sdo_return_t transmitSDOReadRequest(uint16_t index,uint8_t subindex,uint32_t err_code,uint8_t* data,uint8_t& length );
	void transmitNMTRequest(ll_canopen_nmt state);
	void processStateMachine(const ros::TimerEvent& e);

	bool invert;
	bool read_encoder_offset;


private:
	bool processTPDO(uint32_t& pos, int16_t& vel);
	bool checkSDOReply(uint32_t& err_code);

	ll_state_t state;
	ll_config_state_t config_state;
	uint32_t node_id;
	bool msg_sent;

	uint32_t cycle_time_ms;

	ros::Time SDO_timeout_transmit_time;

	double sdo_timeout;
	double preop_timeout;

	msgs::can tx_msg;
	msgs::can rx_msg;

	bool sdo_reply_received;
	msgs::can sdo_reply_msg;

	bool tpdo_received;
	msgs::can tpdo_msg;

	ros::Publisher can_tx_pub;
	ros::Publisher encoder_pub;
	msgs::encoder enc_msg;

	uint32_t last_position;
	int32_t current_position;
	int32_t max_diff;

	ros::Time last_heartbeat;

	ll_canopen_nmt_state encoder_state;





};

#endif /* LEINELINDEENCODER_H_ */
