#ifndef __SCP_COMM_H__
#define __SCP_COMM_H__

	#include "SCP_types.h"
	#include "SCP_Device.h"
	#include "SCP_Buffer.h"
	#include "Group_id.h"
	#include "SCP_msg.h"
	#include <stdint.h>
	#include <stdbool.h>
	#include <ctype.h>
	#include <stdio.h>
	#include <Core/Process/process.h>
	#include <Core/protocol/Net/uip.h>
	#include <core/Timer/timer.h>

	PROCESS_NAME(SCP_process);
	//General Status bytes DevInfo
	//Status1
	#define COM_LOST (1 << 8)


	extern process_event_t event_SCP;
	extern uint16_t SCP_msgID;

	/**
	  * @brief Error state
	  */
	 SCP_error_t g_SCP_error;
	#define SCP_Offline() (g_SCP_error != E_NONE)

	/**
	 * @brief Init server connection. Run Net_Dev_init(); before this function
	 * @param groupnr device type
	 * @param device_reset Reset when there is no connection
	 * @return void
	 */
	 void SCP_Init(GroupId_t groupnr, bool device_reset);


	/*
	 * @brief set or reset device status byte. This function informs the server the device status with a definfo
	 * @status_flags 16bit with device status flags
	 * @Set Set bytes or reset bytes
	 */
	void SCP_Set_status_byte(uint16_t status_flags, bool Set);


	/*
	 * @brief return device information
	 */
	SCP_DevInfo_t SCP_Get_device_info(void);

	/*
	 *@brief increase message group number
	 */
	uint16_t SCP_start_new_group(void);

	/*
	 *@brief get current message group number
	 */
	uint16_t SCP_get_group(void);

	/*
	 * @brief send next message as defined in group_nr
	 */
	void SCP_Send_as_Group(uint16_t group_nr);

	/*
	 * @brief add SCP handler. Call this function in a function called void SCP_handlers_init(void)
	 */
	bool SCP_add_new_handler( tRFIDmsg  Message_type, SCP_Data_t (*handler)(SCP_Device_t * packet_data));

#endif //__SCP_COMM_H__
