#include <stdlib.h>
#include <Board/Def_config.h>
#include <Core/Process/process.h>
#include <Core/Timer/etimer.h>
#include <Driver/include/lpc17xx_adc.h>
#include <Driver/include/lpc17xx_pinsel.h>
#include <core/Extensions/ini/minIni.h>
#include <Driver/include/lpc17xx_gpio.h>
#include <math.h>
#include <Core/Memory/memb.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include "Power.h"
#include "Global.h"

#define RELAY1_ON_PORT 		1
#define RELAY1_ON_PIN		(1<<21)
#define RELAY1_OFF_PORT 	1
#define RELAY1_OFF_PIN		(1<<20)
#define RELAY1_LED_PORT 	1
#define RELAY1_LED_PIN		(1<<26)

#define RELAY2_ON_PORT 		1
#define RELAY2_ON_PIN		(1<<19)
#define RELAY2_OFF_PORT 	1
#define RELAY2_OFF_PIN		(1<<18)
#define RELAY2_LED_PORT 	1
#define RELAY2_LED_PIN		(1<<25)

#define RELAY3_ON_PORT		0
#define RELAY3_ON_PIN		(1<<30)
#define RELAY3_OFF_PORT		0
#define RELAY3_OFF_PIN		(1<<29)
#define RELAY3_LED_PORT 	1
#define RELAY3_LED_PIN		(1<<24)

#define RELAY4_ON_PORT		3
#define RELAY4_ON_PIN		(1<<25)
#define RELAY4_OFF_PORT		3
#define RELAY4_OFF_PIN		(1<<26)
#define RELAY4_LED_PORT 	1
#define RELAY4_LED_PIN		(1<<27)

#define RELAY5_ON_PORT		4
#define RELAY5_ON_PIN		(1<<29)
#define RELAY5_OFF_PORT		4
#define RELAY5_OFF_PIN		(1<<28)
#define RELAY5_LED_PORT 	0
#define RELAY5_LED_PIN		(1<<11)

#define RELAY6_ON_PORT		0
#define RELAY6_ON_PIN		(1<<4)
#define RELAY6_OFF_PORT		0
#define RELAY6_OFF_PIN		(1<<5)
#define RELAY6_LED_PORT 	1
#define RELAY6_LED_PIN		(1<<28)

#define RELAY7_ON_PORT		0
#define RELAY7_ON_PIN		(1<<17)
#define RELAY7_OFF_PORT		0
#define RELAY7_OFF_PIN		(1<<18)
#define RELAY7_LED_PORT 	1
#define RELAY7_LED_PIN		(1<<29)

#define RELAY8_ON_PORT		2
#define RELAY8_ON_PIN		(1<<11)
#define RELAY8_OFF_PORT		2
#define RELAY8_OFF_PIN		(1<<12)
#define RELAY8_LED_PORT 	0
#define RELAY8_LED_PIN		(1<<10)


#define INPUT1_PORT		2
#define INPUT1_PIN		(1<<2)
#define INPUT2_PORT		2
#define INPUT2_PIN		(1<<3)
#define INPUT3_PORT		2
#define INPUT3_PIN		(1<<4)
#define INPUT4_PORT		2
#define INPUT4_PIN		(1<<5)
#define INPUT5_PORT		2
#define INPUT5_PIN		(1<<6)
#define INPUT6_PORT		2
#define INPUT6_PIN		(1<<7)
#define INPUT7_PORT		2
#define INPUT7_PIN		(1<<8)
#define INPUT8_PORT		2
#define INPUT8_PIN		(1<<9)


