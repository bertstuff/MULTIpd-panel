#include "RTP_types.h"
#include "RTP.h"
#include <Core/protocol/Net/uip.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/Memory/cmem.h>

RTP_packet_t *_PTR_msg_Echo(RTP_packet_t * packet){
	packet->status = S_NOK;
	if(RTP_busy){return packet;}
	packet->frame.AID = 0x00;
	packet->frame.CmdID = ECHO;
	packet->frame.Opt = NO_ENCRYPTION;
	packet->Data = cmem_malloc(5);
	packet->frame.Len = sprintf((char *)packet->Data,"PING");
	return packet;
}

RTP_packet_t * _PTR_msg_Set_Message(RTP_packet_t * packet, uint8_t display, char * string){
	packet->status = S_NOK;
	if(RTP_busy){return packet;}
	packet->frame.AID = 0x00;
	packet->frame.CmdID = SET_MESSAGE;
	packet->frame.Opt = KEY_1;
	packet->Data = cmem_malloc(strlen(string)+2);
	packet->frame.Len = sprintf((char *)packet->Data,"%c%s",display, string);
	return packet;
}

uint32_t int32_to_BCD(uint32_t in){
	uint32_t uiResult = 0;
	uint32_t uiMultiplier = 1000000;

	// get the dec bcd value
	while ( uiMultiplier > 0 )
	{
	   uiResult <<= 4;
	   uiResult |= in / uiMultiplier;
	   in -= (uiMultiplier * (uiResult & 0xF));
	   uiMultiplier /=10;
	}
	return uiResult;
}

uint32_t RTP_transaction_cost(uint32_t amount){
	if(amount < ini_getl("Paymate","transaction_cost_threshold",500,"Transaction cost will be added below this value:",inifile_device)){
		return ini_getl("Paymate","transaction_cost",50,"transaction cost:",inifile_device);
	}else{
		return 0;
	}
}

RTP_packet_t * _PTR_msg_EMV_Transaction(RTP_packet_t * packet, uint32_t amount){
	packet->status = S_NOK;
	if(RTP_busy){return packet;}
	uint64_t bcd_amount;

	if(last_trans != NULL){
		cmem_free(last_trans);
		last_trans = NULL;
	}

	if(amount != 0){
		amount += RTP_transaction_cost(amount);
	}

	packet->frame.AID = 0x00;
	packet->frame.CmdID = EMV_TRANSACTION;
	packet->frame.Opt = KEY_1;
	packet->Data = cmem_malloc(17);
	bcd_amount = int32_to_BCD(amount);

	packet->Data[0]  = 0x9f;
	packet->Data[1]  = 0x02;
	packet->Data[2]  = 0x06;
	packet->Data[3]  = (bcd_amount >> 40);
	packet->Data[4]  = (bcd_amount >> 32);
	packet->Data[5]  = (bcd_amount >> 24);
	packet->Data[6]  = (bcd_amount >> 16);
	packet->Data[7]  = (bcd_amount >> 8);
	packet->Data[8]  = (bcd_amount >> 0);
	packet->Data[9]  = 0x9C;
	packet->Data[10] = 0x01;
	packet->Data[11] = 0x00;
	packet->Data[12] = 0x5F;
	packet->Data[13] = 0x2A;
	packet->Data[14] = 0x02;
	packet->Data[15] = 0x09;
	packet->Data[16] = 0x78;

	packet->frame.Len = 17;
	return packet;
}

RTP_packet_t * _PTR_msg_EMV_Cancel(RTP_packet_t * packet){
	packet->status = S_NOK;
	if(RTP_busy){return packet;}

	packet->frame.AID = 0x00;
	packet->frame.CmdID = EMV_CANCEL;
	packet->frame.Opt = KEY_1;
	packet->Data = NULL;
	packet->frame.Len = 0;
	return packet;
}

bool PTR_EMV_Transaction_event_Get_UID(RTP_packet_t * packet, UID_t * UID){
	uint8_t * chr;
	chr = memchr ( packet->received_Data , 0x5A, packet->received_frame.Len );
	memset(UID->UID,0,7);
	UID->type = NONE_UID;
	if((chr == NULL)||(chr[1] != 0x04)){
		return false;
	}
	memmove(UID->UID,chr+2,4);
	UID->type = RFID_UID;
	printf("PTR: pass UID %02x %02x %02x %02x 00 00 00\r\n",UID->UID[0],UID->UID[1],UID->UID[2],UID->UID[3]);
	return true;
}

RTP_packet_t * _PTR_msg_EMV_Clearing(RTP_packet_t * packet, bool Capture){
	packet->status = S_NOK;
	if(last_trans == NULL){
		packet->status = S_INVALID_STATE;
		printf("PTR: no Transaction to cancel/approve \r\n");
		return packet;
	}
	if(RTP_busy){return packet;}
	packet->frame.AID = 0x00;
	packet->frame.CmdID = EMV_CLEARING;
	packet->frame.Opt = KEY_1;
	packet->Data = last_trans;
	packet->Data[0]  = (Capture == true)?1:0;
	packet->frame.Len = last_trans_size + 1;
	return packet;
}


RTP_packet_t * _PTR_msg_EMV_ICR_Transaction(RTP_packet_t * packet){
	packet->status = S_NOK;
	if(RTP_busy){return packet;}

	packet->frame.AID = 0x00;
	packet->frame.CmdID = EMV_ICR_TRANSACTION;
	packet->frame.Opt = KEY_1;
	packet->Data = NULL;
	packet->frame.Len = 0;
	return packet;
}

RTP_packet_t * _PTR_msg_EMV_ICR_Online(RTP_packet_t * packet, bool Status, char * message){
	uint8_t ptr;
	uint8_t * data;

	packet->status = S_NOK;
	if(last_trans == NULL){
		packet->status = S_INVALID_STATE;
		printf("PTR: no Transaction to approve/decline \r\n");
		return packet;
	}
	if(RTP_busy){return packet;}

	packet->frame.AID = 0x00;
	packet->frame.CmdID = EMV_ICR_ONLINE;
	packet->frame.Opt = KEY_1;
	packet->Data = last_trans;
	packet->Data[0] = (Status == true)?1:2;
	ptr = last_trans_size+1;
	if(message != NULL){
		//relocate memory
		data = cmem_malloc(last_trans_size + 4 + strlen(message));
		if(data != NULL){
			memmove(data, last_trans,last_trans_size+1);
			cmem_free(last_trans);
			last_trans = data;
			packet->Data = last_trans;
			packet->Data[ptr++] = 0xDF;//DFCA01
			packet->Data[ptr++] = 0xCA;
			packet->Data[ptr++] = 0x01;
			packet->Data[ptr++] = strlen(message);
			memmove(&packet->Data[ptr], message, strlen(message));
			ptr += strlen(message);
		}
	}
	packet->frame.Len = ptr;
	return packet;
}



