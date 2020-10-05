#ifndef _AMS_H_
#define _AMS_H_
	#include "AMS_types.h"
	#include <Core/protocol/Net/psock.h>
	#include <Core/Process/process.h>

	#define ASM_VERSION 1
	PROCESS_NAME(AMS_data_process);
	extern AMS_packet_t g_AMS_data_packet;
	extern AMS_cmd_type_t g_AMS_data_type;
	extern char * g_data_file;
	extern uint16_t g_AMS_file_port;
	extern uint16_t g_AMS_crc;

	void AMS_init(void);

	void AMS_send_command(AMS_packet_t packet);


#endif //_AMS_H_
