#include "SCR_Idle.h"
#include <Core/Extensions/ini/minIni.h>
#include <Core/Process/process.h>
#include <core/Timer/etimer.h>
#include <Core/Timer/timer_functions.h>
#include <Processes/dev_reset.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Device/I2C/BUTTONS/Buttons_3x4.h>
#include <Screen/SCR_Message.h>
#include <Global.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "Energy_point.h"
#include "SCR_pin.h"

/*prototype*/
PROCESS(SCR_Overview_screen, "Overview screen");

void SCR_Overview(bool update, Energy_point_data_t * data);

//screen process
PROCESS_THREAD(SCR_Overview_screen, ev, data)
{
	static struct etimer poll_tm;
	static struct etimer refresh_tm;
	SCP_pack_t * SCP_packet;
	static Energy_point_data_t * measure_data;
	char name_buffer[20];

	if(etimer_expired(&poll_tm)){
		etimer_set(&poll_tm,CLOCK_SECOND);
	}

	PROCESS_BEGIN();
	etimer_set(&poll_tm,CLOCK_SECOND);

	Please_wait_screen(true);
	PROCESS_PT_SCP_MSG_VAR_INT_REQUEST(&SCP_packet, SCP_varname_array(name_buffer,"State",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
	if(SCP_packet->Data.Message_type == msg_var_int_send){
		measure_data = Energy_point_data(g_Current_energy_point);
		measure_data->State = SCP_msg_var_int_send__value(SCP_packet);
	}else{
		SCR_load(&SCR_idle_screen);
		PROCESS_EXIT();
	}
	PROCESS_PT_SCP_MSG_VAR_INT_REQUEST(&SCP_packet, SCP_varname_array(name_buffer,"MaxCurrent",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
	if(SCP_packet->Data.Message_type == msg_var_int_send){
		measure_data = Energy_point_data(g_Current_energy_point);
		measure_data->MaxAmpere = SCP_msg_var_int_send__value(SCP_packet);
	}else{
		SCR_load(&SCR_idle_screen);
		PROCESS_EXIT();
	}
	PROCESS_PT_SCP_MSG_VAR_INT_REQUEST(&SCP_packet, SCP_varname_array(name_buffer,"Current",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
	if(SCP_packet->Data.Message_type == msg_var_int_send){
		measure_data = Energy_point_data(g_Current_energy_point);
		measure_data->Ampere = SCP_msg_var_int_send__value(SCP_packet);
	}else{
		SCR_load(&SCR_idle_screen);
		PROCESS_EXIT();
	}
	PROCESS_PT_SCP_MSG_VAR_INT_REQUEST(&SCP_packet, SCP_varname_array(name_buffer,"Watt",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
	if(SCP_packet->Data.Message_type == msg_var_int_send){
		measure_data = Energy_point_data(g_Current_energy_point);
		measure_data->Power = SCP_msg_var_int_send__value(SCP_packet);
	}else{
		SCR_load(&SCR_idle_screen);
		PROCESS_EXIT();
	}

	PROCESS_PT_SCP_MSG_VAR_INT_REQUEST(&SCP_packet, SCP_varname_array(name_buffer,"MaxWattHour",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
	if(SCP_packet->Data.Message_type == msg_var_int_send){
		measure_data = Energy_point_data(g_Current_energy_point);
		measure_data->MaxWH = SCP_msg_var_int_send__value(SCP_packet);
	}else{
		SCR_load(&SCR_idle_screen);
		PROCESS_EXIT();
	}

	PROCESS_PT_SCP_MSG_VAR_INT_REQUEST(&SCP_packet, SCP_varname_array(name_buffer,"CurrentWattHour",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
	if(SCP_packet->Data.Message_type == msg_var_int_send){
		measure_data = Energy_point_data(g_Current_energy_point);
		measure_data->WH = SCP_msg_var_int_send__value(SCP_packet);
	}else{
		SCR_load(&SCR_idle_screen);
		PROCESS_EXIT();
	}

	Please_wait_screen(false);
	SCR_Overview(true,measure_data);
	SET_SCREEN_TIMEOUT(30);
	etimer_set(&refresh_tm,CLOCK_SECOND*5);
	while(ev != PROCESS_EVENT_EXIT){
		//keypad

		if(etimer_expired(&refresh_tm)){
			etimer_set(&refresh_tm,CLOCK_SECOND*5);
			BUFFER_SCP_MSG_VAR_INT_REQUEST(SCP_varname_array(name_buffer,"State",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
			BUFFER_SCP_MSG_VAR_INT_REQUEST(SCP_varname_array(name_buffer,"MaxCurrent",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
			BUFFER_SCP_MSG_VAR_INT_REQUEST(SCP_varname_array(name_buffer,"Current",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
			BUFFER_SCP_MSG_VAR_INT_REQUEST(SCP_varname_array(name_buffer,"Watt",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
			BUFFER_SCP_MSG_VAR_INT_REQUEST(SCP_varname_array(name_buffer,"MaxWattHour",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
			BUFFER_SCP_MSG_VAR_INT_REQUEST(SCP_varname_array(name_buffer,"CurrentWattHour",g_Current_energy_point), Energy_point_device(g_Current_energy_point));
			measure_data = Energy_point_data(g_Current_energy_point);
			SCR_Overview(false,measure_data);
		}

		if(ev == event_Button3x4_pressed){
			uint8_t key = keypad_button(ev, data);
			if(key == C_RETURN){ //Clear key
				SCR_load(&SCR_idle_screen);
				PROCESS_EXIT();
			}
			if(key == C_OK){
				if((measure_data->State == ST_OFF)||(measure_data->State == ST_OVERCURRENT)){
					if(ini_getbool("Device","Pin_enable",false,"Ask for pin before activation",inifile_machine)){
						SCR_load(&SCR_Pin_screen);
						PROCESS_EXIT();
					}else{
						PROCESS_PT_SCP_MSG_VAR_INT_SEND(&SCP_packet, SCP_varname_array(name_buffer,"State",g_Current_energy_point), 1, Energy_point_device(g_Current_energy_point));
						if(SCP_packet->Data.Message_type == msg_Ok){
							measure_data->State = 1;
						}
					}
				}else if(measure_data->State == ST_NO_KWH_LEFT){
					PROCESS_PT_MESSAGE_SCREEN("%s\r\n%s",GET_TEXT(S_Enable_not_possible),GET_TEXT(S_All_KWH_used));
				}else if(measure_data->State == ST_NO_RESERVATION){
					//if(ini_getbool("Device","Pin_enable",false,"Ask for pin before activation",inifile_machine)){
						SCR_load(&SCR_Pin_screen);
						PROCESS_EXIT();
					//}else{
						//PROCESS_PT_MESSAGE_SCREEN("%s\r\n%s",GET_TEXT(S_Enable_not_possible),GET_TEXT(S_No_reservation));
					//}
				}else if(measure_data->State == ST_WAIT_RESERVATION){
					PROCESS_PT_MESSAGE_SCREEN("%s\r\n%s",GET_TEXT(S_Enable_not_possible),GET_TEXT(S_Wait_start_reservation));
				}
				SCR_Overview(true,measure_data);
			}
		}

		if(PROCESS_EVENT_SCREEN_TIMEOUT){
			SCR_load(&SCR_idle_screen);
		}
		PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}

void SCR_Overview(bool update, Energy_point_data_t * data){
	Point_t point1 = {.x=120,.y=1};
	Point_t Line_1 = {.x=0,.y=17};
	Point_t Line_2 = {.x=240,.y=17};
	double div;

	if(update == true){
		Edip_Clear_Display();
	}
	Edip_Change_Font_Setting(&StdFont,ALIGN,A_CENTER);
	Edip_Change_Font_Setting(&StdFont,MODE,M_REPLACE);
	Edip_Set_Font(&StdFont);
	switch(data->State){
	case ST_OFF:
	case ST_ON:
		//Edip_Draw_String(point1,GET_TEXT(S_No_notifications));
		Edip_Draw_String(point1,"%s: %d",GET_TEXT(S_Energy_point),g_Current_energy_point);
		break;
	case ST_OVERCURRENT:
		Edip_Draw_String(point1,GET_TEXT(S_Overload));
		break;
	case ST_NO_RESERVATION:
	case ST_END_RESERVATION:
		Edip_Draw_String(point1,GET_TEXT(S_No_reservation));
		break;
	case ST_WAIT_RESERVATION:
		Edip_Draw_String(point1,GET_TEXT(S_Wait_start_reservation));
		break;
	case ST_NO_KWH_LEFT:
		Edip_Draw_String(point1,GET_TEXT(S_All_KWH_used));
		break;
	}
	Edip_Draw_Line(Line_1, Line_2);

	Edip_Change_Font_Setting(&StdFont,ALIGN,A_LEFT);
	Edip_Set_Font(&StdFont);
	/*point1.x = 20;
	point1.y = 30;
	Edip_Draw_String(point1,"%s: ",GET_TEXT(S_Energy_point));
	point1.x = 140;
	Edip_Draw_String(point1,"%d  ",g_Current_energy_point);
	*/
	point1.x = 20;
	point1.y = 19;
	switch(data->State){
	case ST_OFF:
	case ST_OVERCURRENT:
	case ST_NO_KWH_LEFT:
	case ST_NO_RESERVATION:
	case ST_WAIT_RESERVATION:
		Edip_Draw_String(point1,"%s: ",GET_TEXT(S_State));
		point1.x = 140;
		Edip_Draw_String(point1,"%s   ",GET_TEXT(S_Disabled));
		point1.x = 20;
		break;
	case ST_ON:
		Edip_Draw_String(point1,"%s: ",GET_TEXT(S_State));
		point1.x = 140;
		Edip_Draw_String(point1,"%s   ",GET_TEXT(S_Enabled));
		point1.x = 20;
		break;
	default:
		break;
	}
	if(data->MaxAmpere != 0){
		point1.y += 15;
		Edip_Draw_String(point1,"%s: ",GET_TEXT(S_Max_current));
		point1.x = 140;
		Edip_Draw_String(point1,"%d A   ",data->MaxAmpere);
	}
	point1.x = 20;
	point1.y += 15;
	if(data->Ampere < 1000){
		Edip_Draw_String(point1,"%s: ",GET_TEXT(S_Current));
		point1.x = 140;
		Edip_Draw_String(point1,"... A   ");
		point1.x = 20;
		point1.y += 15;
		Edip_Draw_String(point1,"%s: ",GET_TEXT(S_Power));
		point1.x = 140;
		Edip_Draw_String(point1,"... W   ");
		point1.x = 20;
	}else{
		div = data->Ampere;
		div	/= 1000;
		Edip_Draw_String(point1,"%s: ",GET_TEXT(S_Current));
		point1.x = 140;
		Edip_Draw_String(point1,"%2.1f A   ",div);
		point1.x = 20;
		point1.y += 15;
		Edip_Draw_String(point1,"%s: ",GET_TEXT(S_Power));
		point1.x = 140;
		Edip_Draw_String(point1,"%d W   ",data->Power);
		point1.x = 20;
	}

	if(data->MaxWH != 0){
		point1.x = 20;
		point1.y += 15;
		Edip_Draw_String(point1,"%s: ",GET_TEXT(S_Max_KWH));
		point1.x = 140;
		Edip_Draw_String(point1,"%d KWh   ",(data->MaxWH)/1000);
	}

	point1.x = 20;
	point1.y += 15;
	div = data->WH;
	div	/= 1000;
	Edip_Draw_String(point1,"%s: ",GET_TEXT(S_current_KWH));
	point1.x = 140;
	Edip_Draw_String(point1,"%2.1f KWh   ",div);

	Line_1.y=110;
	Line_2.y=110;
	Edip_Draw_Line( Line_1, Line_2);

	point1.y = 113;
	point1.x = 0;
	Edip_Draw_String(point1,"[<] %s", GET_TEXT(S_Back));
	if(data->State != 1){
		point1.x = 240;
		Edip_Change_Font_Setting(&StdFont,ALIGN,A_RIGHT);
		Edip_Set_Font(&StdFont);
		Edip_Draw_String(point1,"%s [V]", GET_TEXT(S_Enable));
	}
	Edip_Change_Font_Setting(&StdFont,ALIGN,A_CENTER);
	Edip_Set_Font(&StdFont);
	return;
}
