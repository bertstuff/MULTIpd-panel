#include "Keypad_Parser.h"

bool keypad_pressed(process_event_t ev){
	if(ev == event_Button3x4_pressed){
		return true;
	}else if(ev == event_keypad_pressed){
		return true;
	}
	return false;
}

uint8_t keypad_button(process_event_t ev, process_data_t data){
	if(ev == event_Button3x4_pressed){
		return *((uint8_t *)data);
	}else if(ev == event_keypad_pressed){
		return *((uint8_t *)data);
	}
	return C_IDLE;
}
