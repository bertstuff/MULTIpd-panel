/* Include files */
#include "Servicepin.h"
#include <Board/Def_config.h>

/* Global Functions */

bool Servicepin(void){
  GPIO_SetDir(BOARD_SERVICE_PORT,(uint32_t)BOARD_SERVICE_PIN, DIR_INPUT);
  return !(GPIO_ReadValue(BOARD_SERVICE_PORT) & ((uint32_t)BOARD_SERVICE_PIN));
}

void set_Servicepin_led(bool state){
	GPIO_SetDir(BOARD_SERVICE_PORT,(uint32_t)BOARD_SERVICE_PIN, DIR_OUTPUT);
	if(state){
		GPIO_SetValue(BOARD_SERVICE_PORT,(uint32_t)BOARD_SERVICE_PIN);
	}else{
		GPIO_ClearValue(BOARD_SERVICE_PORT,(uint32_t)BOARD_SERVICE_PIN);
	}
}