const uint8_t RELAY_ON_PORT[8] = {RELAY1_ON_PORT, RELAY2_ON_PORT, RELAY3_ON_PORT, RELAY4_ON_PORT, RELAY5_ON_PORT, RELAY6_ON_PORT, RELAY7_ON_PORT, RELAY8_ON_PORT};
const uint32_t RELAY_ON_PIN[8] = {RELAY1_ON_PIN, RELAY2_ON_PIN, RELAY3_ON_PIN, RELAY4_ON_PIN, RELAY5_ON_PIN, RELAY6_ON_PIN, RELAY7_ON_PIN, RELAY8_ON_PIN};
const uint8_t RELAY_OFF_PORT[8] = {RELAY1_OFF_PORT, RELAY2_OFF_PORT, RELAY3_OFF_PORT, RELAY4_OFF_PORT, RELAY5_OFF_PORT, RELAY6_OFF_PORT, RELAY7_OFF_PORT, RELAY8_OFF_PORT};
const uint32_t RELAY_OFF_PIN[8] = {RELAY1_OFF_PIN, RELAY2_OFF_PIN, RELAY3_OFF_PIN, RELAY4_OFF_PIN, RELAY5_OFF_PIN, RELAY6_OFF_PIN, RELAY7_OFF_PIN, RELAY8_OFF_PIN};
const uint8_t RELAY_LED_PORT[8] = {RELAY1_LED_PORT, RELAY2_LED_PORT, RELAY3_LED_PORT, RELAY4_LED_PORT, RELAY5_LED_PORT, RELAY6_LED_PORT, RELAY7_LED_PORT, RELAY8_LED_PORT};
const uint32_t RELAY_LED_PIN[8] = {RELAY1_LED_PIN, RELAY2_LED_PIN, RELAY3_LED_PIN, RELAY4_LED_PIN, RELAY5_LED_PIN, RELAY6_LED_PIN, RELAY7_LED_PIN, RELAY8_LED_PIN};
const uint8_t INPUT_PORT[8] = {INPUT1_PORT, INPUT2_PORT, INPUT3_PORT, INPUT4_PORT, INPUT5_PORT, INPUT6_PORT, INPUT7_PORT, INPUT8_PORT};
const uint32_t INPUT_PIN[8] = {INPUT1_PIN, INPUT2_PIN, INPUT3_PIN, INPUT4_PIN, INPUT5_PIN, INPUT6_PIN, INPUT7_PIN, INPUT8_PIN};

#define DIR_OUTPUT 1

#define CLOCK_HOUR (clock_time_t)360000

PROCESS(Power_process,"Power measurement");
PROCESS(relay_process,"Relay puls");
PROCESS(MULTIpdtest_process,"test process");

process_event_t event_samples_done;
process_event_t event_test_input;

struct ADC_sample_t{
	struct ADC_sample_t * next;
	uint16_t Current1;
	uint16_t Current2;
	uint16_t Current3;
	uint16_t Current4;
	uint16_t Voltage;
}ADC_sample_t;

struct ADC_sample_t * first_sample = NULL;
struct ADC_sample_t * last_sample = NULL;
Power_point_data_t g_Mdata[8] = {{.Watt_H = 0},{.Watt_H = 0},{.Watt_H = 0},{.Watt_H = 0}} ;
static struct etimer relay_puls_tm;
bool g_Relay_active = false;
static struct etimer led_blink_tm;
bool relay_state_known[8] = {0,0,0,0,0,0,0,0};

uint16_t sampels = 0;
uint16_t current_mV_A = 66;
uint16_t Diode_mv = 1600;
uint16_t trafo_mult = 22100;
uint16_t send_interval;

uint32_t g_Voltage_RMS = 230000;
int32_t g_Sample_Current[8] = {0,0,0,0,0,0,0,0};
int32_t g_Sample_Voltage = 0;
int32_t g_Sample_top_Voltage = 0;
//uint16_t g_Calibrate_counter = 0;
//uint32_t g_Calibrate_val = 0;
bool MULTIPD_test = false;

MEMB(sample_mem,struct ADC_sample_t, 80);

void ADC_IRQHandler(void);

