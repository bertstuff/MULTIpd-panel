#include <Device/Devices.h>
#include <core/Extensions/ini/minIni.h>
#include <Core/Debug/Error_check.h>
#include <Core/Process/process.h>
#include <Core/Peripherals/I2C_Dev_Init.h>
#include <Core/Util/math_util.h>
#include <Device/I2C/9554/I2C_9554.h>
#include <Driver/include/lpc_types.h>
#include <Core/Timer/etimer.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "EMP_800.h"

#define DISABLE_EMP800 0

#define REG_LINE1		1
#define REG_LINE2		2
#define REG_LINE3		3
#define REG_LINE4		4
#define REG_LINE5		5
#define REG_LINE6		6
#define REG_CLEAR		7
#define REG_BLOCK		8

PROCESS(EMP_800, "muntproever process");
void EMP_800_set_blocking(bool enable);

uint8_t EMP_800_event;
int32_t EMP_800_value;
Status  EMP_800_status = SUCCESS;


const uint8_t coin_val[6] = {100,200,50,20,0,10};
uint32_t coin_received = 0;


void EMP_800_init(){
	g_Devices_available.Coin_acceptor_available = ini_getbool("Device","Coin_acceptor_available",false,"Coin available:",inifile_device);
	if(g_Devices_available.Coin_acceptor_available == false){
		return;
	}
	EMP_800_event = process_alloc_event(); //Claim a event number
	error_check_add(TYPE_STATUS, &EMP_800_status, ERROR_EMP800);
    I2C_Dev_Init(EMP800_MAX_SPEED_HZ);
    EMP_800_enable();
}

void EMP_800_send(uint8_t reg, uint8_t val){
	I2C_M_SETUP_Type Dev_buffer;
	uint8_t buffer[2];
	Status stat;

	buffer[0] = reg;
	buffer[1] = val;

	Dev_buffer.sl_addr7bit = 0xB5 >> 1 ;
	Dev_buffer.tx_data = buffer;
	Dev_buffer.tx_length = 2;
	Dev_buffer.retransmissions_max = 5;
	Dev_buffer.rx_data = NULL;
	Dev_buffer.rx_length = 0;
#if DISABLE_EMP800 == 0
	stat = I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
#else
	stat = SUCCESS;
#endif

	if(stat == ERROR){
		if(EMP_800_status == SUCCESS){
		  printf("EMP800: i2c error\r\n");
		}
	}

	EMP_800_status = stat;
	return;
}

void EMP_800_enable(void){
	process_start(&EMP_800, NULL);
}

void EMP_800_disable(void){
	process_exit(&EMP_800);
}

int32_t EMP_800_get_value(void){
	return EMP_800_value;
}

int32_t EMP_800_get_value_added(void){
	int32_t val_change =  EMP_800_value - coin_received;
	coin_received = EMP_800_value;

	return val_change;
}

void EMP_800_reset_value(void){
	EMP_800_send(REG_CLEAR, 1);
	EMP_800_value = 0;
	coin_received = 0;
}

void EMP_800_set_blocking(bool enable){
	if(enable == true){
		EMP_800_send(REG_BLOCK, 0);
	}else{
		EMP_800_send(REG_BLOCK, 1);
	}
	return;
}

PROCESS_THREAD(EMP_800, ev, data)
{
	static struct etimer poll_tm;
	static uint8_t prev_state[6];

	if(etimer_expired(&poll_tm)){
		etimer_set(&poll_tm,CLOCK_SECOND);
	}

	PROCESS_BEGIN();
	EMP_800_value = 0;
	EMP_800_set_blocking(false);

	while(ev != PROCESS_EVENT_EXIT){
		I2C_M_SETUP_Type Dev_buffer;
		uint8_t tx_buffer[1];
		uint8_t rx_buffer[6];
		Status stat;
		uint8_t i;

		tx_buffer[0] = REG_LINE1;

		Dev_buffer.sl_addr7bit = 0xB5 >> 1 ;
		Dev_buffer.tx_data = tx_buffer;
		Dev_buffer.tx_length = 1;
		Dev_buffer.retransmissions_max = 5;
		Dev_buffer.rx_data = rx_buffer;
		Dev_buffer.rx_length = 6;
#if DISABLE_EMP800 == 0
		stat = I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING);
#else
		rx_buffer[0] = 0;
		rx_buffer[1] = 0;
		rx_buffer[2] = 0;
		rx_buffer[3] = 0;
		rx_buffer[4] = 0;
		rx_buffer[5] = 0;
		stat = SUCCESS;
#endif
		if(stat == ERROR){
			if(EMP_800_status == SUCCESS){
			  printf("EMP800: i2c error\r\n");
			}
		}else{
			EMP_800_status = stat;
			EMP_800_value = 0;
			for(i = 0; i < 6; i++){
				EMP_800_value += rx_buffer[i] * coin_val[i];
				if(prev_state[i] != rx_buffer[i]){
					printf("EMP_800:%d X %.2f coin inserted\r\n",rx_buffer[i],Centen_to_Euro(coin_val[i]));
					prev_state[i] = rx_buffer[i];
					process_post(PROCESS_BROADCAST, EMP_800_event,NULL);
				}
			}
		}
		PROCESS_WAIT_EVENT_UNTIL(ev == PROCESS_EVENT_TIMER);
	}
	printf("EMP_800: process end\r\n");
	EMP_800_set_blocking(true);
	PROCESS_END();
}
