#include <Board/Def_config.h>
#if (PAYING_MODULE == 1)
#ifndef __TRANSACTION_ATM_H__
#define __TRANSACTION_ATM_H__

	#include "SCR_Transaction.h"
	extern struct pt ATM_pt;

	PT_THREAD(ATM_thread(Transaction_info_t * trans, process_event_t ev, process_data_t data));

#endif //__TRANSACTION_ATM_H__
#endif
