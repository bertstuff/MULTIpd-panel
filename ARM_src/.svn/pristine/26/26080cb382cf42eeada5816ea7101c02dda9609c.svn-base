#ifndef _AMS_COMMAND_H_
#define _AMS_COMMAND_H_
	#include "AMS_types.h"
	#include <Core/Extensions/ini/minGlue-FatFs.h>

	extern AMS_msg_t g_AMS_send_command;

	AMS_packet_t AMS_State_msg(bool send, AMS_msg_t command,uint32_t state_nr);
	AMS_packet_t AMS_error_msg(bool send, AMS_msg_t command,uint32_t error_nr);
	AMS_packet_t AMS_file_error_msg(bool send, AMS_msg_t command, FRESULT rc);
	AMS_packet_t AMS_connect_msg(bool send);
	AMS_packet_t AMS_servicepin_msg(bool send);
	AMS_packet_t AMS_FileShare_data_done_msg(bool send,uint16_t crc);

	bool AMS_execute_command(AMS_packet_t * packet);

	uint32_t AMS_packet_to_data(AMS_packet_t * packet);
#endif //_AMS_COMMAND_H_
