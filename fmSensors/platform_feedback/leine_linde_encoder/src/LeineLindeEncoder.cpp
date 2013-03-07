/*
 * LeineLindeEncoder.cpp
 *
 *  Created on: Feb 21, 2012
 *      Author: molar
 */

#include "LeineLindeEncoder.h"

LeineLindeEncoder::LeineLindeEncoder() {
	// TODO Auto-generated constructor stub
	this->cycle_time_ms = 0;
	this->current_position = 0;
	this->last_position = 0;
	this->state = LL_STATE_INIT;
	this->config_state = LL_CONFIG_PREOP;
	this->node_id = 0;
	this->encoder_state = S_NMT_UNKNOWN;
	// wait 1 sec for SDO response
	this->sdo_timeout = 1;
	this->preop_timeout = 5;
	this->last_heartbeat = ros::Time::now();

	this->msg_sent = false;
	this->sdo_reply_received = false;
	this->tpdo_received = false;
	this->tx_msg.length = 8;
	this->max_diff = 1000;

}

LeineLindeEncoder::~LeineLindeEncoder() {
	// TODO Auto-generated destructor stub
}

void LeineLindeEncoder::processRXEvent(const msgs::can::ConstPtr & msg)
{
	if((uint16_t)msg->id == (LL_DEF_TPDO1 + this->node_id))
	{
		//return to upper layers XXX: perhaps use a queue here
		tpdo_received = true;
		tpdo_msg = *msg;

	}
	else if((uint16_t)msg->id == (LL_DEF_SDO_RX_RESPONSE + this->node_id))
	{
		if(sdo_reply_received)
		{
			ROS_ERROR("Unhandled SDO reply!");
		}
		else
		{
			// sdo reply received put in received queue
			sdo_reply_received= true;
			sdo_reply_msg = *msg;
		}
	}
	else if((uint16_t)msg->id == (LL_DEF_HEARTBEAT + this->node_id))
	{
		ROS_DEBUG_THROTTLE(1,"HEARTBEAT Detected, %d",msg->data[0]);
		// heart beat received store time at which it was received
		last_heartbeat = ros::Time::now();
		// store encoder state
		encoder_state = (ll_canopen_nmt_state)(msg->data[0] & (0x7F));
	}
}




ll_sdo_return_t LeineLindeEncoder::transmitSDOWriteRequest(uint16_t index, uint8_t subindex, uint8_t *data, uint8_t data_length,uint32_t& err_code)
{
	ll_sdo_return_t retval = SDO_WAITING;
	if(!msg_sent)
	{
		this->tx_msg.id = LL_DEF_SDO_RX + this->node_id;
		// setup expedited SDO transfer (data is in the last bytes) and that size is not indicated by us
		this->tx_msg.data[0] = (1 << 5) | (1 << 1) ;
		// bit 2 and 3 is the number of bytes not used in data payload
		this->tx_msg.data[0] |= (4 - data_length) << 2;
		// subindex to write

		this->tx_msg.data[1] = index;
		this->tx_msg.data[2] = index >> 8;
		this->tx_msg.data[3] = subindex;

		for(int i=0;i<data_length;i++)
		{
			this->tx_msg.data[4+i] = data[i];
		}

		this->tx_msg.header.stamp = ros::Time::now();
		can_tx_pub.publish(this->tx_msg);
		msg_sent = true;
		this->SDO_timeout_transmit_time = ros::Time::now();
	}
	else
	{
		if(checkSDOReply(err_code))
		{
			if(err_code == 0)
			{
				//success
				retval = SDO_OK;
				msg_sent = false;
			}
			else
			{
				// SDO error occured
				retval = SDO_ERROR;
				msg_sent = false;
			}
		}
		else
		{
			if((ros::Time::now() - this->SDO_timeout_transmit_time).toSec() > this->sdo_timeout)
			{
				ROS_ERROR("SDO Timed out");
				retval = SDO_TIMEOUT;
				msg_sent = false;
			}
		}
	}

	return retval;
}

void LeineLindeEncoder::transmitNMTRequest(ll_canopen_nmt state)
{
	this->tx_msg.id = 0;
	this->tx_msg.data[0] = state;
	this->tx_msg.data[1] = this->node_id;

	this->tx_msg.header.stamp = ros::Time::now();
	can_tx_pub.publish(this->tx_msg);
}

