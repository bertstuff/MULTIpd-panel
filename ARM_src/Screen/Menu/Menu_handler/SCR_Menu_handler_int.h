#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#ifndef __SCR_MENU_HANDLER_INT_H__
#define __SCR_MENU_HANDLER_INT_H__
	#include <stdbool.h>
	#include <stdint.h>
	#include <stdio.h>
	#include <stdlib.h>
	#include <Core/Thread/pt.h>
	#include <Core/Process/process.h>

	/*-------------Variable-----------------*/
 	extern int32_t cur_int_val;
	/*-------------Macro-----------------*/
	#define nDigits(VAL) (VAL == 0 ? 1 : ((int)(log10(fabs(VAL))+1)))

	/*------------Functions-------------*/
	extern PT_THREAD(SCR_int_handler(process_event_t ev, process_data_t data));

#endif //__SCR_MENU_HANDLER_H__
#endif
