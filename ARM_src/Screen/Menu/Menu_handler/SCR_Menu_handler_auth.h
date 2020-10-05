#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#ifndef __SCR_MENU_HANDLER_AUTH_H__
#define __SCR_MENU_HANDLER_AUTH_H__
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <Core/Thread/pt.h>
	#include <Core/Process/process.h>
	#include <Screen/Menu/SCR_Menu_handler.h>

	extern uint8_t Cur_auth_lev;
	extern uint32_t cur_license_code;
	extern uint32_t cur_Service_code;
	extern uint32_t cur_admin_code;

	bool SCR_authorization_screen_needed(void);
	extern PT_THREAD(SCR_auth_handler(process_event_t ev, process_data_t data));

#endif //__SCR_MENU_HANDLER_AUTH_H__
#endif
