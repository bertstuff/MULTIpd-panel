#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#include "SCR_Menu_handler_menu.h"
#include "SCR_Menu_handler_auth.h"

#include <Screen/Menu/SCR_Menu_handler.h>
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_draw.h>
#include <Screen/SCR_Text.h>

#include <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Device/IO/BUTTONS/EBM-Basismodule-buttons.h>

bool SCR_check_menu_item_auth(struct Menu_item_t * menu_item){
	if(menu_item->next_menu == NULL){
		return false;
	}

	if(menu_item->next_menu->Settings == NULL){
		return true;
	}
	if(menu_item->next_menu->Settings->auth_lev == AUTH_NO){
		return true;
	}
	if(Cur_auth_lev & menu_item->next_menu->Settings->auth_lev){
		return true;
	}
	if(menu_item->next_menu->Settings->auth_lev & AUTH_SERVICE){
		return true;
	}
	return false;
}

struct Menu_item_t * SCR_find_prev_menu(struct Menu_item_t * Cur_item){
	struct Menu_item_t * find_menu;
	//find_menu = (struct Menu_item_t *)cur_menu->Menu;
	find_menu = Cur_item;
	while(find_menu->prev != NULL){
		find_menu = find_menu->prev;
		if(SCR_check_menu_item_auth(find_menu) == true){
			return find_menu;
		}
	}

	return NULL;
}

struct Menu_item_t * SCR_find_next_menu(struct Menu_item_t * Cur_item){
	struct Menu_item_t * find_menu;
	//find_menu = (struct Menu_item_t *)cur_menu->Menu;
	find_menu = Cur_item;
	while(find_menu->next != NULL){
		find_menu = find_menu->next;
		if(SCR_check_menu_item_auth(find_menu) == true){
			return find_menu;
		}
	}

	return NULL;
}

uint8_t SCR_get_page_number(struct Menu_item_t * Cur_item){
	uint8_t item = 0;
	struct Menu_item_t * find_menu = Cur_item;
	while(1){
		find_menu = SCR_find_prev_menu(find_menu);
		if(find_menu == NULL){
			break;
		}
		item++;

	}
	return item / MENU_ITEMS_PER_PAGE;
}

struct Menu_item_t * SCR_get_item_on_page(struct Menu_t * menu, uint8_t page_nr, uint8_t item_nr){
	struct Menu_item_t * find_menu;
	struct Menu_item_t * prev_menu;

	find_menu = (struct Menu_item_t *)menu->Menu;
	uint8_t item = 0;

	//find begin of menu
	while(1){
		prev_menu = SCR_find_prev_menu(find_menu);
		if(prev_menu == NULL){
			break;
		}else{
			find_menu = prev_menu;
		}
	}

	//go to page number and item
	while(1){
		if(item == ((page_nr * MENU_ITEMS_PER_PAGE) + item_nr)){
			return find_menu;
		}
		find_menu = SCR_find_next_menu(find_menu);
		if(find_menu == NULL){
			return NULL;
		}
		item++;
	}

}

PT_THREAD(SCR_menu_handler(process_event_t ev, process_data_t data)){
	uint8_t page_nr;
	struct Menu_item_t * find_menu;

	PT_BEGIN(&menu_pt);
	if(keypad_pressed(ev)){
		uint8_t button = keypad_button(ev, data);
		switch(button){
		case C_RETURN://go level lower in menu
			SCR_leave_menu(H_MENU_CANCEL);
			break;
		case C_OK:
			page_nr = SCR_get_page_number((struct Menu_item_t *)cur_menu->Menu);
			find_menu = SCR_get_item_on_page(cur_menu, page_nr + 1, 0);
			if(find_menu != NULL){
				cur_menu->Menu = find_menu;
				SCR_build_menu(false);
			}
			break;
		case 0:
			page_nr = SCR_get_page_number((struct Menu_item_t *)cur_menu->Menu);
			if(page_nr == 0){
				break;
			}
			cur_menu->Menu = SCR_get_item_on_page(cur_menu, page_nr - 1, 0);
			SCR_build_menu(false);
			break;
		default:
			page_nr = SCR_get_page_number((struct Menu_item_t *)cur_menu->Menu);
			find_menu = SCR_get_item_on_page(cur_menu, page_nr, button - 1);
			if(find_menu != NULL){
				cur_menu->Menu = find_menu;
				SCR_enter_menu();
			}
			break;
		}
	}

	PT_END(&menu_pt);
}
#endif