void Relay_init(void){
	uint8_t i;

	for(i = 0; i < 8; i++){
		GPIO_SetDir(RELAY_ON_PORT[i], RELAY_ON_PIN[i], DIR_OUTPUT);
		GPIO_SetDir(RELAY_OFF_PORT[i], RELAY_OFF_PIN[i], DIR_OUTPUT);
		GPIO_SetDir(RELAY_LED_PORT[i], RELAY_LED_PIN[i], DIR_OUTPUT);
		GPIO_SetDir(INPUT_PORT[i], INPUT_PIN[i], DIR_INPUT);

	    GPIO_ClearValue(RELAY_ON_PORT[i],RELAY_ON_PIN[i]);
	    GPIO_ClearValue(RELAY_OFF_PORT[i],RELAY_OFF_PIN[i]);
	    GPIO_ClearValue(RELAY_LED_PORT[i],RELAY_LED_PIN[i]);
	}
}

void Relay_enable(uint8_t relay){
	GPIO_ClearValue(RELAY_OFF_PORT[relay-1], RELAY_OFF_PIN[relay-1]);
	GPIO_SetValue(RELAY_ON_PORT[relay-1], RELAY_ON_PIN[relay-1]);
	GPIO_SetValue(RELAY_LED_PORT[relay-1], RELAY_LED_PIN[relay-1]);
	g_Relay_active = true;
	relay_state_known[relay-1] = true;
	PROCESS_CONTEXT_BEGIN(&relay_process);
	etimer_set(&relay_puls_tm,CLOCK_SECOND);
	PROCESS_CONTEXT_END(&relay_process);
}

void Relay_disable(uint8_t relay){
	GPIO_ClearValue(RELAY_ON_PORT[relay-1],RELAY_ON_PIN[relay-1]);
	GPIO_SetValue(RELAY_OFF_PORT[relay-1],RELAY_OFF_PIN[relay-1]);
	GPIO_ClearValue(RELAY_LED_PORT[relay-1],RELAY_LED_PIN[relay-1]);
	g_Relay_active = true;
	relay_state_known[relay-1] = true;
	PROCESS_CONTEXT_BEGIN(&relay_process);
	etimer_set(&relay_puls_tm,CLOCK_SECOND);
	PROCESS_CONTEXT_END(&relay_process);
}
bool Get_Output_nr(uint16_t point_nr, uint8_t * number){
	uint8_t i;
	for(i = 0 ;i < 8 ;i++){
		if(g_Mdata[i].Number == point_nr){
			*number = i;
			return true;
		}
	}
	return false;
}

bool Relay_state_known(uint16_t point_nr){
	uint8_t i;
	if(point_nr == 0){
		return false;
	}
	for(i = 0 ;i < 8 ;i++){
		if(g_Mdata[i].Number == point_nr){
			return relay_state_known[i];
		}
	}
	return false;
}

void power_enable(uint16_t point_nr){
	uint8_t i;
	if(point_nr == 0){
		return;
	}
	for(i = 0 ;i < 8 ;i++){
		if(g_Mdata[i].Number == point_nr){
			BUFFER_SCP_MSG_MACHINE_STARTED(Server_prefix_nr+point_nr, 1);
			g_Mdata[i].State = ST_ON;
			g_Mdata[i].output_enabled = true;
			Relay_enable(i+1);
			printf("MULTIpd: Output %d enable\r\n",point_nr);
		}
	}
	return;
}



void power_disable(uint16_t point_nr){
	uint8_t i;
	if(point_nr == 0){
		return;
	}
	for(i = 0 ;i < 8 ;i++){
		if(g_Mdata[i].Number == point_nr){
			BUFFER_SCP_MSG_MACHINE_STOPED(Server_prefix_nr+point_nr);
			g_Mdata[i].State = ST_OFF;
			g_Mdata[i].output_enabled = false;
			Relay_disable(i+1);
			printf("MULTIpd: Output %d disable\r\n",point_nr);
		}
	}
	return;
}

