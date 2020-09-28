#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#include "SCR_Menu_handler_auth.h"
#include "SCR_Menu_handler_int.h"


#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_handler.h>
#include <Screen/Menu/SCR_Menu_draw.h>
#include <Screen/SCR_Text.h>
#include <Screen/SCR_Message.h>

#include <Device/I2C/BUTTONS/Buttons_3x4.h>

#include <Core/Timer/stimer.h>
#include <math.h>
#include <Core/protocol/Secure/sha1/sha1.h>
#include <string.h>
#include <Global.h>

struct Menu_message_t glob_message_menu_set;
struct Menu_t glob_message_menu;

/*--------------Functions---------------*/
bool SCR_Message_screen_needed(void){
	if(cur_menu->Settings != NULL){

		if(cur_menu->Settings->exit_message != NULL){
			//change next menu in a auth menu
			glob_message_menu.Items = 0;
			glob_message_menu.prev_menu = cur_menu->prev_menu;
			glob_message_menu.Settings = cur_menu->Settings;
			glob_message_menu.Type = M_MESSAGE;
			glob_message_menu.Menu = &glob_message_menu_set;
			((struct Menu_auth_t *)glob_message_menu.Menu)->next_menu = cur_menu->prev_menu;
			cur_menu = &glob_message_menu;
			return true;
		}
	}
	return false;
}

PT_THREAD(SCR_message_handler(process_event_t ev, process_data_t data)){
	PT_BEGIN(&menu_pt);
	PT_SPAWN(&menu_pt,&message_pt,SCR_Message_screen(GET_TEXT(cur_menu->Settings->exit_message)));
	SCR_enter_menu();
	PT_END(&menu_pt);
}
#endif
