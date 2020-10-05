#include <Board/Def_config.h>
#if ((PAYING_MODULE == 1) && (RESERVATION_MODULE == 1) && (CHECK_AVAILABILITY_MODULE == 1))
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/protocol/Net/SCP/tRFIDmsg.h>
#include <Core/Process/process.h>
#include <Core/Thread/pt.h>
#include <Screen/SCR_Message.h>
#include <Screen/SCR_Text.h>
#include <Screen/Transaction/SCR_Transaction.h>

PT_THREAD(Reservation_availability(struct pt * pt,Transaction_info_t * trans, process_event_t ev, process_data_t data)){
	SCP_pack_t * SCP_packet;

	PT_BEGIN(pt)
	Please_wait_screen(true);
	PT_SCP_MSG_MACHINE_AVAILABLE(pt,&SCP_packet,trans->machine_type);
	Please_wait_screen(false);
	if(SCP_packet->Data.Message_type == msg_Machine_available){
		if(SCP_msg_Machine_available__Akkoord(SCP_packet) != 1){
			PT_MESSAGE_SCREEN(pt,GET_TEXT(S_All_VAR_are_reserved),machine_type_name(g_machine_type_choise, g_Language,PLURAL));
			trans->reservation = RESERVATION_CANCELD;
			trans->min_available = 0;
		}else{
			trans->resv_pending = SCP_msg_Machine_available__ResvPending(SCP_packet);
			trans->min_available = SCP_msg_Machine_available__min(SCP_packet);
		}
	}else{
		trans->reservation = RESERVATION_CANCELD;
		trans->min_available = 0;
		PT_MESSAGE_SCREEN(pt,GET_TEXT(S_Server_error));
	}
	PT_END(pt)
}
#endif