void Power_init(void){
	PINSEL_CFG_Type PinCfg;
	Relay_init();
	memb_init(&sample_mem);
	event_samples_done = process_alloc_event(); //Claim a event number
	event_test_input = process_alloc_event();
	process_start(&Power_process,NULL);
	process_start(&relay_process,NULL);

	PinCfg.Pinmode = PINSEL_PINMODE_TRISTATE;
	PinCfg.OpenDrain = PINSEL_PINMODE_NORMAL;
	PinCfg.Portnum = 0;
	PinCfg.Funcnum = PINSEL_FUNC_2;
	PinCfg.Pinnum = 2;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 3;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Funcnum = PINSEL_FUNC_1;
	PinCfg.Pinnum = 23;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 24;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 25;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 26;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Portnum = 1;
	PinCfg.Funcnum = PINSEL_FUNC_3;
	PinCfg.Pinnum = 30;
	PINSEL_ConfigPin(&PinCfg);
	PinCfg.Pinnum = 31;
	PINSEL_ConfigPin(&PinCfg);

	ADC_Init(LPC_ADC, 50000);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN0, SET);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN1, SET);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN2, SET);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN3, SET);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN4, SET);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN5, SET);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN6, SET);
	ADC_IntConfig (LPC_ADC, ADC_ADINTEN7, SET);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_0,ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_1,ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_2,ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_3,ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_4,ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_5,ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_6,ENABLE);
	ADC_ChannelCmd(LPC_ADC, ADC_CHANNEL_7,ENABLE);
	ADC_BurstCmd(LPC_ADC,ENABLE);
	NVIC_EnableIRQ(ADC_IRQn);


}

void ADC_IRQHandler(void){
	static uint16_t corr[8] = {2047,2047,2047,2047,2047,2047,2047,2047};
	static int32_t ADC_val[8];
	const ADC_CHANNEL_SELECTION ADC_SEL[8] = {ADC_CHANNEL_4,ADC_CHANNEL_5,ADC_CHANNEL_0,ADC_CHANNEL_1,ADC_CHANNEL_2,ADC_CHANNEL_3,ADC_CHANNEL_6,ADC_CHANNEL_7};
	uint8_t i;

	NVIC_ClearPendingIRQ(ADC_IRQn);

	if(sampels < 1000){
		for(i = 0;i<8;i++){
			if(ADC_ChannelGetStatus(LPC_ADC, ADC_SEL[i], ADC_DATA_DONE)){
				ADC_val[i] = (ADC_val[i] + ADC_ChannelGetData(LPC_ADC, ADC_SEL[i]) - corr[i])/2;
				if((g_Mdata[i].State != ST_ON)&&(g_Mdata[i].State != ST_MANUAL)){
					if(ADC_val[i] < 0){
						corr[i]--;
					}else{
						corr[i]++;
					}
				}
				g_Sample_Current[i] += abs(ADC_val[i]);
				sampels++;
			}
		}
	}else{
		process_post(&Power_process,event_samples_done,NULL);
		NVIC_DisableIRQ(ADC_IRQn);
	}
	return;
}


void Power_calc(void){
	static uint64_t Watt_count[8] = {0,0,0,0,0,0,0,0};
	static clock_time_t last_time;
	clock_time_t cur_time;
	clock_time_t diff_time;
	int32_t Delta;
	uint8_t i;

	cur_time = clock_time();
	if(cur_time < last_time){
		diff_time = ~(last_time - cur_time);
	}else{
		diff_time = cur_time - last_time;
	}
	last_time = cur_time;

	for(i=0; i<8; i++){
		g_Sample_Current[i] /= 8;
		Delta = g_Mdata[i].Current_RMS - g_Sample_Current[i];
		Delta = (abs(Delta)/200)+1;
		if(g_Mdata[i].Current_RMS < g_Sample_Current[i]){
			g_Mdata[i].Current_RMS+=Delta;
		}else{
			g_Mdata[i].Current_RMS-=Delta;
		}

		g_Mdata[i].Watt = (g_Voltage_RMS * g_Mdata[i].Current_RMS)/1000000;
		Watt_count[i] += (g_Mdata[i].Watt * diff_time);
		g_Mdata[i].Watt_H += (Watt_count[i]/CLOCK_HOUR);
		Watt_count[i] = Watt_count[i]%CLOCK_HOUR;
		g_Sample_Current[i] = 0;
	}
	sampels = 0;

	NVIC_EnableIRQ(ADC_IRQn);
	return;
}

