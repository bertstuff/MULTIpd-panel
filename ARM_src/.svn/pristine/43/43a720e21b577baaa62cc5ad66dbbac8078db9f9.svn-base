#include "Buttons_3x4.h"
#include <Device/I2C/8574/I2C_8574.h>
#include <Core/Timer/etimer.h>
#include <Core/Debug/Debug_Terminal.h>
#include <Core/Debug/Error_check.h>

static struct etimer Button_poll_tm;

//prototype
PROCESS(Button_process, "Button process");
uint8_t Button3x4_get_value(void);
void Button3x4_disable_led(void);

////////////I2C-button matrix: /////
//PCF8574P
//
// P4  P5  P6
//
//	1	2	3 	P0
//	4	5	6	P1
//	7	8	9	P2
//	A	0	B	P3
//
// P7 = Backlight


//Global variables
const uint8_t Button3x4_val[4][3] = {
		{ 1, 2, 3},
		{ 4, 5, 6},
		{ 7, 8, 9},
		{'X',0,'V'}
};


Status Button3x4_Error = ERROR;

uint8_t cur_buttonval = C_IDLE;
uint8_t Button_address = 0xFF;

process_event_t event_Button3x4_pressed;

void Button3x4_init(uint8_t address){
	I2C_8574_Init ();

	if (address < 10) {
		Button_address = (PCF8574A_addr >> 1) + address ;
	}else{
		Button_address = address >> 1 ;
	}
	
	error_check_add(TYPE_STATUS,&Button3x4_Error,ERROR_KEYPAD);
	event_Button3x4_pressed = process_alloc_event(); //Claim a event number
	Button3x4_disable_led();
	return;
}

void Button3x4_activate(bool state){
	if(Button_address == 0xFF){
		return;
	}

	if(state == true){
		process_start(&Button_process, NULL);
	}else{
		process_exit(&Button_process);
	}
}

PROCESS_THREAD(Button_process, ev, data)
{
	uint8_t New_buttonval;

	if(etimer_expired(&Button_poll_tm)) {
      etimer_set(&Button_poll_tm, CLOCK_SECOND / 10);
    }

	PROCESS_BEGIN();
	etimer_set(&Button_poll_tm, CLOCK_SECOND / 10);
    cur_buttonval = Button3x4_get_value();
    while (ev != PROCESS_EVENT_EXIT){ //loop till exit
    	New_buttonval = Button3x4_get_value();
    	if(New_buttonval != cur_buttonval){
    		 cur_buttonval = New_buttonval;

    		 if(New_buttonval != C_IDLE){
       		 	 process_post(PROCESS_BROADCAST, event_Button3x4_pressed, &cur_buttonval);
    		 	 printf("Button3x4: button %d pressed\r\n",cur_buttonval);
    		 }
    	}
    	PROCESS_YIELD();
	}
    Button3x4_disable_led();
	PROCESS_END();
}

void Button3x4_disable_led(void){
	if(Button_address == 0xFF){
		return;
	}
    I2C_M_SETUP_Type Dev_buffer;
    uint8_t Output_val = 0;
    Output_val = 0x00;
	Dev_buffer.sl_addr7bit = Button_address ;// PCF8574
	Dev_buffer.tx_data = &Output_val;
	Dev_buffer.tx_length = 1;
	Dev_buffer.retransmissions_max = 5;
	Dev_buffer.rx_data =  NULL;
	Dev_buffer.rx_length = 0;
    if(I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING)==ERROR){
		printf("Button3x4: ic not found \r\n");
		Button3x4_Error = ERROR;
	}else{
		Button3x4_Error = SUCCESS;
	}
}

uint8_t Button3x4_get_value(void){
	I2C_M_SETUP_Type Dev_buffer;
	uint8_t byte_val;
	uint8_t X_val,Y_val;
	uint8_t Output_val;
	uint8_t KeyRow = (1 << 4);

	for(X_val = 0; X_val < 3 ;X_val++){
		Output_val = ~KeyRow ;

		Dev_buffer.sl_addr7bit = Button_address ;// PCF8574
		Dev_buffer.tx_data = &Output_val;
		Dev_buffer.tx_length = 1;
		Dev_buffer.retransmissions_max = 5;
		Dev_buffer.rx_data =  &byte_val;
		Dev_buffer.rx_length = 1;
		if(I2C_MasterTransferData(LPC_I2C1, &Dev_buffer, I2C_TRANSFER_POLLING)==ERROR){
			if(Button3x4_Error != ERROR){
				printf("Button3x4: ic not found \r\n");
			}
			Button3x4_Error = ERROR;
			return C_IDLE;
		}
		Button3x4_Error = SUCCESS;


		for(Y_val = 0; Y_val < 4; Y_val++){
			if(~byte_val & (1<<Y_val)){
				break;
			}
		}

		if(Y_val > 3){
			KeyRow = (KeyRow<<1);
		}else{
			return Button3x4_val[Y_val][X_val];
		}
	}
	return C_IDLE;
}
