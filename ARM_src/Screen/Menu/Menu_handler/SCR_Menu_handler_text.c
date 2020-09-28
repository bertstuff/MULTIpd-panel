#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#include "SCR_Menu_handler_text.h"

#include <Screen/Menu/SCR_Menu_handler.h>
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_draw.h>
#include <Screen/SCR_Text.h>

#include <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Device/IO/BUTTONS/EBM-Basismodule-buttons.h>

PT_THREAD(SCR_text_handler(process_event_t ev, process_data_t data)){
	PT_BEGIN(&menu_pt);

	if(keypad_pressed(ev)){
		uint8_t button = keypad_button(ev, data);
		switch(button){
		case C_RETURN://go level lower in menu
			SCR_leave_menu(H_MENU_CANCEL);
			break;
		case 0:
			if(((struct Menu_text_t *)cur_menu->Menu)->next != NULL){
				cur_menu->Menu = ((struct Menu_text_t *)cur_menu->Menu)->next;
				SCR_build_menu(false);
			}
			break;
		case C_OK:
			if(((struct Menu_text_t *)cur_menu->Menu)->prev != NULL){
				cur_menu->Menu = ((struct Menu_text_t *)cur_menu->Menu)->prev;
				SCR_build_menu(false);
			}
			break;
		default:
			break;
		}
	}

	PT_END(&menu_pt);
}

#endif
