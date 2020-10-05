#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#ifndef __SCR_MENU_HANDLER_MESSAGE_H__
#define __SCR_MENU_HANDLER_MESSAGE_H__
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <Core/Thread/pt.h>
	#include <Core/Process/process.h>
	#include <Screen/Menu/SCR_Menu_handler.h>

	bool SCR_Message_screen_needed(void);
	extern PT_THREAD(SCR_message_handler(process_event_t ev, process_data_t data));

#endif //__SCR_MENU_HANDLER_MESSAGE_H__
#endif
