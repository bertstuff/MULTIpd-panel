#include <Core/Clock/clock.h>
#include <Core/Debug/Error_check.h>
#include <Core/protocol/Net/dns/resolv.h>
#include <Core/Process/process.h>
#include <Core/Timer/etimer.h>
#include <Core/Type/type.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <stdbool.h>
#include <stdint.h>
#include <Screen/SCR_Idle.h>
#include <Screen/SCR_Init.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Processes/dev_reset.h>
#if(BOARD_SCREEN == 1)
#include <Device/Common/LCD_16x2/LCD_16x2.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Display.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Text.h>
#include <Screen/SCR_Message.h>
#endif
#if((SERVICE_BUTTON_MODULE == 1)||(SERVICE_PASS_MODULE == 1))
	#include <Processes/Servicebutton.h>
#endif
/*Global variables*/


/*prototype*/
PROCESS(SCR_Startup_screen, "Startup screen");
void SCR_Startup(bool event);
void Startup_check_error(void);
struct etimer poll_tm;
struct etimer reset_tm;

/*functions*/
PROCESS_THREAD(SCR_Startup_screen, ev, data)
{

	PROCESS_BEGIN();
	etimer_set(&poll_tm, CLOCK_SECOND*5);
	etimer_set(&reset_tm, CLOCK_SECOND * 180);

	while(ev != PROCESS_EVENT_EXIT){

#if((SERVICE_BUTTON_MODULE == 1)||(SERVICE_PASS_MODULE == 1))
		bool exit_scr = false;
		PROCESS_PT_SPAWN(&pin_pt,Servicebutton_handler(&exit_scr, ev, data));
		if(exit_scr){
			PROCESS_EXIT();
		}
#endif

#if(BOARD_SCREEN == 1)
		if(ev == PROCESS_EVENT_TIMER){
			if(etimer_expired(&poll_tm)){
				SCR_Startup(false);
			}

			if(g_screen_type == SCR_EDIP){
				SCR_bussy_bar();
			}
		}

		if(ev == resolv_event_found){
			SCR_Startup(true);
		}
#endif

		if(ev == PROCESS_EVENT_TIMER){
			if(etimer_expired(&reset_tm)){
				Hard_reset();
			}
		}

		if(error_check_no_errors()){
#if(BOARD_SCREEN == 1)
			SCP_DevInfo_t SCP_info = SCP_Get_device_info();
			if((SCP_info.SCP_modnr == 0)||(SCP_info.SCP_locnr == 0)){
				PROCESS_PT_MESSAGE_SCREEN("Module/location\r\n is not set!");
			}
#endif
			SCR_load(&SCR_idle_screen);
			PROCESS_EXIT();
		}else{
			process_add_poll_event(PROCESS_CURRENT());
		}
		PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}
#if(BOARD_SCREEN == 1)
void SCR_Show_error(void){
	error_number_t error_number = 0;
	Point_t point;

	error_number = error_check_next_error();
	if(error_number == 0){
		error_check_start();
		error_number = error_check_next_error();
	}

	switch(g_screen_type){
		case SCR_EDIP:
			point.x = 120;
			point.y = 100;
			Edip_Set_Font(&StdFont);
			Edip_Draw_String(point,"  Error code: %d  ",error_number);
			break;
		case SCR_LCD_2X16:
			point.x = 0;
			point.y = 1;
			LCD_2x16_Draw_String(point,"Error code: %d  ",error_number);
			break;
		case SCR_PC:
			printf("SCR_PC:[error] %d\r\n",error_number);
			break;
	}
	return;
}

void SCR_Startup(bool event){
	uint16_t total = 0, Done = 0;
	uint16_t procent;
	Point_t point;
	static bool init = true;

	if(init == true){
		switch(g_screen_type){
			case SCR_EDIP:
				point.x = 120;
				point.y = 30;

				Edip_Clear_Display();
				Edip_Set_Font(&H1Font);
				Edip_Draw_String(point,"Startup");
				Edip_Set_Font(&SmallFont);
				SCP_DevInfo_t SCP_info = SCP_Get_device_info();
				point.y = 0;
				Edip_Draw_String(point,"Mod:%d Loc:%d DevId:%d",SCP_info.SCP_modnr,SCP_info.SCP_locnr,SCP_info.SCP_devID);
				break;
			case SCR_LCD_2X16:
				point.x = 0;
				point.y = 0;

				LCD_2x16_Clear_Display();
				LCD_2x16_Draw_String(point,"Startup");
				break;
			case SCR_PC:
				printf("SCR_PC:[msg] Startup\r\n");
		}
		init = false;
	}

	if(Resolv_progress(&total,&Done)==false){
		procent = (Done * 100)/(total);
		switch(g_screen_type){
			case SCR_EDIP:
				point.x = 120;
				point.y = 100;
				Edip_Set_Font(&StdFont);
				Edip_Draw_String(point,"DNS resolver %d%% ",procent);
				break;
			case SCR_LCD_2X16:
				point.x = 0;
				point.y = 1;
				LCD_2x16_Draw_String(point,"DNS resolver %d%% ",procent);
				break;
			case SCR_PC:
				printf("SCR_PC:[msg] DNS resolver %d%% \r\n",procent);
				break;
		}
	}else{
		if(event == false){
			SCR_Show_error();
		}
		etimer_set(&poll_tm, CLOCK_SECOND*5);
	}
}
#endif