Power_point_data_t * Get_Energy_point_data(uint16_t Energy_point){
	uint8_t i;
	for(i = 0 ;i < 8 ;i++){
		if(g_Mdata[i].Number == Energy_point){
			return &g_Mdata[i];
		}
	}
	return NULL;
}

PROCESS_THREAD(relay_process, ev, data){
	static uint8_t led_blinked = 0;

	PROCESS_BEGIN();

	etimer_set(&led_blink_tm,CLOCK_SECOND/4);
	while (ev != PROCESS_EVENT_EXIT){
		uint8_t i;
		PROCESS_WAIT_EVENT();
		if(ev == PROCESS_EVENT_TIMER){
			if(etimer_expired(&led_blink_tm)){
				led_blinked++;
				for(i=0; i<8; i++){
					if(relay_state_known[i] == false){
						if(GPIO_ReadValue(RELAY_LED_PORT[i]) & RELAY_LED_PIN[i]){
							GPIO_ClearValue(RELAY_LED_PORT[i], RELAY_LED_PIN[i]);
						}else{
							GPIO_SetValue(RELAY_LED_PORT[i], RELAY_LED_PIN[i]);
						}
					}else{
						if(g_Mdata[i].State == ST_MANUAL){
							if((led_blinked % 20)== 0){
								if(g_Mdata[i].output_enabled){
									GPIO_ClearValue(RELAY_LED_PORT[i], RELAY_LED_PIN[i]);
								}else{
									GPIO_SetValue(RELAY_LED_PORT[i], RELAY_LED_PIN[i]);
								}
							}else{
								if(g_Mdata[i].output_enabled){
									GPIO_SetValue(RELAY_LED_PORT[i], RELAY_LED_PIN[i]);
								}else{
									GPIO_ClearValue(RELAY_LED_PORT[i], RELAY_LED_PIN[i]);
								}
							}
						}
					}
				}
				etimer_set(&led_blink_tm,CLOCK_SECOND/4);
			}

			if(etimer_expired(&relay_puls_tm) && (g_Relay_active == true)){
				for(i=0; i<8; i++){
					GPIO_ClearValue(RELAY_ON_PORT[i],RELAY_ON_PIN[i]);
					GPIO_ClearValue(RELAY_OFF_PORT[i],RELAY_OFF_PIN[i]);
				}
				g_Relay_active = false;
			}
		}
	}
	PROCESS_END();
}

