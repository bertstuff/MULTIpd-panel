#ifndef __TIME_FUNCTIONS_h__
#define __TIME_FUNCTIONS_h__

	#include <stdint.h>
	#include <stdbool.h>
	#include <time.h>
	#include <math.h>
	#include "rtc_cal.h"

	extern uint8_t Timezone;

	struct tm * LocalTime(struct tm * UTC_time);

	/*
	 * @brief check if periode is buzzy
	 * @param start_time start time of periode UTC
	 * @param end_time start time of periode UTC
	 * @return
	 * 0 = cur time between periode.
	 * 0 > seconds after periode.
	 * 0 < senconds before periode.
	 */
	double Time_in_periode(struct tm start_time, struct tm end_time, struct tm cur_time);

	double CurTime_in_periode(struct tm start_time, struct tm end_time, bool local_time);

	/*
	 * @brief Create time offset
	 * @param time time to change
	 * @param offset_sec offset in seconden
	 * @return time with offset
	 */
	struct tm Time_offset(struct tm time,int32_t offset_sec);

	/*
	 * @brief time/date string to time
	 * @param Buffer pointer to string
	 * @return time
	 */
	struct tm string_to_time(char * Buffer);
	
	/*
	* @brief print current system date and time to console
	*/
	void print_datetime(void);
#endif// __TIME_FUNCTIONS_h__
