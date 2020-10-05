#include <Board/Def_config.h>
#if(MENU_MODULE == 1)
#include "SCR_Menu_handler_int.h"


#include <Screen/Menu/SCR_Menu_handler.h>
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_draw.h>
#include <Screen/SCR_Text.h>

#include <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Device/IO/BUTTONS/EBM-Basismodule-buttons.h>

#include <math.h>

int32_t cur_int_val;

PT_THREAD(SCR_int_handler(process_event_t ev, process_data_t data)){
	PT_BEGIN(&menu_pt);
	bool val_change = false;
	int64_t new_val;

	if(keypad_pressed(ev)){
		uint8_t button = keypad_button(ev, data);
		switch(button){
		case C_RETURN://go level lower in menu
			SCR_leave_menu(H_MENU_CANCEL);
			break;
		case C_OK:
			if(cur_int_val >= ((struct Menu_int_t *)cur_menu->Menu)->min){
				SCR_leave_menu(H_MENU_ACCEPT);
			}
			break;
		case 0:
			if(cur_edit_pos == 0){
				if(((struct Menu_int_t *)cur_menu->Menu)->min == 0){
					cur_int_val = 0;
					val_change = true;
					SCR_build_menu(false);
				}
				break;
			}
		default:
			if(cur_edit_pos == 0){
				cur_int_val = 0;
			}
			new_val = (cur_int_val * 10)+button;

			if(new_val <= ((struct Menu_int_t *)cur_menu->Menu)->max){
				cur_int_val = new_val;
				val_change = true;
				cur_edit_pos++;
				SCR_build_menu(false);
			}
			break;
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
