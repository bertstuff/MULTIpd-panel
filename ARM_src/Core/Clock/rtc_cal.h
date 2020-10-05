#ifndef RTC_CAL_H
#define RTC_CAL_H

#include <time.h>
#include <Driver/include/lpc17xx_rtc.h>

/* Definitions needed to calculate the calibration value and direction */
#define RTC_IDEAL_FREQ        32768000     //in mHz (milli Hz) (=32.768kHz)
#define RTC_ACTUAL_FREQ       32768000

void rtc_cal_init(struct tm  *pFullTime);

void rtc_cal_config(uint32_t actual_rtc_frequency);

void rtc_cal_settime(struct tm  *pFullTime);
uint8_t rtc_cal_gettime(struct tm  *pFullTime);



#endif

