#include "Service_IO.h"
#include <Core/Process/process.h>
#include <Core/Timer/timer.h>
#include <Core/protocol/Net/AMS/AMS_command.h>
#include <Core/protocol/Net/AMS/AMS.h>
#include <stdio.h>
#include <Driver/include/lpc17xx_wdt.h>
#include <Core/Peripherals/Net_Dev_Init.h>

uint32_t idle_count = 0;

PROCESS(Service_process, "Process for debug and service");

void Service_init(void){
	process_start(&Service_process,NULL);
}

PROCESS_THREAD(Service_process, ev, data)
{
	static struct timer deadband;
	static struct timer led_timer;
	static bool led;

	PROCESS_BEGIN();
	GPIO_SetDir(BOARD_SERVICE_PORT,(uint32_t)BOARD_SERVICE_PIN, DIR_OUTPUT);
	GPIO_SetValue(BOARD_SERVICE_PORT,(uint32_t)BOARD_SERVICE_PIN);

	if(Servicepin() == true){
		printf("Enter service mode\r\n");
#if(AMS_MODULE == 1)
		Net_Dev_init();
		AMS_init();
		AMS_send_command(AMS_servicepin_msg(false));
#endif
		while (1){
		  process_run();
		  idle_count++;
		  WDT_Feed();
		  set_Servicepin_led((idle_count&(1<<16)));
		}
	}

	while (ev != PROCESS_EVENT_EXIT){ //main loop  till exit
		if(Servicepin() == true){
			if(timer_expired(&deadband)){
#if(AMS_MODULE == 1)
				AMS_send_command(AMS_servicepin_msg(false));
#endif
				timer_set(&deadband,CLOCK_SECOND*2);
			}else{
				set_Servicepin_led(true);
				timer_set(&deadband,CLOCK_SECOND);
			}
		}
		idle_count++;

		if((idle_count > 10000) || timer_expired(&led_timer)){
			WDT_Feed();
			led = !led;
			Debug_led1(led);
			idle_count = 0;
			timer_set(&led_timer,CLOCK_SECOND);
		}
		process_add_poll_event(PROCESS_CURRENT());
		PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}
