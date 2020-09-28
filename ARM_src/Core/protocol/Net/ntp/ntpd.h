
#ifndef __NTPD_H__
#define __NTPD_H__


#include <string.h>
#include <Core/protocol/Net/uip-udp-packet.h>
#include <Core/Peripherals/Net_Dev_Init.h>

#define NTP_EPOCH            (86400U * (365U * 70U + 17U))
#define NTPD_PORT             123

struct ntpformat{
  char padding[40];
  unsigned long seconds;
  unsigned long fractions;
};

extern bool Time_set;
extern bool RTC_ok;
PROCESS_NAME(ntpd_process);

/*
 * @brief start time protocol
 */
void ntpd_init(void);

/*
 * @brief get current time
 * @return time in seconds
 */
unsigned long getCurrTime(void);


/*
 * @brief check is time is set
 * @return true when time is set
 */
bool ntpdTimeSet(void);


#endif /* __NTPD_H__ */

