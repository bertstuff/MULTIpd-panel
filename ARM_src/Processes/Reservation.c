#include "Reservation.h"
#if (RESERVATION_MODULE == 1)

#include <Core/Clock/Time_functions.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/protocol/Net/SCP/tRFIDmsg.h>
#include <Core/Process/process.h>
#include <Core/Thread/pt.h>
#include <Global.h>
#include <Screen/SCR_Text.h>
#include <Processes/Global_UID.h>
#include <Screen/SCR_Message.h>

struct pt resv_pt;
int32_t g_Cur_ResvNr;

PT_THREAD(get_reservation_time(Reservation_time_t * resv_time, process_event_t ev, process_data_t data))
{
	SCP_pack_t * Reservation_packet;

	PT_BEGIN(&resv_pt);
	PT_SCP_MSG_GEEFRESERVERING(&resv_pt, &Reservation_packet, g_Cur_UID, g_Cur_ResvNr,MP_SERVER);
	if(Reservation_packet->Data.Message_type == msg_Reservering){
		resv_time->Resv_ok = true;
		resv_time->Start_time = SCP_msg_Reservering__StartTime(Reservation_packet);
		printf("SCP:Start reservation %02d-%02d-%02d %02d:%02d UTC\r\n",resv_time->Start_time.tm_mday,resv_time->Start_time.tm_mon+1,resv_time->Start_time.tm_year+1900,resv_time->Start_time.tm_hour,resv_time->Start_time.tm_min);
		resv_time->End_time = SCP_msg_Reservering__EndTime(Reservation_packet);
		printf("SCP:End reservation %02d-%02d-%02d %02d:%02d UTC\r\n",resv_time->End_time.tm_mday,resv_time->End_time.tm_mon+1,resv_time->End_time.tm_year+1900,resv_time->End_time.tm_hour,resv_time->End_time.tm_min);
		rtc_cal_gettime(&resv_time->Cur_time);
		printf("Reservation:Current time %02d:%02d UTC\r\n",resv_time->Cur_time.tm_hour,resv_time->Cur_time.tm_min);
		resv_time->time_to_end = difftime(mktime(&resv_time->End_time), mktime(&resv_time->Cur_time));
		resv_time->time_to_start = difftime(mktime(&resv_time->Start_time), mktime(&resv_time->Cur_time));
		resv_time->lenght = difftime(mktime(&resv_time->End_time),mktime(&resv_time->Start_time));
	}else if(Reservation_packet->Data.Message_type == msg_Onbekend){
		resv_time->Resv_ok = false;
		printf("SCP: Reservation unknown\r\n");
		PT_MESSAGE_SCREEN(&resv_pt,GET_TEXT(S_Reservation_unknown));
		resv_time->time_to_end = 0;
		resv_time->lenght = 0;
		PT_EXIT(&resv_pt);
	}else{
		resv_time->Resv_ok = false;
		printf("SCP: Error\r\n");
		PT_MESSAGE_SCREEN(&resv_pt,GET_TEXT(S_Server_error));
		resv_time->time_to_end = 0;
		resv_time->lenght = 0;
		PT_EXIT(&resv_pt);
	}
	PT_END(&resv_pt);
}


PT_THREAD(show_reservation_message(Reservation_time_t resv_time ,process_event_t ev, process_data_t data))
{

	PT_BEGIN(&resv_pt);
	if(resv_time.Resv_ok == false){
		PT_EXIT(&resv_pt);
	}

	if(resv_time.time_to_start > 0){
		printf("SCP:User is to early\r\n");
		struct tm local_Start_time = {.tm_min = 0, .tm_hour = 0, .tm_year = 0, .tm_mon = 0, .tm_mday = 0};
		local_Start_time = *LocalTime(&resv_time.Start_time);
		PT_MESSAGE_SCREEN(&resv_pt,GET_TEXT(S_To_early),local_Start_time.tm_mday,local_Start_time.tm_mon+1,local_Start_time.tm_year+1900,local_Start_time.tm_hour,local_Start_time.tm_min);
		g_Language = g_Language_def;
		PT_EXIT(&resv_pt);
	}
	if(resv_time.time_to_end < 0){
		printf("SCP:User is to late\r\n");
		PT_MESSAGE_SCREEN(&resv_pt,GET_TEXT(S_To_late));
		g_Language = g_Language_def;
		PT_EXIT(&resv_pt);
	}

	PT_END(&resv_pt);
}
#endif
