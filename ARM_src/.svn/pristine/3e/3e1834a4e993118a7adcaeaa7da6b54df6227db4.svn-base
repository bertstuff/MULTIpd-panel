#include "EBM-Basismodule-buttons.h"
#include <Driver/include/lpc17xx_gpio.h>
#include <Core/Timer/etimer.h>
#include <stdbool.h>

const EBM_Button_place_t Button_place_list[8] = {
  {.Port_sw = 2,.Pin_sw = 11,.Port_led = 1,.Pin_led = 18},//button 1
  {.Port_sw = 2,.Pin_sw = 13,.Port_led = 1,.Pin_led = 19},//button 2
  {.Port_sw = 0,.Pin_sw = 27,.Port_led = 1,.Pin_led = 20},//button 3
  {.Port_sw = 0,.Pin_sw = 28,.Port_led = 1,.Pin_led = 21},//button 4
  {.Port_sw = 1,.Pin_sw = 22,.Port_led = 1,.Pin_led = 26},//button 5
  {.Port_sw = 1,.Pin_sw = 23,.Port_led = 1,.Pin_led = 27},//button 6
  {.Port_sw = 1,.Pin_sw = 24,.Port_led = 1,.Pin_led = 28},//button 7
  {.Port_sw = 1,.Pin_sw = 25,.Port_led = 1,.Pin_led = 29} //button 8
};

EBM_Button_state_t Button_state_list[8] = {
  {.Sw_stat = false,.SW_changed = false,.Led_stat = false},//button 1
  {.Sw_stat = false,.SW_changed = false,.Led_stat = false},//button 2
  {.Sw_stat = false,.SW_changed = false,.Led_stat = false},//button 3
  {.Sw_stat = false,.SW_changed = false,.Led_stat = false},//button 4
  {.Sw_stat = false,.SW_changed = false,.Led_stat = false},//button 5
  {.Sw_stat = false,.SW_changed = false,.Led_stat = false},//button 6
  {.Sw_stat = false,.SW_changed = false,.Led_stat = false},//button 7
  {.Sw_stat = false,.SW_changed = false,.Led_stat = false} //button 8
};
void EBM_Buttons_init(void);
void EBM_Button_poll(void);
PROCESS(EBM_button_process, "EBM Button process");


process_event_t event_EBM_button_Pressed;
process_event_t event_EBM_button_Released;

uint8_t button_pressed;
uint8_t button_released;
bool button_invert;
uint8_t nr_buttons;

void EBM_button_init(uint8_t buttons, bool invert){
	  nr_buttons = buttons;
	  EBM_Buttons_init();
	  process_start(&EBM_button_process, NULL);
	  EBM_Button_poll();
	  button_invert = invert;
	  return;
}

PROCESS_THREAD(EBM_button_process, ev, data){
  static struct etimer etimer;

  PROCESS_BEGIN();
  while (ev != PROCESS_EVENT_EXIT){ //loop till exit
	    etimer_set(&etimer, CLOCK_SECOND/10);
	    PROCESS_WAIT_UNTIL(etimer_expired(&etimer));
	    EBM_Button_poll();
  }
  PROCESS_END();
}


void EBM_Buttons_init(void){
  uint8_t i;
  event_EBM_button_Pressed = process_alloc_event(); //Claim a event number
  event_EBM_button_Released = process_alloc_event(); //Claim a event number
  for(i = 0; i < nr_buttons; i++){
	GPIO_SetDir(Button_place_list[i].Port_sw,(1<<Button_place_list[i].Pin_sw), DIR_INPUT);
	GPIO_SetDir(Button_place_list[i].Port_led,(1<<Button_place_list[i].Pin_led), DIR_OUTPUT);
  }
  return;
}

void EBM_Button_poll(void){
	bool Button_stat;
	uint8_t i;
	uint32_t port_stat;

	for(i = 0; i < nr_buttons; i++){
		port_stat = GPIO_ReadValue(Button_place_list[i].Port_sw);
		Button_stat = (port_stat & (1 << Button_place_list[i].Pin_sw)) > 0 ?true:false;
		if(Button_state_list[i].Sw_stat != Button_stat){
			Button_state_list[i].Sw_stat = Button_stat;
			Button_state_list[i].SW_changed = true;
			if(Button_stat != button_invert){
				button_pressed = i;
				process_post(PROCESS_BROADCAST, event_EBM_button_Pressed, &button_pressed);
			}else{
				button_released = i;
				process_post(PROCESS_BROADCAST, event_EBM_button_Released, &button_released);
			}
		}else{
			Button_state_list[i].SW_changed = false;
		}
	}
	return;
}


void EBM_Button_led(int8_t Button, bool state){
	if(state == true){
		GPIO_SetValue(Button_place_list[Button].Port_led,1 << Button_place_list[Button].Pin_led);
	}else{
		GPIO_ClearValue(Button_place_list[Button].Port_led,1 << Button_place_list[Button].Pin_led);
	}
	Button_state_list[Button].Led_stat = state;
	return;
}

bool EBM_Button_pressed(uint8_t button){
	if(button_invert == true){
		return !Button_state_list[button].Sw_stat;
	}else{
		return Button_state_list[button].Sw_stat;
	}
}

