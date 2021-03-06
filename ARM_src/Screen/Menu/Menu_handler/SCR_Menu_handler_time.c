#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#include "SCR_Menu_handler_int.h"

#include <Screen/Menu/SCR_Menu_handler.h>
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_draw.h>
#include <Screen/SCR_Text.h>

#include <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Device/IO/BUTTONS/EBM-Basismodule-buttons.h>

#include <time.h>

struct tm cur_time_val;

PT_THREAD(SCR_time_handler(process_event_t ev, process_data_t data)){
	PT_BEGIN(&menu_pt);
	bool val_change = false;
	int32_t new_val;

	if(keypad_pressed(ev)){
		uint8_t button = keypad_button(ev, data);
		switch(button){
		case C_RETURN://go level lower in menu
			SCR_leave_menu(H_MENU_CANCEL);
			break;
		case C_OK:
			SCR_leave_menu(H_MENU_ACCEPT);
			break;
		default:
			switch(cur_edit_pos){
				default:
				case 0:
					cur_edit_pos = 0;
					cur_time_val.tm_hour = 0;
					cur_time_val.tm_min = 0;
					new_val = cur_time_val.tm_hour + (button * 10);
					if(new_val < 24){
						cur_time_val.tm_hour = new_val;
					}
					cur_edit_pos++;
					break;
				case 1:
					new_val = cur_time_val.tm_hour + button;
					if(new_val < 24){
						cur_time_val.tm_hour = new_val;
					}
					cur_edit_pos++;
					break;
				case 2:
					new_val = cur_time_val.tm_min + (button * 10);
					if(new_val < 60){
						cur_time_val.tm_min = new_val;
					}
					cur_edit_pos++;
					break;
				case 3:
					new_val = cur_time_val.tm_min + button;
					if(new_val < 60){
						cur_time_val.tm_min = new_val;
					}
					cur_edit_pos++;
					break;
			}
			SCR_build_menu(false);
			val_change = true;
		}
	}

	//Start handler function
	if(val_change == true){
		if((cur_menu->Settings != NULL)&&(cur_menu->Settings->Handler != NULL)){
			(cur_menu->Settings->Handler)(H_MENU_VAL_CHANGE);
		}
	}
	PT_END(&menu_pt);
}
#endif
