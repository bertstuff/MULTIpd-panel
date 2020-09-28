#ifndef _RTP_H_
#define _RTP_H_
#include "RTP_types.h"
#include "RTP_msg.h"
#include <Core/Memory/cmem.h>
#include <stdint.h>

extern process_event_t event_RTP;
extern uint8_t * last_trans;
extern uint16_t last_trans_size;


void RTP_init(void);

PT_THREAD(RTP_Send_thread(RTP_packet_t * packet, process_event_t ev, process_data_t data));

void RTP_Free_packet(RTP_packet_t * packet);

#endif //_RTP_H_
