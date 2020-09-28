#ifndef _AMS_TYPES_H_
#define _AMS_TYPES_H_

	#include <stdint.h>
	#include <stdbool.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdarg.H>

	typedef enum{
		M_NONE = 0,
		M_CONNECT = 1,
		M_WINK = 3,
		M_SERVICEPIN = 4,
		M_FIRMWARE_INFO = 5,
		M_RESET = 6,
		M_DEBUG_TERMINAL = 10,
		M_FILESHARE = 20,
		M_ETHERNET_INFO = 25,
		M_SCP_INFO = 26,
		M_TIME_INFO = 27,
		M_END = 0xFFFF
	}AMS_msg_t;

	typedef enum{
		T_NONE = 0,
		T_STATE = 1,
		T_ERROR = 2,
		T_ACTIVE = 3,
		T_START = 3,
		T_STOP = 4,
		T_WINK = 3,
		T_GET_INFO = 3,
		T_INFO = 4,
		T_SAVE_RESET = 3,
		T_HARD_RESET = 4,
		T_DOWNLOAD = 3,
		T_UPLOAD = 4,
		T_LIST = 5,
		T_DELETE = 6,
		T_GET_IP = 3,
		T_IP = 4,
		T_GET_MAC = 5,
		T_MAC = 6,
		T_GET_NETMASK = 7,
		T_NETMASK = 8,
		T_GET_DEF_GATEWAY = 9,
		T_DEF_GATEWAY = 10,
		T_DATA_DONE = 10,
		T_GET_DNS = 11,
		T_DNS = 12,
		T_GET_DHCP = 13,
		T_DHCP = 14,
	}AMS_cmd_type_t;

	typedef enum{
		COMMAND = 0,
		FILE_DATA = 1,
		RAW_DATA = 2,
	}conn_type_t;


	typedef struct{
		uint8_t AMS_version;
		uint32_t data_lenght;
		conn_type_t conn_type;
		uint8_t msg_id;
		uint32_t UID[4];
	}AMS_header_t;


	typedef struct{
		bool encrypted;

		//AMS frame
		AMS_header_t header;

		//Payload
		AMS_msg_t command;
		AMS_cmd_type_t cmd_type;
		uint8_t * raw;
	}AMS_packet_t;

	typedef struct{
		void * next;
		AMS_packet_t packet;
	}AMS_packet_buffer_t;
#endif //_AMS_TYPES_H_
