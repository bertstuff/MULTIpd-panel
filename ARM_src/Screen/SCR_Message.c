#include <Board/Def_config.h>
#if(BOARD_SCREEN == 1)
#include "SCR_Message.h"
#include "SCR_Init.h"
#include "Screen/SCR_Text.h"
#include <Global.h>
#include <Core/Timer/etimer.h>
#include <Core/Thread/pt.h>
#include <Device/I2C/Edip_240B/EDIP_240B.h>

/*prototype*/
void SCR_Message(char * message);
void Please_wait_screen(bool state);

/*global variables*/
static struct etimer exit_tm;
struct pt message_pt;
static struct etimer screen_tm;

PT_THREAD(SCR_Message_screen(char * format,...))
{
	char message[150];
	uint32_t bytes;
	PT_BEGIN(&message_pt);
	SCR_set_busy(true);
	Please_wait_screen(false);
	printf("SCREEN: Message\r\n");
	va_list args;
	va_start (args, format);
	bytes = vsnprintf (message, 150, format, args);
	va_end (args);
	bytes = (bytes / 20); //a human reads 240 words per minute. i took 5 char per word
	etimer_set(&exit_tm,CLOCK_SECOND * MAX(4,bytes));
	SCR_Message(message);
	PT_WAIT_UNTIL(&message_pt, etimer_expired(&exit_tm));
	SCR_set_busy(false);
	PT_END(&message_pt);
}


void SCR_Message(char * message){
	Point_t point = {.x=120,.y=20};

	switch(g_screen_type){
	case SCR_EDIP:
		point.x = 120;
		point.y = 20;
		Please_wait_screen(false);
		Edip_Clear_Display();
		Edip_Set_Font(&StdFont);
		Edip_Draw_String(point,message);
		break;
	case SCR_LCD_2X16:
		point.x = 0;
		point.y = 0;
		LCD_2x16_Clear_Display();
		LCD_2x16_Draw_String(point,message);
		break;
	case SCR_PC:
		printf("SCR_PC:[msg] %s\r\n",message);
		break;
	}
	return;
}

////////////////wait screen////////////////////////
PROCESS(SCR_Wait_screen, "Wait screen");

void SCR_bussy_bar(void){
	static uint8_t bussy_counter;
	Point_t point;

	if(g_screen_type == SCR_EDIP){
		point.x = 120;
		point.y = 60;
		Edip_Set_Font(&H1Font);
		switch(bussy_counter){
		case 12: bussy_counter = 0;
		case 0: Edip_Draw_String(point,"....   ");
		break;
		case 11:
		case 1: Edip_Draw_String(point,"... .  ");
		break;
		case 10:
		case 2: Edip_Draw_String(point,".. . . ");
		break;
		case 9:
		case 3: Edip_Draw_String(point,". . . .");
		break;
		case 8:
		case 4: Edip_Draw_String(point," . . ..");
		break;
		case 7:
		case 5: Edip_Draw_String(point,"  . ...");
		break;
		case 6: Edip_Draw_String(point,"   ....");
		break;
		}
	}

	if(g_screen_type == SCR_LCD_2X16){
		point.x = 4;
		point.y = 1;
		switch(bussy_counter){
		case 12: bussy_counter = 0;
		case 0: LCD_2x16_Draw_String(point,"....   ");
		break;
		case 11:
		case 1: LCD_2x16_Draw_String(point,"... .  ");
		break;
		case 10:
		case 2: LCD_2x16_Draw_String(point,".. . . ");
		break;
		case 9:
		case 3: LCD_2x16_Draw_String(point,". . . .");
		break;
		case 8:
		case 4: LCD_2x16_Draw_String(point," . . ..");
		break;
		case 7:
		case 5: LCD_2x16_Draw_String(point,"  . ...");
		break;
		case 6: LCD_2x16_Draw_String(point,"   ....");
		break;
		}
	}

	bussy_counter++;
	etimer_set(&screen_tm, CLOCK_SECOND / 4);
}

void SCR_please_wait(void){
	Point_t point = {.x=120,.y=30};

	if(g_screen_type == SCR_EDIP){
		point.x = 120;
		point.y = 30;
		Edip_Clear_Display();
		Edip_Set_Font(&H1Font);
		Edip_Draw_String(point,GET_TEXT(S_Please_wait));
	}

	if(g_screen_type == SCR_LCD_2X16){
		point.x = 0;
		point.y = 0;
		LCD_2x16_Clear_Display();
		point.x = (16-strlen(GET_TEXT(S_Please_wait)))/2;
		LCD_2x16_Draw_String(point,GET_TEXT(S_Please_wait));
	}

	if(g_screen_type == SCR_PC){
		printf("SCR_PC:[msg] please wait");
	}

	SCR_bussy_bar();
	return;
}

PROCESS_THREAD(SCR_Wait_screen, ev, data)
{
	PROCESS_BEGIN();
	SCR_please_wait();
	while(ev != PROCESS_EVENT_EXIT){
		PROCESS_WAIT_EVENT();
		if(etimer_expired(&screen_tm)){
				SCR_bussy_bar();
		}
	}
	PROCESS_END();
}

void Please_wait_screen(bool state){
	if(state == true){
		process_start(&SCR_Wait_screen, NULL);
	}else{
		process_exit(&SCR_Wait_screen);
	}
}
#endif
