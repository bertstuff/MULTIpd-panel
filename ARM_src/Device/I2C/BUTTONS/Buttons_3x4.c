#include "Buttons_3x4.h"
#include <Device/I2C/8574/I2C_8574.h>
#include <Core/Timer/etimer.h>
#include <Core/Debug/Debug_Terminal.h>
#include <Core/Debug/Error_check.h>
#include <Device/Devices.h>
#include <Core/Extensions/ini/minIni.h>
#include <Driver/include/lpc17xx_gpio.h>

#ifndef KEYPAD_IO_SETTINGS
#define KEYPAD_IO_SETTINGS
	#define KEYPAD_H1_PORT 2
	#define KEYPAD_H1_PIN (1<<2)
	#define KEYPAD_H2_PORT 2
	#define KEYPAD_H2_PIN (1<<3)
	#define KEYPAD_H3_PORT 2
	#define KEYPAD_H3_PIN (1<<4)
	#define KEYPAD_H4_PORT 2
	#define KEYPAD_H4_PIN (1<<5)

	#define KEYPAD_V1_PORT 2
	#define KEYPAD_V1_PIN (1<<6)
	#define KEYPAD_V2_PORT 2
	#define KEYPAD_V2_PIN (1<<7)
	#define KEYPAD_V3_PORT 2
	#define KEYPAD_V3_PIN (1<<8)

	#define KEYPAD_BACKLIGHT_PORT 2
	#define KEYPAD_BACKLIGHT_PIN (1<<1)
#endif

static struct etimer Button_poll_tm;

//prototype
PROCESS(Button_process, "Button process");
uint8_t Button3x4_get_value(void);
void Button3x4_disable_led(void);
uint8_t Button3x4_IO_com(uint8_t output);
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

void Button3x4_I2C_init(uint8_t address){
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

	if(Button_address == GPIO_ADDR){
		GPIO_ClearValue(KEYPAD_BACKLIGHT_PORT,KEYPAD_BACKLIGHT_PIN);
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
		if(Button_address == GPIO_ADDR){
			//IO
			byte_val = Button3x4_IO_com(KeyRow);
		}else{
			// PCF8574
			Output_val = ~KeyRow ;
			Dev_buffer.sl_addr7bit = Button_address ;
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



void keypad_init(uint8_t address){
	g_Devices_available.Keypad_available = ini_getbool("Device","Keypad_available",true,"Keypad available:",inifile_device);
	if(g_Devices_available.Keypad_available == false){
		return;
	}
	if(address == GPIO_ADDR){
		Button3x4_IO_init();
	}else{
		Button3x4_I2C_init(address);
	}
}

bool keypad_pressed(process_event_t ev){
	if(ev == event_Button3x4_pressed){
		return true;
	}
	return false;
}

uint8_t keypad_button(process_event_t ev, process_data_t data){
	if(ev == event_Button3x4_pressed){
		return *((uint8_t *)data);
	}
	return C_IDLE;
}


uint8_t Button3x4_IO_com(uint8_t output){
	uint8_t intput = 0;
	uint8_t i;


	GPIO_SetValue(KEYPAD_BACKLIGHT_PORT,KEYPAD_BACKLIGHT_PIN);
	GPIO_SetValue(KEYPAD_V1_PORT,KEYPAD_V1_PIN);
	GPIO_SetValue(KEYPAD_V2_PORT,KEYPAD_V2_PIN);
	GPIO_SetValue(KEYPAD_V3_PORT,KEYPAD_V3_PIN);

	switch(output){
	case (1<<4):
		GPIO_ClearValue(KEYPAD_V1_PORT,KEYPAD_V1_PIN);
		break;
	case (1<<5):
		GPIO_ClearValue(KEYPAD_V2_PORT,KEYPAD_V2_PIN);
		break;
	case (1<<6):
		GPIO_ClearValue(KEYPAD_V3_PORT,KEYPAD_V3_PIN);
		break;
	default:
		break;
	}

	for(i=0; i<4; i++){
		switch(i){
		case 0:
			if(GPIO_ReadValue(KEYPAD_H1_PORT) & KEYPAD_H1_PIN){
				intput |= (1<<i);
			}
			break;
		case 1:
			if(GPIO_ReadValue(KEYPAD_H2_PORT) & KEYPAD_H2_PIN){
				intput |= (1<<i);
			}
			break;
		case 2:
			if(GPIO_ReadValue(KEYPAD_H3_PORT) & KEYPAD_H3_PIN){
				intput |= (1<<i);
			}
			break;
		case 3:
			if(GPIO_ReadValue(KEYPAD_H4_PORT) & KEYPAD_H4_PIN){
				intput |= (1<<i);
			}
			break;
		default:
			break;
		}
	}

	return intput;
}

void Button3x4_IO_init(void){
	Button_address = GPIO_ADDR;

	GPIO_SetDir(KEYPAD_H1_PORT,KEYPAD_H1_PIN,DIR_INPUT);
	GPIO_SetDir(KEYPAD_H2_PORT,KEYPAD_H2_PIN,DIR_INPUT);
	GPIO_SetDir(KEYPAD_H3_PORT,KEYPAD_H3_PIN,DIR_INPUT);
	GPIO_SetDir(KEYPAD_H4_PORT,KEYPAD_H4_PIN,DIR_INPUT);

	GPIO_SetDir(KEYPAD_V1_PORT,KEYPAD_V1_PIN,DIR_OUTPUT);
	GPIO_SetDir(KEYPAD_V2_PORT,KEYPAD_V2_PIN,DIR_OUTPUT);
	GPIO_SetDir(KEYPAD_V3_PORT,KEYPAD_V3_PIN,DIR_OUTPUT);

	GPIO_SetDir(KEYPAD_BACKLIGHT_PORT,KEYPAD_BACKLIGHT_PIN,DIR_OUTPUT);
}

