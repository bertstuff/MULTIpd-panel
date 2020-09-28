#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#ifndef __SCR_MENU_HANDLER_QUESTION_H__
#define __SCR_MENU_HANDLER_QUESTION_H__
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <Core/Thread/pt.h>
	#include <Core/Process/process.h>

	/*------------Functions-------------*/
	extern PT_THREAD(SCR_question_handler(process_event_t ev, process_data_t data));

#endif //__SCR_MENU_HANDLER_QUESTION_H__
#endif