bool LeineLindeEncoder::processTPDO(uint32_t& pos, int16_t& vel)
{

	bool ret = false;

	if(tpdo_received)
	{
		tpdo_received = false;
		// extract position and data from TPDO
		int16_t velocity = 0;
		uint32_t position = 0;
		velocity = tpdo_msg.data[4];
		velocity |= ((uint16_t)tpdo_msg.data[5]) << 8;

		position = tpdo_msg.data[0];
		position |= ((uint32_t)tpdo_msg.data[1]) << 8;
		position |= ((uint32_t)tpdo_msg.data[2]) << 16;
		position |= ((uint32_t)tpdo_msg.data[3]) << 24;

		pos = position;
		vel = velocity;
		ret = true;
	}

	return ret;
}

void LeineLindeEncoder::setEncoderPub(ros::Publisher p)
{
	this->encoder_pub = p;
}

void LeineLindeEncoder::setCanPub(ros::Publisher p)
{
	this->can_tx_pub = p;
}

void LeineLindeEncoder::setID(int id)
{
	this->node_id = id;
}

ll_sdo_return_t LeineLindeEncoder::transmitSDOReadRequest(uint16_t index, uint8_t subindex, uint32_t err_code, uint8_t *data, uint8_t & length)
{
	ll_sdo_return_t retval = SDO_WAITING;
	if(!msg_sent)
	{
		this->tx_msg.id = (LL_DEF_SDO_TX + this->node_id);
		// set CCS to 2 initiate upload from encoder
		this->tx_msg.data[0] = (2 << 5);
		this->tx_msg.data[1] = index;
		this->tx_msg.data[2] = index >> 8;
		this->tx_msg.data[3] = subindex;

		this->tx_msg.header.stamp = ros::Time::now();
		this->can_tx_pub.publish(this->tx_msg);
		msg_sent = true;
		this->SDO_timeout_transmit_time = ros::Time::now();
	}
	else
	{
		if(checkSDOReply(err_code))
		{
			if(err_code == 0)
			{
				// first check that this is an expedited transfer (only expedited are supported atm.)
				if((sdo_reply_msg.data[0] & 0x02) == 0x02)
				{
					retval = SDO_OK;
					// copy data to data

					// if size bit is not set we copy all data bytes
					uint8_t unused =0 ;

					if(sdo_reply_msg.data[0] & 0x01)
					{
						unused = (sdo_reply_msg.data[0] & 0xC) >> 2;
					}

					// copy size to user
					length = 4-unused;

					// copy data to user
					for(int i=0;i<length;i++)
					{
						data[i] = sdo_reply_msg.data[i+4];
					}
				}
				else
				{
					ROS_ERROR("Only expedited transfers are supported");

					err_code = 0xFFFFFFFF;
					retval = SDO_ERROR;
				}
			}
			else
			{
				retval = SDO_ERROR;
			}
			msg_sent = false;
		}
		else
		{
			if((ros::Time::now() - this->SDO_timeout_transmit_time).toSec() > this->sdo_timeout)
			{
				ROS_ERROR("SDO Timeout");
				retval = SDO_TIMEOUT;
				msg_sent = false;
			}
		}
	}
	return retval;
}

void LeineLindeEncoder::setPollInterval(int interval)
{
	this->cycle_time_ms = interval;
}

bool LeineLindeEncoder::checkSDOReply(uint32_t& err_code)
{

	if(sdo_reply_received)
	{
		// acknowledge reception of SDO reply to receive handler
		sdo_reply_received = false;

		// check SDO Client Command Specifier (bits 7-5 in message)
		if((sdo_reply_msg.data[0] >> 5) == 3 )
		{
			// write was accepted
			err_code = 0;
		}
		else if((sdo_reply_msg.data[0] >> 5) == 2)
		{
			err_code = 0;
		}
		else if((sdo_reply_msg.data[0] >> 5) == 4)
		{
			// SDO Abort was received return error code
			err_code = sdo_reply_msg.data[4];
			err_code += ((uint32_t)sdo_reply_msg.data[5]) << 8;
			err_code += ((uint32_t)sdo_reply_msg.data[6]) << 16;
			err_code += ((uint32_t)sdo_reply_msg.data[7]) << 24;
		}

		return true;
	}

	return false;
}