PROCESS_THREAD(Power_process, ev, data){
	static struct etimer SCP_send_tm;
	static struct etimer SCP_debug_tm;
	uint8_t i = 0;

	PROCESS_BEGIN();
	etimer_set(&SCP_debug_tm, CLOCK_SECOND * 10);
	//g_Calibrate_val = ini_getl("Measurement", "Calibrate_V", 0, "Calibrate voltage, Change to 0 to start:", inifile_machine);
	while (ev != PROCESS_EVENT_EXIT){
		PROCESS_WAIT_EVENT();
		if(ev == event_samples_done){
			Power_calc();
		}

		if(ev == PROCESS_EVENT_TIMER){

			if(etimer_expired(&SCP_send_tm)){
				if(MULTIPD_test == false){
					for(i = 0; i<8; i++){
						if(g_Mdata[i].Watt_H != g_Mdata[i].Watt_H_send){
							g_Mdata[i].Watt_H_send = g_Mdata[i].Watt_H;
							char buffer[50];
							sprintf(buffer,"CurrentWattHour=%d",g_Mdata[i].Watt_H_send);
							BUFFER_SCP_MSG_RESERVERINGSETINFO(g_Mdata[i].Resvnr, buffer,0);
						}
					}
				}
				etimer_set(&SCP_send_tm, CLOCK_SECOND * 120);
			}
			if(etimer_expired(&SCP_debug_tm)){
			    //printf("Voltage: %dV \r\n",g_Voltage_RMS/1000);
				if(MULTIPD_test == false){
					for(i = 0; i<8; i++){
						printf("%d> I: %d w: %d WH: %d \r\n",i+1,g_Mdata[i].Current_RMS, g_Mdata[i].Watt,g_Mdata[i].Watt_H);
					}
				}
				etimer_set(&SCP_debug_tm, CLOCK_SECOND *10);
			}
		}

	}
	PROCESS_END();
}


PROCESS_THREAD(MULTIpdtest_process, ev, data){
	static bool test_ok;
	static struct etimer test_tm;
	static uint8_t i;
	PROCESS_BEGIN();
	printf("TEST: START!\r\n");
	MULTIPD_test = true;
	for(i=0; i<8; i++){
		Relay_disable(i+1);
	}

	test_ok = true;
	for(i=0; i<8; i++){
		printf("TEST: Relay %d \r\n", i+1);
		if(g_Mdata[i].Current_RMS > 100){
			printf("TEST: FAILED! Relay %d is not off\r\n", i+1);
			test_ok = false;
		}
		Relay_enable(i+1);
		etimer_set(&test_tm, CLOCK_SECOND*5);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&test_tm));
		if(g_Mdata[i].Current_RMS < 100){
			printf("TEST: FAILED! Relay %d is not on\r\n", i+1);
			test_ok = false;
		}else{
			printf("TEST: ON OK Relay %d current is %d\r\n", i+1,g_Mdata[i].Current_RMS);
		}
		Relay_disable(i+1);
		etimer_set(&test_tm, CLOCK_SECOND*8);
		PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&test_tm));
		if(g_Mdata[i].Current_RMS > 100){
			printf("TEST: FAILED! Relay %d is not off\r\n", i+1);
			test_ok = false;
		}else{
			printf("TEST: OFF OK Relay %d current is %d\r\n", i+1,g_Mdata[i].Current_RMS);
		}
	}
	printf("TEST: all leds off? Y/N\r\n", i+1);
	PROCESS_WAIT_EVENT_UNTIL(ev == event_test_input);
	if(*((char *)data) != 'Y'){
		test_ok = false;
		printf("TEST: FAILED! led failure\r\n");
	}
	for(i=0; i<8; i++){
		GPIO_SetValue(RELAY_LED_PORT[i], RELAY_LED_PIN[i]);
		printf("TEST: is led %d on? Y/N\r\n", i+1);
		PROCESS_WAIT_EVENT_UNTIL(ev == event_test_input);
		if(*((char *)data) != 'Y'){
			test_ok = false;
			printf("TEST: FAILED! led %d failure\r\n", i+1);
		}
		GPIO_ClearValue(RELAY_LED_PORT[i],RELAY_LED_PIN[i]);
	}
	printf("TEST: All gate buttons work? Y/N\r\n", i+1);
	PROCESS_WAIT_EVENT_UNTIL(ev == event_test_input);
	if(*((char *)data) != 'Y'){
		test_ok = false;
		printf("TEST: FAILED! buttons failure\r\n", i+1);
	}
	if(test_ok == true){
		printf("TEST: DONE! PASSED\r\n");
	}else{
		printf("TEST: DONE! FAILED!!\r\n");
	}
	MULTIPD_test = false;

	PROCESS_END();
}
