#include "ntpd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <Core/Clock/rtc_cal.h>
#include <Core/Clock/Time_functions.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/Debug/Error_check.h>
#include <time.h>
#include <math.h>

#define UDP_HDR ((struct uip_udpip_hdr *)&uip_buf[UIP_LLH_LEN])

#define PROGMEM

static unsigned int SEND_INTERVAL=2;
static unsigned long StartTime=0;
static unsigned long CurrTime=0;

static const char ntpmsg[] PROGMEM = {
  0x00000008U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
  0x00000000U, 0x00000000U, 0x00000000U, 0x00000000U,
};

static struct uip_udp_conn *udpconn;
bool Time_set = false;
bool RTC_ok = true;

void * NTP_Resolv_handel;


bool ntpdTimeSet(void){
	return Time_set;
}

void ntpd_init(void){
	if(Net_settings.Offline == true){
		return;
	}
	process_start(&ntpd_process, NULL);
}

/*---------------------------------------------------------------------------*/
void ntpdSetRTC(void){
	time_t timer;
	timer = getCurrTime();
	struct tm * cur_time = gmtime(&timer);
	if(Time_set == false){
		rtc_cal_init(cur_time);
		printf("NTP: RTC enabled \r\n");
		struct tm Date_Time;
		rtc_cal_gettime(&Date_Time);
		printf ( "NTP: The current date/time is: %s\r\n", asctime (LocalTime(&Date_Time)) );
		Time_set = true;
	}else{
		struct tm rtc_time;
		rtc_cal_gettime(&rtc_time);
		if(timer != mktime(&rtc_time)){
		  if(RTC_ok == true){
			RTC_ok = false;
			printf("RTC:error %d %d\r\n", timer, mktime(&rtc_time));
			rtc_cal_init(cur_time);
		  }
		}else{
			if(RTC_ok == false){
				RTC_ok = true;
				printf("RTC:fixed\r\n");
			}
		}
		rtc_cal_settime(gmtime(&timer));
	}
	return;
}
/*---------------------------------------------------------------------------*/
unsigned long
getCurrTime(void)
{
  if(StartTime==0 && CurrTime==0)
    return 0;
  return (clock_seconds() - StartTime + CurrTime);
}
/*---------------------------------------------------------------------------*/
bool udphandler(process_event_t ev, process_data_t data)
{
    if (uip_newdata())
    {
        //printf("NTP: Received from %u.%u.%u.%u:%u: '%s'\n", uip_ipaddr_to_quad(&UDP_HDR->srcipaddr), UIP_HTONS(UDP_HDR->srcport), (char*)uip_appdata);
        if(uip_datalen()==48){
           CurrTime = uip_ntohl(((struct ntpformat *)uip_appdata)->seconds) - NTP_EPOCH;
           StartTime = clock_seconds();
           ntpdSetRTC();
           return true;
        }
    }
    return false;
}

/*---------------------------------------------------------------------------*/
static void
timeout_handler(void)
{
  uip_udp_packet_send(udpconn, ntpmsg, 48);
}
/*---------------------------------------------------------------------------*/
/*
* We declare the process.
*/
PROCESS(ntpd_process, "ntpd");
/*---------------------------------------------------------------------------*/
/*
* The definition of the process.
*/
PROCESS_THREAD(ntpd_process, ev, data)
{
	static struct etimer et;
	static uint8_t ntp_server = 0;
	static uint8_t retry = 0;
    PROCESS_BEGIN();

    uip_ipaddr_t NTP_server;
    error_check_add(TYPE_STATUS,&Time_set,ERROR_NTP);
    error_check_add(TYPE_STATUS,&RTC_ok,ERROR_RTC);
    error_check_lower_priority(ERROR_RTC,0);
    Timezone = ini_getl("Time servers","Timezone",1,"The current timezone:",inifile_net);

    while(1){
    	if(Net_settings.setup_done == true){
    		break;
    	}
    	process_add_poll_event(PROCESS_CURRENT());
    	PROCESS_YIELD();
    }

    while(1){
    	ntp_get_server:
    	switch(ntp_server){
    	case 0 :
    		ini_resolv_ipaddr(&NTP_Resolv_handel,"Time servers", "Server_URL1", "ntp1.custommate.eu","URL or ip time server:", inifile_net);/* ntp1.ptb.de */
    		break;
    	case 1:
    		ini_resolv_ipaddr(&NTP_Resolv_handel,"Time servers", "Server_URL2", "ntp2.custommate.eu","URL or ip time server:", inifile_net);/* ntp1.ptb.de */
    		break;
    	default:
    		ini_resolv_ipaddr(&NTP_Resolv_handel,"Time servers", "Server_URL3", "pool.ntp.org","URL or ip time server:", inifile_net);/* ntp1.ptb.de */
    		break;
    	}
    	etimer_set(&et, 30 * CLOCK_SECOND);
		while(1){
				if(get_resolv_ipaddr(&NTP_Resolv_handel, &NTP_server, data)){
					udpconn = udp_new(&NTP_server, UIP_HTONS(NTPD_PORT), NULL);

					//Bind the UDP 'connection' to the port 50000. That's the port we're listening on.
					udp_bind(udpconn, UIP_HTONS(50000));
					printf("NTP: connecting to %d.%d.%d.%d on port %d listening on port %u\r\n",uip_ipaddr_to_quad(&NTP_server), NTPD_PORT,UIP_HTONS(udpconn->lport));

					etimer_set(&et, CLOCK_SECOND);
					break;
				}else{
					PROCESS_WAIT_EVENT();
				}

				if(etimer_expired(&et)){
					ntp_server++;
					goto ntp_get_server;
				}
		}
		while(1) {
			PROCESS_WAIT_EVENT();

			if(etimer_expired(&et)) {
			  timeout_handler();
			  retry++;

			  if((clock_seconds() > 4294967290U) || (clock_seconds() < 20) || (Time_set == false)){
				SEND_INTERVAL = 2 * CLOCK_SECOND;
				etimer_set(&et, SEND_INTERVAL);
				if(retry > 10){
					printf("NTP: Retry\r\n");
					retry = 0;
					ntp_server++;
					goto ntp_get_server;
				}
			  }else{
				if(SEND_INTERVAL <= 51200 && (getCurrTime() != 0)) {
					SEND_INTERVAL = 2 * SEND_INTERVAL;
				}
				if(RTC_ok == false){
					SEND_INTERVAL = 10 * CLOCK_SECOND;
				}
				etimer_set(&et, SEND_INTERVAL);
			  }
			}


			if(ev == tcpip_event){
				if(udphandler(ev, data) == true){
					ntp_server = 0;
				}
				retry = 0;
			}

		}
    }
    PROCESS_END();
}
/*---------------------------------------------------------------------------*/