void LeineLindeEncoder::processStateMachine(const ros::TimerEvent& e)
{
	uint8_t data[4];
	uint32_t err_code;
	ll_sdo_return_t ret;
	uint32_t pos;
	int16_t vel;
	int32_t diff;
	uint32_t tmp;

	uint8_t recv_data[4];
	uint8_t recv_data_length;
	switch(this->state)
	{
	case LL_STATE_INIT:
		this->config_state = LL_CONFIG_PREOP;
		this->state = LL_STATE_CONFIGURE;
		ROS_DEBUG("Entered Init state");
		break;
	case LL_STATE_CONFIGURE:
		switch(this->config_state)
		{
		case LL_CONFIG_PREOP:
			ROS_DEBUG_ONCE("Entered Config step PREOP");
			// When encoder is running it does not respond to NMT_PREOP cmds so we try reset instead
			transmitNMTRequest(NMT_RESET);
			this->last_heartbeat = ros::Time::now();
			this->config_state = LL_CONFIG_WAIT_PREOP;
			break;
		case LL_CONFIG_WAIT_PREOP:
			// wait for node to change state into preop
			ROS_DEBUG_ONCE("Entered Config step WAIT PREOP");
			if(this->encoder_state == S_NMT_BOOT)
			{
				this->config_state = LL_CONFIG_CYCLE_TIME;
			}
			if((ros::Time::now() - this->last_heartbeat ).toSec() > this->preop_timeout)
			{
				ROS_ERROR_THROTTLE(1,"Encoder Boot message not detected, retrying");
				this->config_state = LL_CONFIG_PREOP;
			}
			break;
		case LL_CONFIG_CYCLE_TIME:
			ROS_DEBUG_ONCE("Entered Config step CYCLE_TIME");
			// configure cycle time
			data[0] = this->cycle_time_ms;
			data[1] = this->cycle_time_ms >> 8;
			data[2] = this->cycle_time_ms >> 16;
			data[3] = this->cycle_time_ms >> 24;

			ret = transmitSDOWriteRequest(0x6200,0,data,4,err_code);
			if(ret == SDO_OK)
			{
				this->config_state = LL_CONFIG_DISABLE1;
			}
			else if(ret == SDO_ERROR)
			{
				ROS_ERROR("Error configuring encoder cycle time, SDO Error code: 0x%X",err_code);
				this->state = LL_STATE_ERROR;
			}
			else if(ret == SDO_TIMEOUT)
			{
				this->state = LL_STATE_ERROR;
			}


			break;
		case LL_CONFIG_DISABLE1:
			// set bit 31 in TPDO COMM param meaning that it is disabled
			ROS_DEBUG_ONCE("Entered Config step DISABLE_1");

			tmp = (node_id + LL_DEF_TPDO1) | (1 << 31);
			data[0] = tmp;
			data[1] = tmp >> 8;
			data[2] = tmp >> 16;
			data[3] = tmp >> 24;

			ret = transmitSDOWriteRequest(0x1800,01,data,4,err_code);
			if(ret == SDO_OK)
			{
				this->config_state = LL_CONFIG_DISABLE2;
			}
			else if(ret == SDO_ERROR)
			{
				ROS_ERROR("Error disabling TPDO comm, SDO Error code: 0x%X",err_code);
			}
			else if(ret == SDO_TIMEOUT)
			{
				this->state = LL_STATE_ERROR;
			}

			break;

		case LL_CONFIG_DISABLE2:
			// disable PDO mapping by writing 0 to num_entries
			ROS_DEBUG_ONCE("Entered Config step DISABLE2");

			data[0] = 0;
			ret = transmitSDOWriteRequest(0x1A00,00,data,1,err_code);
			if(ret == SDO_OK)
			{
				this->config_state = LL_CONFIG_MAPP1;
			}
			else if(ret == SDO_ERROR)
			{
				ROS_ERROR("Error disabling TPDO mapp, SDO Error code: 0x%X",err_code);
			}
			else if(ret == SDO_TIMEOUT)
			{
				this->state = LL_STATE_ERROR;
			}

			break;
		case LL_CONFIG_MAPP1:
			// modify pdomapping, in this case map position entry 6004 to first TPDO entry
			ROS_DEBUG_ONCE("Entered Config step MAPP1");

			data[0] = 32;
			data[1] = 00;
			data[2] = 4;
			data[3] = 0x60;
			ret = transmitSDOWriteRequest(0x1A00,01,data,4,err_code);
			if(ret == SDO_OK)
			{
				this->config_state = LL_CONFIG_MAPP2;
			}
			else if(ret == SDO_ERROR)
			{
				ROS_ERROR("Error configuring entry 1 of TPDO1, SDO Error code: 0x%X",err_code);
			}
			else if(ret == SDO_TIMEOUT)
			{
				this->state = LL_STATE_ERROR;
			}
			break;
		case LL_CONFIG_MAPP2:
			// map velocity to second entry in TPDO
			ROS_INFO_ONCE("Entered Config step MAPP2");
			data[0] = 16;
			data[1] = 01;
			data[2] = 0x30;
			data[3] = 0x60;
			ret = transmitSDOWriteRequest(0x1A00,02,data,4,err_code);
			if(ret == SDO_OK)
			{
				this->config_state = LL_CONFIG_ENABLE1;
			}
			else if(ret == SDO_ERROR)
			{
				ROS_ERROR("Error configuring entry 2 of TPDO1, SDO Error code: 0x%X",err_code);
			}
			else if(ret == SDO_TIMEOUT)
			{
				this->state = LL_STATE_ERROR;
			}

			break;
		case LL_CONFIG_ENABLE1:
			ROS_INFO_ONCE("Entered Config step ENABLE1");

			data[0] = 2;; // write the number of entries in the mapping to enable
			ret = transmitSDOWriteRequest(0x1A00,00,data,1,err_code);
			if(ret == SDO_OK)
			{
				this->config_state = LL_CONFIG_ENABLE2;
			}
			else if(ret == SDO_ERROR)
			{
				ROS_ERROR("Error enabling TPDO mapp of TPDO1, SDO Error code: 0x%X",err_code);
			}
			else if(ret == SDO_TIMEOUT)
			{
				this->state = LL_STATE_ERROR;
			}
			break;
		case LL_CONFIG_ENABLE2:
			// set COB-ID param meaning that the PDO1 is enabled again.
			ROS_INFO_ONCE("Entered Config step ENABLE2");

			tmp = node_id + LL_DEF_TPDO1;
			data[0] = tmp;
			data[1] = tmp >> 8;
			data[2] = tmp >> 16;
			data[3] = tmp >> 24;

			ret = transmitSDOWriteRequest(0x1800,01,data,4,err_code);
			if(ret == SDO_OK)
			{
				if(read_encoder_offset)
				{
					this->config_state = LL_CONFIG_GET_START;
				}
				else
				{
					last_position = 0;
					this->config_state = LL_CONFIG_OP;
				}
			}
			else if(ret == SDO_ERROR)
			{
				ROS_ERROR("Error enabling TPDO comm of TPDO1, SDO Error code: 0x%X",err_code);
			}
			else if(ret == SDO_TIMEOUT)
			{
				this->state = LL_STATE_ERROR;
			}
			break;
		case LL_CONFIG_GET_START:
			ROS_INFO_ONCE("Entered Config step GET_START");
			ret = transmitSDOReadRequest(0x6004,0,err_code,recv_data,recv_data_length);
			if(ret == SDO_OK)
			{
				this->config_state = LL_CONFIG_OP;
				this->last_position = recv_data[0];
				this->last_position |= ((uint32_t)recv_data[1]) << 8;
				this->last_position |= ((uint32_t)recv_data[2]) << 16;
				this->last_position |= ((uint32_t)recv_data[3]) << 24;
				ROS_DEBUG_ONCE("Preloading encoder value with %d",this->last_position);

			}
			else if(ret == SDO_ERROR)
			{
				ROS_ERROR("Error getting current encoder position via SDO, SDO error 0x%X",err_code);
			}
			else if(ret == SDO_TIMEOUT)
			{
				this->state = LL_STATE_ERROR;
			}

			break;
		case LL_CONFIG_OP:
			// bring node into operational state
			ROS_DEBUG_ONCE("Entered Config step OPERATIONAL");

			transmitNMTRequest(NMT_OP);
			this->state = LL_STATE_READY;
			break;
		default:
			break;
		}
		break;
	case LL_STATE_READY:
		ROS_INFO_ONCE("Entered READY");

		while(processTPDO(pos,vel))
		{
			diff = pos - last_position;

			if(diff > max_diff)
			{
				diff = -(8191 - diff);
			}
			else if(diff < -(max_diff))
			{
				diff = (diff + 8191);
			}
			if(invert)
			{
				current_position -= diff;
			}
			else
			{
				current_position += diff;
			}
			last_position = pos;
			ROS_DEBUG("Read: pos %d, vel %d, diff %d",pos,vel,diff);

		}

		this->enc_msg.header.stamp = ros::Time::now();
		this->enc_msg.encoderticks = current_position;
		encoder_pub.publish(this->enc_msg);

		break;
	case LL_STATE_ERROR:
		// do nothing
		this->state = LL_STATE_INIT;
		ROS_ERROR("Error occurred Resetting...");
		break;
	default:
		break;
	}
}



