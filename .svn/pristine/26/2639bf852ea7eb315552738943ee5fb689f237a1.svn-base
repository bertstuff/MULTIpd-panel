#include "SCR_Idle.h"
#include <Core/Clock/clock.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/protocol/Net/SCP/tRFIDmsg.h>
#include <Core/Process/process.h>
#include <Core/Thread/pt.h>
#include <Core/Timer/etimer.h>
#include <Core/Timer/timer_functions.h>
#include <Core/Type/type.h>
#include <Processes/dev_reset.h>
#include <Device/I2C/BUTTONS/Buttons_3x4.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Display.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Draw.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Text.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Types.h>
#include <Device/I2C/RFID/RFID.h>
#include <Device/IO/BUTTONS/EBM-Basismodule-buttons.h>
#include <Device/UART/XBA/XBA.h>
#include <Global.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <Screen/SCR_Text.h>
#include <Screen/SCR_Init.h>
#include <Screen/SCR_Message.h>
#include <Screen/SCR_Idle.h>
#include <Screen/SCR_Text.h>

/*prototype*/
PROCESS(SCR_language_screen, "language screen");
void SCR_language(void);

//screen process
PROCESS_THREAD(SCR_language_screen, ev, data)
{

	PROCESS_BEGIN();
	SCR_language();
	Button3x4_activate(true);
	EBM_Button_led(EBM_BUTTON1,true);
	SET_SCREEN_TIMEOUT(15);
	while(ev != PROCESS_EVENT_EXIT){

		//keypad
		if(ev == event_Button3x4_pressed){
			uint8_t key = *((uint8_t *)data);
			uint8_t number = 1;
			if(lanuage_exsist(g_screen_type, NL)){
				if(number == key){
					g_Language = NL;
					SCR_load(&SCR_idle_screen);
				}
				number++;
			}

			if(lanuage_exsist(g_screen_type, EN)){
				if(number == key){
					g_Language = EN;
					SCR_load(&SCR_idle_screen);
				}
				number++;
			}

			if(lanuage_exsist(g_screen_type, DE)){
				if(number == key){
					g_Language = EN;
					SCR_load(&SCR_idle_screen);
				}
				number++;
			}

			if(lanuage_exsist(g_screen_type, FR)){
				if(number == key){
					g_Language = FR;
					SCR_load(&SCR_idle_screen);
				}
				number++;
			}
		}

		if(PROCESS_EVENT_SCREEN_TIMEOUT){
			SCR_load(&SCR_idle_screen);
		}
		PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}

void SCR_language(void){
	uint8_t number = 1;
	Point_t point = {.x=120,.y=30};
	Point_t Line_1 = {.x=0,.y=0};
	Point_t Line_2 = {.x=240,.y=20};

	Edip_Clear_Display();


	Edip_Draw_Fill_Rectangle(Line_1, Line_2);

	Edip_Change_Font_Setting(&StdFont,MODE,M_INVERSE);
	Edip_Set_Font(&StdFont);
	point.x = 120;
	point.y = 5;
	Edip_Draw_String(point,GET_TEXT(S_Choose_language));
	Edip_Change_Font_Setting(&StdFont,MODE,M_SET);
	Edip_Set_Font(&StdFont);

	if(lanuage_exsist(g_screen_type, NL)){
		point.x = 10;
		point.y += 25;
		Edip_Draw_String(point,"%d:",number++);
		point.x = 120;
		Edip_Draw_String(point,GET_LANG_TEXT(S_Dutch,NL));

		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
	}

	if(lanuage_exsist(g_screen_type, EN)){
		point.x = 10;
		point.y += 25;
		Edip_Draw_String(point,"%d:",number++);
		point.x = 120;
		Edip_Draw_String(point,GET_LANG_TEXT(S_English,EN));
		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
	}

	if(lanuage_exsist(g_screen_type, DE)){
		point.x = 10;
		point.y += 25;
		Edip_Draw_String(point,"%d:",number++);
		point.x = 120;
		Edip_Draw_String(point,GET_LANG_TEXT(S_German,DE));
		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
	}

	if(lanuage_exsist(g_screen_type, FR)){
		point.x = 10;
		point.y += 25;
		Edip_Draw_String(point,"%d:",number++);
		point.x = 120;
		Edip_Draw_String(point,GET_LANG_TEXT(S_French,FR));
		Line_1.y = point.y + 20;
		Line_2.y = point.y + 20;
		Edip_Draw_Line(Line_1, Line_2);
	}
	return;
}
