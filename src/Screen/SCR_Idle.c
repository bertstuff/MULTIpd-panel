#include <Driver/include/lpc17xx_gpio.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/Process/process.h>
#include <Core/Timer/etimer.h>
#include <Core/Clock/Time_functions.h>
#include <Processes/dev_reset.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Global.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "Power.h"
#include <Core/Util/array.h>


#define CON1 (1<<2)
#define CON2 (1<<3)
#define CON3 (1<<4)
#define CON4 (1<<5)
#define CON5 (1<<6)
#define CON6 (1<<7)
#define CON7 (1<<8)
#define CON8 (1<<9)

#define CON_MASK (CON1 | CON2 | CON3 | CON4 | CON5 | CON6 | CON7 | CON8)

/*prototype*/
PROCESS(SCR_idle_screen, "idle screen");

void check_manual_input(void);

struct timer Reservation_tm;
struct etimer Startup_tm;

//screen process
PROCESS_THREAD(SCR_idle_screen, ev, data)
{
	static SCP_pack_t * SCP_Packet;
	static struct etimer poll_tm;
	static uint8_t i,j;
	uint16_t Devices_list[5];
	char varname_buffer[10];

	char ini_key_buffer[20];


	if(etimer_expired(&poll_tm)){
		etimer_set(&poll_tm, CLOCK_SECOND * 5);
	}

	check_manual_input();

	PROCESS_BEGIN();

	timer_set(&Reservation_tm,CLOCK_SECOND);
	for( i = 0; i<8; i++){
		sprintf(ini_key_buffer, "Energy_point%d_nr", i + 1);
		g_Mdata[i].Number = ini_getl("Energy_point",ini_key_buffer,i + 1,"Energy point number of output:",inifile_machine);

		if(ini_key_buffer == 0){
			continue;
		}

		sprintf(ini_key_buffer, "Manual_active%d", i + 1);
		switch(ini_getl("Energy_point",ini_key_buffer,0,"Manual control active:",inifile_machine)){
		default:
		case 0:
			g_Mdata[i].State = ST_NO_RESERVATION;
			break;
		case 1:
			power_enable(g_Mdata[i].Number);
			g_Mdata[i].State = ST_MANUAL;
			break;
		case 2:
			power_disable(g_Mdata[i].Number);
			g_Mdata[i].State = ST_MANUAL;
			break;
		}
	}
	while(ev != PROCESS_EVENT_EXIT){
		Save_reset_poll();
		if(ev == PROCESS_EVENT_TIMER){
			for( i = 0; i<8; i++){
				if(g_Mdata[i].State == ST_MANUAL){
					continue;
				}

				//GET RESERVATION NR
				if(((g_Mdata[i].State == ST_END_RESERVATION) && (g_Mdata[i].Resvnr != 0))||
				  (g_Mdata[i].refresh_reservation == true)||
				  (timer_expired(&Reservation_tm))){
					g_Mdata[i].refresh_reservation = false;
					printf("MULTIpd: get reservation output %d\r\n",i+1);
					PROCESS_PT_SCP_MSG_RESERVERINGMACHINE(&SCP_Packet, Server_prefix_nr + g_Mdata[i].Number, 0);
					if(SCP_Packet->Data.Message_type == msg_ReserveringMachine){
						g_Mdata[i].Resvnr = SCP_msg_ReserveringMachine__ResvNr(SCP_Packet);
						g_Mdata[i].refresh_data = true;
						printf("MULTIpd: new reservation %d output %d\r\n",g_Mdata[i].Resvnr,i+1);
					}else if(SCP_Packet->Data.Message_type == msg_Onbekend){
						if(g_Mdata[i].Resvnr != 0){
							if(CurTime_in_periode(g_Mdata[i].Start_time, g_Mdata[i].End_time, false) == 0){ //check old reservation
								printf("MULTIpd: Reservation removed\r\n");
								g_Mdata[i].State = ST_NO_RESERVATION;
								for(j = SCP_Devices_of_types(Devices_list, 5, 1, MULTIPD_PANEL); j > 0 ; j--){
									BUFFER_SCP_MSG_VAR_INT_SEND(SCP_varname_array(varname_buffer,"State",g_Mdata[i].Number), g_Mdata[i].State, Devices_list[j-1]);
								}
							}
						}

						g_Mdata[i].Resvnr = 0;
						power_disable(g_Mdata[i].Number);
						g_Mdata[i].State = ST_NO_RESERVATION;
						printf("MULTIpd: no reservation\r\n");
					}else{
						continue;
					}
				}

				if(g_Mdata[i].refresh_data == true){
					//VERKRIJG RESERVERING TIJD
					printf("MULTIpd: Get reservation time output\r\n",i+1);
					PROCESS_PT_SCP_MSG_GEEFRESERVERING(&SCP_Packet, g_Empty_UID, g_Mdata[i].Resvnr, 0);
					if(SCP_Packet->Data.Message_type == msg_Reservering){
						g_Mdata[i].Start_time = SCP_msg_Reservering__StartTime(SCP_Packet);
						g_Mdata[i].End_time = SCP_msg_Reservering__EndTime(SCP_Packet);
						printf ( "MULTIpd: reservation output%d form %s",i+1,asctime (LocalTime(&g_Mdata[i].Start_time)));
						printf ( " to %s\r\n",asctime (LocalTime(&g_Mdata[i].End_time)) );
					}else if(SCP_Packet->Data.Message_type == msg_Onbekend){
						g_Mdata[i].Resvnr = 0;
						power_disable(g_Mdata[i].Number);
						g_Mdata[i].State = ST_NO_RESERVATION;

					}else{
						continue;
					}

					//VERKRIJG RESERVERING INFORMATIE
					printf("MULTIpd: Get reservation info output %d\r\n",i+1);
					PROCESS_PT_SCP_MSG_RESERVERINGGETINFO(&SCP_Packet, g_Mdata[i].Resvnr, "MaxCurrent,MaxWattHour,CurrentWattHour",0);
					if(SCP_Packet->Data.Message_type == msg_ReserveringSetInfo){
						char **arr;
						if(explode(&arr, SCP_msg_ReserveringSetInfo__var(SCP_Packet), '=') == 4){
							printf("MULTIpd: Reservation info output %d\r\n",i+1);
							g_Mdata[i].Current_max = atoi(arr[1]);
							g_Mdata[i].Watt_H_max = atoi(arr[2]);
							g_Mdata[i].Watt_H = atoi(arr[3]);
						}
						free(arr);
					}else{
						continue;
					}

					g_Mdata[i].refresh_data = false;
				}

				//is there a reservation
				if(g_Mdata[i].Resvnr != 0){

					if(g_Mdata[i].State != ST_MANUAL){
						//CHECK CURRENT
						if(g_Mdata[i].Current_max != 0){
							if((g_Mdata[i].Current_RMS/1000) > (g_Mdata[i].Current_max)){
								if(g_Mdata[i].State == ST_ON){
									power_disable(g_Mdata[i].Number);
									g_Mdata[i].State = ST_OVERCURRENT;
									printf("MULTIpd: Over current output %d\r\n",i+1);
									for(j = SCP_Devices_of_types(Devices_list, 5, 1, MULTIPD_PANEL); j > 0 ; j--){
										BUFFER_SCP_MSG_VAR_INT_SEND(SCP_varname_array(varname_buffer,"State",g_Mdata[i].Number), g_Mdata[i].State, Devices_list[j-1]);
									}
								}
							}
						}

						//CHECK POWER
						if(g_Mdata[i].Watt_H_max != 0){
							if((g_Mdata[i].Watt_H) > (g_Mdata[i].Watt_H_max)){
								if(g_Mdata[i].State == ST_ON){
									power_disable(g_Mdata[i].Number);
									g_Mdata[i].State = ST_NO_KWH_LEFT;
									printf("MULTIpd: No kw left output %d\r\n",i+1);
									for(j = SCP_Devices_of_types(Devices_list, 5, 1, MULTIPD_PANEL); j > 0 ; j--){
										BUFFER_SCP_MSG_VAR_INT_SEND(SCP_varname_array(varname_buffer,"State",g_Mdata[i].Number), g_Mdata[i].State, Devices_list[j-1]);
									}
								}
							}
						}
					}

					//CHECK TIME
					double  Reservation_in_periode = CurTime_in_periode(g_Mdata[i].Start_time, g_Mdata[i].End_time, false);

					if(g_Mdata[i].State == ST_MANUAL){//Manual override
						Reservation_in_periode = 0;
					}

					if(Reservation_in_periode == 0){ //In period
						if((g_Mdata[i].State != ST_ON)&&
						   (g_Mdata[i].State != ST_OFF)&&
						   (g_Mdata[i].State != ST_OVERCURRENT)&&
						   (g_Mdata[i].State != ST_NO_KWH_LEFT))
						{
							printf("MULTIpd: Power enable output %d\r\n",i+1);
							power_enable(g_Mdata[i].Number);
							for(j = SCP_Devices_of_types(Devices_list, 5, 1, MULTIPD_PANEL); j > 0 ; j--){
								BUFFER_SCP_MSG_VAR_INT_SEND(SCP_varname_array(varname_buffer,"State",g_Mdata[i].Number), g_Mdata[i].State, Devices_list[j-1]);
							}
						}
					}else if(Reservation_in_periode > 0){ //reservation expired
						g_Mdata[i].Current_max = 0;
						g_Mdata[i].Watt_H_max = 0;
						power_disable(g_Mdata[i].Number);
						g_Mdata[i].State = ST_END_RESERVATION;
						printf("MULTIpd: End of reservation output %d\r\n",i+1);
						for(j = SCP_Devices_of_types(Devices_list, 5, 1, MULTIPD_PANEL); j > 0 ; j--){
							BUFFER_SCP_MSG_VAR_INT_SEND(SCP_varname_array(varname_buffer,"State",g_Mdata[i].Number), g_Mdata[i].State, Devices_list[j-1]);
						}
					}else{
						g_Mdata[i].State = ST_WAIT_RESERVATION;
						if(Relay_state_known(g_Mdata[i].Number) == false){
							power_disable(g_Mdata[i].Number);
						}
					}
				}else{
					if(g_Mdata[i].State != ST_NO_RESERVATION){
						g_Mdata[i].State = ST_NO_RESERVATION;
					}
				}
			}

			if(timer_expired(&Reservation_tm)){
				timer_set(&Reservation_tm,CLOCK_MIN * 10);
			}
		}
		PROCESS_WAIT_EVENT();
	}
	PROCESS_END();
}

void check_manual_input(void){
	uint8_t i;
	static uint32_t old_input_port;
	uint32_t input_port = 0;
	const uint32_t pin[8] = {CON8,CON7,CON6,CON5,CON4,CON3,CON2,CON1};

	input_port = GPIO_ReadValue(2) & (CON_MASK);
	if(old_input_port != input_port){
		for(i=0; i<8; i++){
			if(g_Mdata[i].Number != 0){
				if(!(input_port & pin[i])){
					if((g_Mdata[i].State != ST_MANUAL)&&(g_Mdata[i].State != ST_ON)){
						power_enable(g_Mdata[i].Number);
						g_Mdata[i].refresh_data = false;
						g_Mdata[i].State = ST_MANUAL;
					}
				}else if(g_Mdata[i].State == ST_MANUAL){
					power_disable(g_Mdata[i].Number);
					g_Mdata[i].refresh_data = true;
					g_Mdata[i].State = ST_NO_RESERVATION;
				}
			}
		}
		old_input_port = input_port;
	}
}
