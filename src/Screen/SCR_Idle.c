#include "SCR_Overview.h"
#include "SCR_Idle.h"
#include "SCR_Language.h"
#include <Core/Extensions/ini/minIni.h>
#include <Core/Process/process.h>
#include <core/Timer/etimer.h>
#include <Processes/dev_reset.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Device/I2C/BUTTONS/Buttons_3x4.h>
#include <Screen/Transaction/SCR_Transaction.h>
#include <Screen/SCR_Message.h>
#include <Global.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "Energy_point.h"

/*prototype*/
PROCESS(SCR_idle_screen, "idle screen");
void SCR_idle(void);
void SCR_idle_input(void);
int16_t g_Current_energy_point;
bool scroll_screen = false;

//screen process
PROCESS_THREAD(SCR_idle_screen, ev, data)
{
	static struct etimer poll_tm;
	static struct etimer language_tm;
	static struct timer scroll_tm;

	if(etimer_expired(&poll_tm)){
		etimer_set(&poll_tm,CLOCK_SECOND);
	}

	PROCESS_BEGIN();
	etimer_set(&poll_tm,CLOCK_SECOND);
	g_Current_energy_point = 0;
	SCR_idle();
	Button3x4_activate(true);
	etimer_set(&language_tm,CLOCK_SECOND *15);
	timer_set(&scroll_tm,CLOCK_SECOND * 7);
	while(ev != PROCESS_EVENT_EXIT){
		Save_reset_poll();
		if(ev == event_state_update){
			SCR_idle();
			timer_set(&scroll_tm,CLOCK_SECOND * 7);
		}

		if(ev == PROCESS_EVENT_TIMER){
			if(etimer_expired(&language_tm)){
				if(g_Language != NL){
					g_Language = NL;
					SCR_idle();
				}
			}

			if(timer_expired(&scroll_tm)){
				SCR_idle();
				timer_set(&scroll_tm,CLOCK_SECOND * 7);
			}
		}
		//keypad
		if(ev == event_Button3x4_pressed){
			uint8_t key = keypad_button(ev, data);
			if(key == C_RETURN){ //Clear key
				g_Current_energy_point = 0;
				SCR_load(&SCR_language_screen);
				PROCESS_EXIT();
			}else if (key == C_OK){ //confirm key
				if(Energy_point_exist(g_Current_energy_point)){
					SCR_load(&SCR_Overview_screen);
					PROCESS_EXIT();
				}else{
					PROCESS_PT_MESSAGE_SCREEN(GET_TEXT(S_Energy_point_does_not_exist));
					g_Current_energy_point = 0;
					//Clear input and reprint screen
					SCR_idle();
				}
			}else if(key < 10){ // number key
				etimer_set(&language_tm,CLOCK_SECOND *15);
				uint32_t new_input = (g_Current_energy_point * 10) + (key);
				g_Current_energy_point = new_input;
				if(g_Current_energy_point > 999){
					g_Current_energy_point = 0;
				}
			}
			SCR_idle_input();
		}


		PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}

void SCR_idle(void){
	static struct Energy_point_t * Energy_point = NULL;
	uint8_t msg_nr = 0;
	Point_t point1 = {.x=120,.y=5};
	Point_t Line_1 = {.x=0,.y=20};
	Point_t Line_2 = {.x=240,.y=20};

	Edip_Clear_Display();
	Edip_Change_Font_Setting(&StdFont,ALIGN,A_CENTER);
	Edip_Set_Font(&StdFont);

	scroll_screen = (Energy_point == NULL)?false:true;
	while(1){
		Energy_point = Energy_point_loop(Energy_point);
		if(Energy_point == NULL){
			//if(!((scroll_screen == true)&&(msg_nr == 0))){
				break;
			//}
		}
		switch(Energy_point->data.State){
		case ST_OVERCURRENT:
			Edip_Draw_String(point1,"[%d] %s", Energy_point->number, GET_TEXT(S_Energy_point_overload));
			Edip_Draw_Line(Line_1, Line_2);
			point1.y += 20;
			Line_1.y += 20;
			Line_2.y += 20;
			msg_nr++;
			break;
		case ST_NO_KWH_LEFT:
			Edip_Draw_String(point1,"[%d] %s", Energy_point->number, GET_TEXT(S_All_KWH_used));
			Edip_Draw_Line(Line_1, Line_2);
			point1.y += 20;
			Line_1.y += 20;
			Line_2.y += 20;
			msg_nr++;
			break;
		default:
			break;
		}

		if(msg_nr >= 3){
			scroll_screen = true;
			break;
		}

	}

	/*while(1){
		Energy_point = Energy_point_loop(Energy_point);
		if(Energy_point == NULL){
			//if(!((scroll_screen == true)&&(msg_nr == 0))){
				break;
			//}
		}
		switch(Energy_point->data.State){
		case ST_END_RESERVATION:
			Edip_Draw_String(point1,"[%d] %s", Energy_point->number, GET_TEXT(S_Reservation_ended));
			Edip_Draw_Line(Line_1, Line_2);
			point1.y += 20;
			Line_1.y += 20;
			Line_2.y += 20;
			msg_nr++;
			break;
		default:
			break;
		}

		if(msg_nr >= 3){
			scroll_screen = true;
			break;
		}
	}*/

	if(msg_nr == 0){
		Edip_Draw_String(point1,GET_TEXT(S_No_notifications));
		Edip_Draw_Line(Line_1, Line_2);
	}


	Line_1.y = 75;
	Line_2.y = 110;
	Edip_Draw_Fill_Rectangle(Line_1, Line_2);

	Edip_Change_Font_Setting(&StdFont,MODE,M_INV_REP);
	Edip_Set_Font(&StdFont);
	point1.y = 80;
	Edip_Draw_String(point1,GET_TEXT(S_Enter_energy_point_number));
	SCR_idle_input();
	Edip_Change_Font_Setting(&StdFont,MODE,M_SET);
	Edip_Change_Font_Setting(&StdFont,ALIGN,A_LEFT);
	Edip_Set_Font(&StdFont);
	point1.y = 113;
	point1.x = 0;
	Edip_Draw_String(point1,"[<] %s", GET_LANG_TEXT(S_Language,EN));
	Edip_Change_Font_Setting(&StdFont,ALIGN,A_RIGHT);
	Edip_Set_Font(&StdFont);
	point1.x = 240;
	Edip_Draw_String(point1,"%s [V]", GET_TEXT(S_Accept));
	Edip_Change_Font_Setting(&StdFont,ALIGN,A_CENTER);
	Edip_Set_Font(&StdFont);
	return;
}

void SCR_idle_input(void){
	Point_t point = {.x=120,.y=95};
	Please_wait_screen(false);
	Edip_Change_Font_Setting(&StdFont,MODE,M_INV_REP);
	Edip_Change_Font_Setting(&StdFont,ALIGN,A_CENTER);
	Edip_Set_Font(&StdFont);
	if(g_Current_energy_point == 0){
		Edip_Draw_String(point,"   %s:@_@   ",GET_TEXT(S_Energy_point));
	}else{
		Edip_Draw_String(point,"   %s:%d%s   ",GET_TEXT(S_Energy_point),g_Current_energy_point,(g_Current_energy_point < 100)?"@ @":"");
	}
	Edip_Change_Font_Setting(&StdFont,MODE,M_SET);
	Edip_Set_Font(&StdFont);
	return;
}
