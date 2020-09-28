

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#include "Driver/include/lpc17xx_wdt.h"
#endif

#include <Core/Debug/Serial_Terminal.h>
#include <Core/Debug/Error_check.h>
#include <Core/Clock/clock.h>
#include <Core/Clock/rtc_cal.h>
#include <Core/Timer/etimer.h>
#include <Core/Timer/ctimer.h>
#include <Core/Memory/cmem.h>
#include <Core/Process/process.h>
#include <Device/IO/SERVICE/Service_IO.h>
#include <Core/Debug/Debug_Terminal.h>
#include <Core/Debug/Hard_Fault_handler.h>
#include <Core/protocol/FAT/ff.h>
#include <Board/Def_config.h>

void Default_init(void){
	//WDT init
#ifndef DEBUGGING
	WDT_Init (WDT_CLKSRC_IRC, WDT_MODE_RESET);
	WDT_Start(5000000);
#endif
	SCB->VTOR = (0x4000 & 0x1FFFFF80);

	//critical init
	clock_init();
	cmem_init();
	Serial_Terminal_init();
	process_init();
	ctimer_init();
	process_start(&etimer_process, NULL);//Handels event driven timers
	Debug_led_init();
#if (BOARD_SD_CARD == 1)
	if(f_mount(&SD_Fatfs,"SD1:",1) == FR_OK){
		printf("Mount SD1: ok\r\n");
	}else{
		printf("Mount SD1: error\r\n");
		 Debug_led2(true);
		while(1){
			;
		}
	}
#endif
	Service_init();
	error_check_init();
}
