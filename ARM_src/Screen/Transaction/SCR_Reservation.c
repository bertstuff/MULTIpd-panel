#include <Board/Def_config.h>
#if ((PAYING_MODULE == 1) && (RESERVATION_MODULE == 1))
#include <stdbool.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/Process/process.h>
#include <Core/Thread/pt.h>
#include <Screen/SCR_Text.h>
#include <Screen/Transaction/SCR_Reservation_availability.h>
#include <Screen/Transaction/SCR_Transaction.h>
#include <Screen/Transaction/SCR_Transaction_products.h>
#include <Screen/Transaction/SCR_Transaction_Reservation.h>
#include <Screen/SCR_Message.h>
#include "SCR_Reservation.h"

/*
 * @ brief main reservation thread
 */
PT_THREAD(Reservation_thread(Transaction_info_t * trans,process_event_t ev, process_data_t data)){
	PT_BEGIN(&Transaction_pt);

	trans->payment_success = false;

	//Reservation?
	PT_SPAWN(&Transaction_pt,&Transaction_sub_pt,reservation_transaction(trans, ev, data));
	//Available?
	#ifdef CHECK_AVAILABILITY_MODULE
	if((trans->check_availability == true)&&(trans->reservation == RESERVATION_NO)){
		PT_SPAWN(&Transaction_pt, &Transaction_sub_pt, Reservation_availability(&Transaction_sub_pt, trans, ev, data));
	}
	#endif

	PT_END(&Transaction_pt);
}
#endif
