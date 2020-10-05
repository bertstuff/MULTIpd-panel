#include "build_defs.h"

struct tm get_build_date(void){
	struct tm build_date;
	char buffer[12];

	build_date.tm_sec =  0x00;
	buffer[0] = BUILD_MIN_CH0;
	buffer[1] = BUILD_MIN_CH1;
	buffer[2] = '\0';
	build_date.tm_min =  atoi(buffer);
	buffer[0] = BUILD_HOUR_CH0;
	buffer[1] = BUILD_HOUR_CH1;
	buffer[2] = '\0';
	build_date.tm_hour = atoi(buffer);
	buffer[0] = BUILD_YEAR_CH0;
	buffer[1] = BUILD_YEAR_CH1;
	buffer[2] = BUILD_YEAR_CH2;
	buffer[3] = BUILD_YEAR_CH3;
	buffer[4] = '\0';
	build_date.tm_year = atoi(buffer);
	buffer[0] = BUILD_MONTH_CH0;
	buffer[1] = BUILD_MONTH_CH1;
	buffer[2] = '\0';
	build_date.tm_mon = atoi(buffer);
	buffer[0] = BUILD_DAY_CH0;
	buffer[1] = BUILD_DAY_CH1;
	buffer[2] = '\0';
	build_date.tm_mday = atoi(buffer);
	build_date.tm_isdst = -1;
	build_date.tm_wday = -1;
	build_date.tm_yday = -1;

	return build_date;
}

uint32_t get_build_date_number(void){
	uint32_t build_number;
	struct tm build_date = get_build_date();

	build_number = (build_date.tm_mday);
	build_number += (build_date.tm_mon * 100);
	build_number += ((build_date.tm_year - 2000) * 10000);
	return build_number;
}

uint32_t get_build_time_number(void){
	uint32_t build_number;
	struct tm build_date = get_build_date();
	build_number = build_date.tm_min;
	build_number += (build_date.tm_hour * 100);
	return build_number;
}
