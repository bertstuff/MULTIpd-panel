#include "Keypad_3x4.h"
#include <Device/I2C/8574/I2C_8574.h>
#include <Core/Timer/etimer.h>
#include <Core/Debug/Debug_Terminal.h>
#include <Core/Debug/Error_check.h>

#define keypad_key_addr PCF8574A_addr
#define keypad_io_addr  PCF8574_addr

static struct etimer keypad_poll_tm;
static struct etimer buzzer_tm;

//prototype
PROCESS(keypad_process, "Keypad process");
uint8_t Keypad_get_value(void);

////////////I2C-keypad: /////
//PCF8574P
//P0=A,P1=B,P2=C,P3=1,P4=2,P5=4,P6=3
//
//	A	B	C
//
//	1	2	3 		1
//	4	5	6		2
//	7	8	9		4
//	A	0	B		3

//p7=input1

//PCF8574AP
//p0 = LEDROOD
//p1 = LEDGEEL
//p2 = LEDGROEN
//p3 = BUZZER
//p4 = OUTPUT1
//p5 = OUTPUT2
//p6 = OUTPUT3
//p7 = OUTPUT4


//Global variables
const uint8_t keypad_val[4][3] = {{1,2,3},{4,5,6},{C_STER,0,C_HECK},{7,8,9}};
bool Keypad_Input1;
Status Keypad_Error = ERROR;

uint8_t Output_val = 0xFF;
uint8_t cur_keyval = C_IDLE;
uint8_t keypad_address;

process_event_t event_keypad_pressed;

void Keypad_init(uint8_t address){
	I2C_8574_Init ();
	keypad_address = address;
	event_keypad_pressed = process_alloc_event(); //Claim a event number
	error_check_add(TYPE_STATUS,&Keypad_Error,ERROR_KEYPAD);
	return;
}

void Keypad_activate(bool state){
	if(state == true){
		process_start(&keypad_process, NULL);
	}else{
	    Keypad_set_output(T_BUZZER, false);
		process_exit(&keypad_process);
	}
}

PROCESS_THREAD(keypad_process, ev, data)
{
	uint8_t New_keyval;

	if(etimer_expired(&keypad_poll_tm)) {
      etimer_set(&keypad_poll_tm, CLOCK_SECOND / 10);
    }

	PROCESS_BEGIN();
	etimer_set(&keypad_poll_tm, CLOCK_SECOND / 10);
    cur_keyval = Keypad_get_value();
    Keypad_set_output(T_LEDGROEN, true);
    Keypad_set_output(T_LEDROOD, false);
    while (ev != PROCESS_EVENT_EXIT){ //loop till exit
    	New_keyval = Keypad_get_value();
    	if(New_keyval != cur_keyval){
    		 cur_keyval = New_keyval;

    		 if(New_keyval != C_IDLE){
    			 etimer_set(&buzzer_tm, CLOCK_SECOND / 4);
    			// Keypad_set_output(T_BUZZER, true);
    		 	 process_post(PROCESS_BROADCAST, event_keypad_pressed, &cur_keyval);
    		 	 printf("Keypad: button %d pressed\r\n",cur_keyval);
    		 }
    	}
    	if(etimer_expired(&buzzer_tm)){
    		Keypad_set_output(T_BUZZER, false);
    	}
    	PROCESS_YIELD();
	}
    Keypad_set_output(T_LEDROOD, true);
    Keypad_set_output(T_LEDGEEL, false);
    Keypad_set_output(T_LEDGROEN, false);
    Keypad_set_output(T_BUZZER, false);
	PROCESS_END();
}

uint8_t Keypad_get_value(void){
	I2C_M_SETUP_Type Dev_buffer;
	uint8_t byte_val;
	uint8_t X_val,Y_val;
	uint8_t Output_val, KeyRow = 1;

	for(X_val = 0; X_val < 3 ;X_val++){
		//Output_val = (0xF8 | (0x07 ~KeyRow));
		Output_val = ~KeyRow;

		Dev_buffer.sl_addr7bit = (keypad_key_addr >> 1) + keypad_address ;// PCF8574
		Dev_buffer.tx_data = &Output_val;
		Dev_buffer.tx_length = 1;
		Dev_buffer.retransmissions_max = 5;
		Dev_buffer.rx_data =  &byte_val;
		Dev_buffer.rx_length = 1;
		if(I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING)==ERROR){
			if(Keypad_Error != ERROR){
				printf("Keypad: keypad ic not found \r\n");
			}
			Keypad_Error = ERROR;
			return C_IDLE;
		}
		Keypad_Error = SUCCESS;


		Keypad_Input1 = (byte_val & T_INPUT1);
		byte_val = ((~byte_val) >> 3);
		for(Y_val = 0; Y_val < 4; Y_val++){
			if(byte_val & (1<<Y_val)){
				break;
			}
		}

		if(Y_val > 3){
			KeyRow = (KeyRow<<1);
		}else{
			return keypad_val[Y_val][X_val];
		}
	}
	return C_IDLE;
}

bool Keypad_get_input1(void){
	Keypad_get_value();
	return Keypad_Input1;
}

void Keypad_set_output(uint8_t Pin,bool state){
	if(state == false){
		Output_val |= Pin;
	}else{
		Output_val &= ~Pin;
	}
	if(I2C_8574_WriteOutput (keypad_io_addr + (1<<keypad_address), &Output_val, 0x00) == ERROR){
		printf("Keypad: output ic not found \r\n");
	}
}
