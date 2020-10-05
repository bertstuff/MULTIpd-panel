/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/

#ifdef __USE_CMSIS
#include "LPC17xx.h"
//#include "Driver/include/lpc17xx_wdt.h"
#endif


/* Include files */
#include <Board/Def_init.h>
#include <Core/protocol/Net/SCP/Group_id.h>
#include <Core/Extensions/ini/minIni.h>

#include <Core/Peripherals/Net_Dev_Init.h>
#include <Device/I2C/BUTTONS/Buttons_3x4.h>

#include <Screen/SCR_Init.h>
#include <Screen/SCR_Startup.h>
#include <Screen/Transaction/SCR_Transaction_init.h>

#include "MULTIpd_main_process.h"

#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/ntp/ntpd.h>
#include <Core/protocol/Net/AMS/AMS.h>
#include <Core/Debug/Error_check.h>

#include "Global.h"
#include "Power.h"
uint16_t Server_prefix_nr = 0;

/* Private Variables */
Language_t g_Language;




int main(void) {
	Default_init();
	Power_init();
	SCR_init();
	Net_Dev_init();
	AMS_init();
	SCP_Init(MULTIPD, true);
	ntpd_init();
	keypad_init(0);

	Server_prefix_nr = ini_getl("Device","Server_prefix_nr",0,"increase energy point nr",inifile_machine);

	//Start main applications
	SCR_load(&SCR_Startup_screen);
	error_check_change_priority(ERROR_NTP, LOW_PRIO);
	error_check_change_priority(ERROR_SCP, LOW_PRIO);

	process_start(&MULTIpd_main_process,NULL);
	printf("Enter main loop\r\n");
	while (1){
	  process_run();
	}
	  return 0;
}


