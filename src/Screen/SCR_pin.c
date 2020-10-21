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
#include <Core/Extensions/code/access_code.h>
#include "Energy_point.h"
#include "SCR_Overview.h"
#include "Power.h"

struct timer Reservation_tm;

uint8_t input_lenght = 0;
uint8_t input_pos = 0;

/*prototype*/
PROCESS(SCR_Pin_screen, "Pin screen");
void SCR_input(void);
void SCR_pin(void);

//screen process
PROCESS_THREAD(SCR_Pin_screen, ev, data)
{

	char name_buffer[20];
	static SCP_pack_t * SCP_packet;
	static Energy_point_data_t * measure_data;

	PROCESS_BEGIN();
	input_pos = 0;
	memset(g_Cur_UID.UID,0xC,7);
	input_lenght = ini_getl("General","Code_lenght",5,"Lenght of access code",inifile_machine);
	SCR_pin();
	SET_SCREEN_TIMEOUT(30);
	while(ev != PROCESS_EVENT_EXIT){
		if(PROCESS_EVENT_SCREEN_TIMEOUT){
			SCR_load(&SCR_idle_screen);
		}

		if(ev == event_Button3x4_pressed){
			SET_SCREEN_TIMEOUT(30);
			uint8_t key = keypad_button(ev,data);
			if(key == C_RETURN){
				memset(g_Cur_UID.UID,0xC,7);
				input_pos = 0;
				SCR_load(&SCR_Overview_screen);
				PROCESS_EXIT();
			}else if(key == C_OK){


			}else if(key != C_IDLE){
				if(input_pos < 7){
					g_Cur_UID.type = PIN_UID;
					g_Cur_UID.UID[input_pos] = keypad_button(ev,data);
					input_pos++;
					SCR_pin();
					if(input_pos == input_lenght){
						if(Access_Code_present_on_sd(g_Cur_UID.UID,input_lenght) == true){
							memset(g_Cur_UID.UID,0xC,7);
							input_pos = 0;
							PROCESS_PT_SCP_MSG_VAR_INT_SEND(&SCP_packet, SCP_varname_array(name_buffer,"State",g_Current_energy_point), 1, Energy_point_device(g_Current_energy_point));
							if(SCP_packet->Data.Message_type == msg_Ok){
								measure_data = Energy_point_data(g_Current_energy_point);
								measure_data->State = 1;
							}
							SCR_load(&SCR_Overview_screen);
							PROCESS_EXIT();
						}
						Please_wait_screen(true);
						PROCESS_PT_SCP_MSG_PRESENT(&SCP_packet,g_Cur_UID,VAL_MACHINE_NR,Server_prefix_nr + g_Current_energy_point);
						Please_wait_screen(false);
						input_pos = 0;
						if(SCP_packet->Data.Message_type == msg_Akkoord){
							int i;
							memset(g_Cur_UID.UID,0xC,7);
							input_pos = 0;
							Power_point_data_t * Power_data = Get_Energy_point_data(Server_prefix_nr + g_Current_energy_point);
							timer_set(&Reservation_tm,CLOCK_SECOND);
							for(i = 0;i<8;i++){
								g_Mdata[i].refresh_reservation = true;
							}
							power_enable(Power_data->Number);
							measure_data = Energy_point_data(g_Current_energy_point);
							measure_data->State = 1;
							SCR_load(&SCR_Overview_screen);
							PROCESS_EXIT();
						}else if(SCP_packet->Data.Message_type == msg_NietAkkoord){
							PROCESS_PT_MESSAGE_SCREEN(GET_TEXT(S_Code_not_valid));
						}else{
							PROCESS_PT_MESSAGE_SCREEN(GET_TEXT(S_Server_error));
						}
						memset(g_Cur_UID.UID,0xC,7);
						SCR_pin();
					}else{
						SCR_input();
					}

				}
			}
		}
		PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}

void SCR_pin(void){
	Point_t pos = {.x = 120,.y = 0};

	Edip_Clear_Display();
	Edip_Set_Font(&H1Font);
	Edip_Draw_String(pos,"-%s-",GET_TEXT(S_Enter_code));
	Edip_Set_Font(&StdFont);

	SCR_input();

}


void SCR_input(void){
	Point_t point = {.x = 120,.y = 55};

	uint8_t i;
	char input_visual[9];
	memset(input_visual,' ',7);
	memset(input_visual,'_',input_lenght);

	point.y = 55;
	for(i=0;i<7;i++){
		if(i < input_pos){
			input_visual[i] = '*';
		}
	}
	input_visual[7] = 0;
	Edip_Draw_String(point,"%s",input_visual);

}
