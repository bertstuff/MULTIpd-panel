#ifndef __TIMER_FUNCTIONS_H__
#define __TIMER_FUNCTIONS_H__

	#include "etimer.h"

	extern struct etimer SCR_timeout_timer;
	extern struct etimer delay_timer;
	extern struct etimer ret_tm;

	/*
	 * @brief set the time in sec to fire a PROCESS_EVENT_SCREEN_TIMEOUT event
	 */
	#define SET_SCREEN_TIMEOUT(SEC) 					etimer_set(&SCR_timeout_timer,CLOCK_SECOND * SEC)
	#define PROCESS_EVENT_SCREEN_TIMEOUT 				((ev == PROCESS_EVENT_TIMER) && etimer_expired(&SCR_timeout_timer))


	/*
	 * @brief delay process or thread in ticks
	 */
	#define PROCESS_PT_DELAY(TIME)						etimer_set(&delay_timer,TIME); 							\
														PROCESS_WAIT_UNTIL(etimer_expired(&delay_timer))

	#define PT_DELAY(PT,TIME)							etimer_set(&delay_timer,TIME);							 \
														PT_WAIT_UNTIL(PT,etimer_expired(&delay_timer))

#endif

