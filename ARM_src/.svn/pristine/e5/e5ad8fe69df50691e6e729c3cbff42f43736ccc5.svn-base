#ifndef _RTP_MSG_H_
#define _RTP_MSG_H_

#include <Processes/Global_UID.h>

#define PTR_msg_Echo(PACK) RTP_Send_thread(_PTR_msg_Echo(PACK), ev, data)
RTP_packet_t * _PTR_msg_Echo(RTP_packet_t * packet);

#define PTR_msg_Set_Message(PACK,DISPLAY,STRING) RTP_Send_thread(_PTR_msg_Set_Message(PACK,DISPLAY,STRING), ev, data)
RTP_packet_t * _PTR_msg_Set_Message(RTP_packet_t * packet, uint8_t display,char * string);

#define PTR_msg_EMV_Transaction(PACK,AMOUNT) RTP_Send_thread(_PTR_msg_EMV_Transaction(PACK,AMOUNT), ev, data)
RTP_packet_t * _PTR_msg_EMV_Transaction(RTP_packet_t * packet, uint32_t amount);

bool PTR_EMV_Transaction_event_Get_UID(RTP_packet_t * packet, UID_t * UID);

#define PTR_msg_EMV_Cancel(PACK) RTP_Send_thread(_PTR_msg_EMV_Cancel(PACK), ev, data)
RTP_packet_t * _PTR_msg_EMV_Cancel(RTP_packet_t * packet);

#define PTR_msg_EMV_Clearing(PACK,CAPTURE) RTP_Send_thread(_PTR_msg_EMV_Clearing(PACK,CAPTURE), ev, data)
RTP_packet_t * _PTR_msg_EMV_Clearing(RTP_packet_t * packet, bool Capture);

#define PTR_msg_EMV_ICR_Transaction(PACK) RTP_Send_thread(_PTR_msg_EMV_ICR_Transaction(PACK), ev, data)
RTP_packet_t * _PTR_msg_EMV_ICR_Transaction(RTP_packet_t * packet);

#define PTR_msg_EMV_ICR_Online(PACK,STATUS,MESSAGE) RTP_Send_thread(_PTR_msg_EMV_ICR_Online(PACK,STATUS,MESSAGE), ev, data)
RTP_packet_t * _PTR_msg_EMV_ICR_Online(RTP_packet_t * packet, bool Status, char * message);

/*
 * @brief returns the transaction cost
 * @param amount to pay
 * @retrun extra cost in centen
 */
uint32_t RTP_transaction_cost(uint32_t amount);

/*
 * @brief start a transaction
 * @param PACK packet handle (free handle after use with RTP_Free_packet)
 * @param AMOUNt  amount to pay in centen (transaction cost will be added)
 */
#define PROCESS_PT_PTR_MSG_EMV_TRANSACTION(PACK, AMOUNT)	PROCESS_PT_SPAWN(PACK.p, PTR_msg_EMV_Transaction(PACK,AMOUNT))
#define PT_PTR_MSG_EMV_TRANSACTION(PT, PACK, AMOUNT)		PT_SPAWN(PT, PACK.p, PTR_msg_EMV_Transaction(PACK,AMOUNT))

/*
 * @brief cancel transaction
 * @param PACK packet handle (free handle after use with RTP_Free_packet)
 */
#define PROCESS_PT_PTR_MSG_EMV_CANCEL(PACK)					PROCESS_PT_SPAWN(PACK.p, PTR_msg_EMV_Cancel(PACK))
#define PT_PTR_MSG_EMV_CANCEL(PT, PACK)						PT_SPAWN(PT, PACK.p, PTR_msg_EMV_Cancel(PACK))

/*
 * @brief Deliver product failed or succes.
 * @param PACK packet handle (free handle after use with RTP_Free_packet)
 * @param CAPTURE Accept or fail last transaction
 */
#define PROCESS_PT_PTR_MSG_EMV_CLEARING(PACK, CAPTURE)		PROCESS_PT_SPAWN(PACK.p, PTR_msg_EMV_Clearing(PACK,CAPTURE))
#define PT_PTR_MSG_EMV_CLEARING(PT, PACK, CAPTURE)			PT_SPAWN(PT, PACK.p, PTR_msg_EMV_Clearing(PACK,CAPTURE))

#define PROCESS_PT_PTR_MSG_EMV_ICR_TRANSACTION(PACK)		PROCESS_PT_SPAWN(PACK.p, PTR_msg_EMV_ICR_Transaction(PACK))
#define PT_PTR_MSG_EMV_ICR_TRANSACTION(PT, PACK)			PT_SPAWN(PT, PACK.p, PTR_msg_EMV_ICR_Transaction(PACK))

#define PROCESS_PT_PTR_MSG_EMV_ICR_ONLINE(PACK, STATUS, MESSAGE)		PROCESS_PT_SPAWN(PACK.p, PTR_msg_EMV_ICR_Online(PACK,STATUS,MESSAGE))
#define PT_PTR_MSG_EMV_ICR_ONLINE(PT, PACK, STATUS, MESSAGE)			PT_SPAWN(PT, PACK.p, PTR_msg_EMV_ICR_Online(PACK,STATUS,MESSAGE))



#endif //_RTP_MSG_H_
