#ifndef __SCP_DEVICE_H__
#define __SCP_DEVICE_H__

	#include "SCP_Comm.h"
	#include <Core/protocol/Net/psock.h>
	#include <Core/Process/process.h>

	#define SCP_BUFFER_SIZE 160
	#define MP_SERVER 0

	extern process_event_t event_SCP_device_update;

	void SCP_Device_init(void);
	SCP_Device_t * SCP_create_new_device(uint16_t DevID);
	void SCP_Device_delete(SCP_Device_t * Device);
	SCP_Device_t * SCP_Device_connection(struct uip_conn * conn);
	void SCP_Device_Find(uint16_t DevID);
	void SCP_Device_found(SCP_Device_t * Device);
	bool SCP_Device_Known(uint16_t DevID);
	uint8_t SCP_number_of_known_devices(void);
	SCP_Device_t * SCP_Loop_known_devices(SCP_Device_t * Device);
	SCP_Device_t * SCP_Loop_active_devices(SCP_Device_t * Device);
	bool SCP_Device_prev_online(SCP_Device_t * Device);
	bool SCP_Device_Sock_Begin(SCP_Device_t * Device);
	bool SCP_Device_Sock_End(SCP_Device_t * Device);
	uint8_t SCP_Devices_of_types(uint16_t * Devices_list,uint8_t Devices_list_size, uint8_t num, ...);
#endif //__SCP_DEVICE_H__
