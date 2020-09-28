
#ifndef __SCP_BUFFER_H__
#define __SCP_BUFFER_H__
	#include "SCP_types.h"

	void SCP_Buffer_init(void);
	bool SCP_Buffer_put(SCP_pack_t ** packet_handle, SCP_Data_t packet_data, struct process * process, bool important, uint16_t Device);
	bool SCP_Buffer_put_incomming(SCP_pack_t ** packet_handle, SCP_pack_t packet_data);
	SCP_pack_t * SCP_Buffer_get(uint16_t DevID);
	void SCP_Buffer_remove(SCP_pack_t * packet);
	bool SCP_packet_in_buffer(SCP_pack_t * packet);
	void SCP_timeout_handler(void);
	void SCP_packet_timeout(SCP_pack_t * packet);
	void SCP_Buffer_check(void);
	bool SCP_packet_from_sd(SCP_pack_t * packet);
#endif //__SCP_BUFFER_H__
