#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#include "SCR_Menu_handler_enum.h"


#include <Screen/Menu/SCR_Menu_handler.h>
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_draw.h>
#include <Screen/SCR_Text.h>

#include <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Device/IO/BUTTONS/EBM-Basismodule-buttons.h>

PT_THREAD(SCR_enum_handler(process_event_t ev, process_data_t data)){
	struct Menu_enum_t * cur_item;
	uint8_t i;

	PT_BEGIN(&menu_pt);


	if(keypad_pressed(ev)){
		uint8_t button = keypad_button(ev, data);
		switch(button){
		case C_RETURN://go level lower in menu
			SCR_leave_menu(H_MENU_CANCEL);
			break;
		case C_OK:
			SCR_leave_menu(H_MENU_ACCEPT);
			break;
		case 0:
			break;
		default:
			cur_item = ((struct Menu_enum_t *)cur_menu->Menu);
			while(1){
				if(cur_item->prev == NULL){
					break;
				}
				cur_item = cur_item->prev;
			}
			for(i = 1; i < button ; i++){
				cur_item = cur_item->next;
				if(cur_item == NULL){
					break;
				}
			}
			if(cur_item != NULL){
				cur_menu->Menu = cur_item;
				SCR_build_menu(false);
			}
			break;
		}
	}
	PT_END(&menu_pt);
}
#endif
