#include <Device/UART/XBA/XBA.h>
#include <Core/Process/process.h>
#include <core/Extensions/ini/minIni.h>
#include <Device/Devices.h>

#define BILL_VALUE_TYPE1  500
#define BILL_VALUE_TYPE2  1000
#define BILL_VALUE_TYPE3  2000
#define BILL_VALUE_TYPE4  5000
#define BILL_VALUE_TYPE5  10000

enum{
	XBA_ACCEPT = 0x02,
	XBA_STATUS_REQUEST = 0x0C,
	XBA_REJECT = 0x0F,
	XBA_ACCEPT_OK = 0x10,
	XBA_REJECT_OK = 0x11,
	XBA_HOLD_IN_ESCROW = 0x18,
	XBA_MOTOR_FAILURE = 0x20,
	XBA_CHECKSUM_ERROR = 0x21,
	XBA_BILL_JAM = 0x22,
	XBA_BILL_REMOVE = 0x23,
	XBA_STACKER_OPEN = 0x24,
	XBA_SENSOR_PROBLEM = 0x25,
	XBA_BILL_FISH = 0x27,
	XBA_STACKER_PROBLEM = 0x28,
	XBA_BILL_REJECT = 0x29,
	XBA_INVALID_COMMAND = 0x2A,
	XBA_STATUS_EXCLUSION = 0x2F,
	XBA_RESET = 0x30,
	XBA_ENABLE = 0x3E,
	XBA_BILL_TYPE1 = 0x40,
	XBA_BILL_TYPE2 = 0x41,
	XBA_BILL_TYPE3 = 0x42,
	XBA_BILL_TYPE4 = 0x43,
	XBA_BILL_TYPE5 = 0x44,
	XBA_INHIBIT = 0x5E,
	XBA_POWER_UP1 = 0x80,
	XBA_BILL_VALIDATED = 0x81,
	XBA_POWER_UP2 = 0x8F,
}XBA_command_t;


////prototypes//////
void XBA_Receive_data(char data);

//global variables;
UART_select_t XBA_COM;
process_event_t event_Bill_validated;
process_event_t event_Bill_error;
uint16_t last_value;
uint8_t data_send[1];
XBA_error_t XBA_error;

void XBA_init(UART_select_t Com){
	UART_config_type config;
	g_Devices_available.Cash_available = ini_getbool("Device","Cash_available",false,"Cash available:",inifile_device);
	if(g_Devices_available.Cash_available == false){
		return;
	}
	config.settings.Baud_rate = 9600;
	config.settings.Databits = UART_DATABIT_8;
	config.settings.Parity = UART_PARITY_EVEN;
	config.settings.Stopbits = UART_STOPBIT_1;
	config.New_data = XBA_Receive_data;
	config.Auto_CTS = false;
	config.Auto_RTS = false;

	XBA_COM = Com;
	event_Bill_validated = process_alloc_event(); //Claim a event number
	event_Bill_error = process_alloc_event(); //Claim a event number
	UART_poort_Init(Com, config);
}


void XBA_Receive_data(char data){
	static uint8_t prev_data;

	switch(data){
	case XBA_POWER_UP2:
		if(prev_data == XBA_POWER_UP1){
			data_send[0] = XBA_ACCEPT;
			UART_send(XBA_COM,data_send,1);
		}
		break;
	case XBA_BILL_TYPE1:
		if(prev_data == XBA_BILL_VALIDATED){
			 last_value = BILL_VALUE_TYPE1;
			 process_post(PROCESS_BROADCAST,event_Bill_validated,&last_value);
		}
		break;
	case XBA_BILL_TYPE2:
		if(prev_data == XBA_BILL_VALIDATED){
			 last_value = BILL_VALUE_TYPE2;
			 process_post(PROCESS_BROADCAST,event_Bill_validated,&last_value);
		}
		break;
	case XBA_BILL_TYPE3:
		if(prev_data == XBA_BILL_VALIDATED){
			 last_value = BILL_VALUE_TYPE3;
			 process_post(PROCESS_BROADCAST,event_Bill_validated,&last_value);
		}
		break;
	case XBA_BILL_TYPE4:
		if(prev_data == XBA_BILL_VALIDATED){
			 last_value = BILL_VALUE_TYPE4;
			 process_post(PROCESS_BROADCAST,event_Bill_validated,&last_value);
		}
		break;
	case XBA_BILL_TYPE5:
		if(prev_data == XBA_BILL_VALIDATED){
			 last_value = BILL_VALUE_TYPE5;
			 process_post(PROCESS_BROADCAST,event_Bill_validated,&last_value);
		}
		break;
	case XBA_ACCEPT_OK:
	case XBA_REJECT_OK:
		data_send[0] = XBA_STATUS_REQUEST;
		UART_send(XBA_COM,data_send,1);
		break;
	case XBA_MOTOR_FAILURE:
		XBA_error = E_XBA_MOTOR_FAILURE;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	case XBA_CHECKSUM_ERROR:
		XBA_error = E_XBA_CHECKSUM_ERROR;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	case XBA_BILL_JAM:
		XBA_error = E_XBA_BILL_JAM;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	case XBA_STACKER_OPEN:
		XBA_error = E_XBA_STACKER_OPEN;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	case XBA_SENSOR_PROBLEM:
		XBA_error = E_XBA_SENSOR_PROBLEM;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	case XBA_BILL_FISH:
		XBA_error = E_XBA_BILL_FISH;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	case XBA_STACKER_PROBLEM:
		XBA_error = E_XBA_STACKER_PROBLEM;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	case XBA_BILL_REJECT:
		XBA_error = E_XBA_BILL_REJECT;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	case XBA_INVALID_COMMAND:
		XBA_error = E_XBA_INVALID_COMMAND;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	case XBA_INHIBIT:
	case XBA_ENABLE:
	case XBA_BILL_REMOVE:
		XBA_error = E_XBA_NONE;
		break;
	case XBA_STATUS_EXCLUSION:
		XBA_error = E_XBA_NONE;
		process_post(PROCESS_BROADCAST,event_Bill_error,&XBA_error);
		break;
	default:
		break;
	}

	prev_data = data;
}

void XBA_Bill_Hold_in_escrow(void){
	data_send[0] = XBA_HOLD_IN_ESCROW;
	UART_send(XBA_COM,data_send,1);
}

void XBA_Bill_Accept(void){
	data_send[0] = XBA_ACCEPT;
	UART_send(XBA_COM,data_send,1);
}

void XBA_Bill_reject(void){
	data_send[0] = XBA_REJECT;
	UART_send(XBA_COM,data_send,1);
}

void XBA_Enable(void){
	data_send[0] = XBA_ENABLE;
	UART_send(XBA_COM,data_send,1);
}

void XBA_Disable(void){
	data_send[0] = XBA_INHIBIT;
	UART_send(XBA_COM,data_send,1);
}

void XBA_Reset(void){
	data_send[0] = XBA_RESET;
	UART_send(XBA_COM,data_send,1);
}
