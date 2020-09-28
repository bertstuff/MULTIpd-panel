/* Include files */
#include "Debug_led.h"
#include <Device/IO/BUTTONS/EBM-Basismodule-buttons.h>

/* Global Functions */
void Debug_led_init(void){
    GPIO_SetDir(BOARD_LED1_PORT,(uint32_t)BOARD_LED1_PIN, DIR_OUTPUT);
    GPIO_SetDir(BOARD_LED2_PORT,(uint32_t) BOARD_LED2_PIN, DIR_OUTPUT);
    return;
}

void Debug_led1(bool state){
  if(state == true){
    GPIO_SetValue(BOARD_LED1_PORT,(uint32_t)BOARD_LED1_PIN);
  }else{
    GPIO_ClearValue(BOARD_LED1_PORT,(uint32_t)BOARD_LED1_PIN);
  }
  return;
}

void Debug_led2(bool state){
  if(state == true){
    GPIO_SetValue(BOARD_LED2_PORT,(uint32_t)BOARD_LED2_PIN);
  }else{
    GPIO_ClearValue(BOARD_LED2_PORT,(uint32_t)BOARD_LED2_PIN);
  }
  return;
}

void Debug_led_wink(void){

}

void Debug_led_Multipass_EBM(bool state){
  EBM_Button_led(EBM_BUTTON3, state);
  return;
}
