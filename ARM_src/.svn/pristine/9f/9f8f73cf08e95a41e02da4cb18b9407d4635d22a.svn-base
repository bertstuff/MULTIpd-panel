#include "EBM_Multipass_io.h"
#include <driver/include/lpc17xx_gpio.h>

 typedef struct{
    uint8_t Port;
    uint8_t Pin;
  }EBM_port_t;

const EBM_port_t EBM_relay_list[4] = {
	{.Port = EBM_RELAY1_PORT,.Pin= EBM_RELAY1_PIN},
	{.Port = EBM_RELAY2_PORT,.Pin= EBM_RELAY2_PIN},
	{.Port = EBM_RELAY3_PORT,.Pin= EBM_RELAY3_PIN},
	{.Port = EBM_RELAY4_PORT,.Pin= EBM_RELAY4_PIN}
};

const EBM_port_t EBM_input_list[3] = {
	{.Port = EBM_INPUT1_PORT,.Pin = EBM_INPUT1_PIN},
	{.Port = EBM_INPUT2_PORT,.Pin = EBM_INPUT2_PIN},
	{.Port = EBM_INPUT3_PORT,.Pin = EBM_INPUT3_PIN},
};

void EBM_Multipass_io_init(void){
	uint8_t i;
	for(i = 0;i < (sizeof(EBM_relay_list)/sizeof(EBM_relay_list[0])); i++){
		GPIO_SetDir(EBM_relay_list[i].Port,(1<<EBM_relay_list[i].Pin), DIR_OUTPUT);
	}

	for(i = 0;i < (sizeof(EBM_input_list)/sizeof(EBM_relay_list[0])); i++){
		GPIO_SetDir(EBM_input_list[i].Port,(1<<EBM_input_list[i].Pin), DIR_INPUT);
	}
}

void EBM_Multipass_Set_output(uint8_t Relay){
	if(Relay < (sizeof(EBM_relay_list)/sizeof(EBM_relay_list[0]))){
		GPIO_SetValue(EBM_relay_list[Relay].Port,(1<<EBM_relay_list[Relay].Pin));
	}
	return;
}

void EBM_Multipass_Clear_output(uint8_t Relay){
	if(Relay < (sizeof(EBM_relay_list)/sizeof(EBM_relay_list[0]))){
		GPIO_ClearValue(EBM_relay_list[Relay].Port,(1<<EBM_relay_list[Relay].Pin));
	}
	return;
}

bool EBM_Multipass_Read_input(uint8_t Input){
	if(Input < (sizeof(EBM_input_list)/sizeof(EBM_input_list[0]))){
		uint32_t port_val = GPIO_ReadValue(EBM_input_list[Input].Port);
		return (port_val & (1<<EBM_input_list[Input].Pin))?false:true;
	}
	return false;
}

uint8_t EBM_Multipass_Read_All_inputs(void){
	uint8_t i;
	uint8_t value = 0x00;
	for(i = 0; i < 4; i++){
		if(EBM_Multipass_Read_input(i)){
			value |= (1<<i);
		}else{
			value &= ~(1<<i);
		}
	}
	return value;
}


