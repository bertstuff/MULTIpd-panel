#include "Time_functions.h"
#include <string.h>
#include <stdlib.h>
#include <Core/Util/array.h>
#include <stdio.h>
#define HOUR_SEC 3600

uint8_t Timezone = 1;


bool daylight_saving_time(struct tm * UTC_time){
	uint8_t func;
	uint32_t year;
	struct tm Begin_summer;
	struct tm End_summer;

	Begin_summer = *UTC_time;

	year = UTC_time->tm_year + 1900;
	func = ((uint32_t)(floor((5 * year)/4) - floor(year/100) + floor(year/400)) % 7);

	Begin_summer.tm_year = UTC_time->tm_year;
	Begin_summer.tm_mon = 2;
	Begin_summer.tm_mday = 31 - ((func + 5) % 7);
	Begin_summer.tm_hour = 1;
	Begin_summer.tm_min = 0;
	Begin_summer.tm_sec = 0;

	End_summer.tm_year = UTC_time->tm_year;
	End_summer.tm_mon = 9;
	End_summer.tm_mday = 31 - ((func + 2) % 7);
	End_summer.tm_hour = 1;
	End_summer.tm_min = 0;
	End_summer.tm_sec = 0;

	if((mktime(UTC_time) > mktime(&Begin_summer)) && (mktime(UTC_time) < mktime(&End_summer))){
		return true;
	}

	return false;
}

struct tm * LocalTime(struct tm * UTC_time){
	static struct tm Local_time;

	time_t Cur_Time = mktime(UTC_time);
	Cur_Time += ((Timezone + (daylight_saving_time(UTC_time) == true? 1:0)) * HOUR_SEC);
	Local_time = *gmtime(&Cur_Time);
	return &Local_time;
}

double Time_in_periode(struct tm start_time, struct tm end_time, struct tm cur_time){
	time_t Start_tm, End_tm, Cur_tm;
	double diff_start, diff_end;

	Start_tm = mktime(&start_time);
	End_tm = mktime(&end_time);
	Cur_tm = mktime(&cur_time);

	diff_start = difftime(Cur_tm, Start_tm);
	diff_end = difftime(Cur_tm, End_tm);

	if(diff_start < 0){
		return diff_start;
	}
	if(diff_end > 0){
		return diff_end;
	}

	return 0;
}

double CurTime_in_periode(struct tm start_time, struct tm end_time, bool local_time){
	struct tm cur_time;

	rtc_cal_gettime(&cur_time);
	if(local_time){
		return Time_in_periode(start_time,end_time,*LocalTime(&cur_time));
	}
	return Time_in_periode(start_time,end_time,cur_time);
}

struct tm Time_offset(struct tm time,int32_t offset_sec){
	time_t time_sec;
	time_sec = mktime (&time);
	time_sec += offset_sec;
	return *gmtime(&time_sec);
}


struct tm string_to_time(char * Buffer){
	struct tm time = {.tm_mday = 0,.tm_mon = 0,.tm_year = 0,.tm_hour = 0,.tm_min = 0,.tm_sec = 0};
	char **arr;
	char **arr2;
	uint8_t size;
	uint8_t size2;
	uint8_t i;

	size = explode(&arr, Buffer,' '); //string to array
	for(i = 0; i < size; i++){
		if(strchr (arr[i],'-') != NULL){
			size2 = explode(&arr2, arr[i],'-'); //string to array
			if(size2 > 0){
				time.tm_mday = atoi(arr2[0]);
			}
			if(size2 > 1){
				time.tm_mon = (atoi(arr2[1])-1);
			}
			if(size2 > 2){
				time.tm_year = (atoi(arr2[2])-1900);
			}
			free(arr2);
		}

		if(strchr (arr[i],':') != NULL){
			size2 = explode(&arr2, arr[i],':'); //string to array
			if(size2 > 0){
				time.tm_hour = atoi(arr2[0]);
			}
			if(size2 > 1){
				time.tm_min = atoi(arr2[1]);
			}
			if(size2 > 2){
				time.tm_sec = atoi(arr2[2]);
			}
			free(arr2);
		}
	}
	free(arr);
	return time;
}

void print_datetime(void){
	struct tm Date_Time;
	rtc_cal_gettime(&Date_Time);
	printf ( "RTC: The current date/time is: %s\r\n", asctime (LocalTime(&Date_Time)) );
}
