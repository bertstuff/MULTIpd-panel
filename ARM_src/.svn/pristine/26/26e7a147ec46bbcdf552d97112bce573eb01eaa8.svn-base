#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#include "SCR_Menu_handler_auth.h"
#include "SCR_Menu_handler_int.h"


#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_handler.h>
#include <Screen/Menu/SCR_Menu_draw.h>
#include <Screen/SCR_Text.h>
#include <Screen/SCR_Message.h>

#include <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Core/Timer/stimer.h>
#include <math.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <Core/protocol/Secure/sha1/sha1.h>
#include <string.h>
#include <Global.h>

struct Menu_auth_t glob_auth_menu_set;
struct Menu_t glob_auth_menu;

uint8_t Cur_auth_lev = AUTH_NO;
struct stimer license_tm;
uint32_t cur_license_code;
uint32_t cur_Service_code = 0;
uint32_t cur_admin_code = 0;
extern uint32_t idle_count;

const char AUTH_KEY[7] = {0x84,0xa2,0x0a,0x03,0x4e,0x3d,0xd9};

uint16_t SCR_calc_licence_code(uint8_t auth_lev);

/*--------------Functions---------------*/
void SCR_create_license_code(void){
	uint8_t i,j;
	char str[4] = {'A','S','M','D'};

	if(stimer_expired(&license_tm)){
		create_license_code:
		srand(idle_count);
		cur_license_code = (rand() % 10000);
		stimer_set(&license_tm,60);
	}

	//check if code is different from user and admin password
	j = 0;
	for(i = AUTH_ADMIN;i <= AUTH_DEBUG; i = (i<<1)){
		printf("%c=%d ",str[j], SCR_calc_licence_code(i));
		if(SCR_calc_licence_code(i) == cur_Service_code){
			idle_count++;
			goto create_license_code;
		}
		if(SCR_calc_licence_code(i) == cur_admin_code){
			idle_count++;
			goto create_license_code;
		}
		j++;
	}
	printf("\r\n");
return;
}

bool SCR_authorization_screen_needed(void){
	if(cur_menu->Settings != NULL){
		//check auth level
		if(!((Cur_auth_lev & cur_menu->Settings->auth_lev)||(cur_menu->Settings->auth_lev == AUTH_NO))){
			//change next menu in a auth menu
			glob_auth_menu.Items = 0;
			glob_auth_menu.prev_menu = cur_menu->prev_menu;
			glob_auth_menu.Settings = cur_menu->Settings;
			glob_auth_menu.Type = M_AUTH;
			glob_auth_menu.Menu = &glob_auth_menu_set;
			glob_auth_menu.Name = REF_TEXT(S_login);
			//memmove(glob_auth_menu.Name,S_login,sizeof(char *[LANG_CNT]));
			((struct Menu_message_t *)glob_auth_menu.Menu)->next_menu = cur_menu;
			cur_menu = &glob_auth_menu;
			SCR_create_license_code();
			return false;
		}
	}
	return true;
}

uint16_t SCR_calc_licence_code(uint8_t auth_lev){
	//create sha1 hash
	SHA1Context SCP_SHA1;
	char key[10];
	uint8_t Key_hash[SHA1_HASH_SIZE];
	uint32_t final_code;

	memmove(key,AUTH_KEY,7);
	key[7] = auth_lev;
	key[8] = cur_license_code & 0xFF;
	key[9] = (cur_license_code >> 8)& 0xFF;
	SHA1Init (&SCP_SHA1);
	SHA1Update (&SCP_SHA1, key, 10);
	SHA1Final (&SCP_SHA1,Key_hash);
	final_code = Key_hash[0] + (Key_hash[1] << 8);
	final_code &= 0xFFF;
	return final_code;
}

uint8_t SCR_check_license_code(int32_t input_code){
	uint16_t code , auth_lev;


	if((input_code == cur_admin_code)&&(cur_admin_code != 0)){
		return AUTH_ADMIN;
	}

	if((input_code == cur_Service_code)&&(cur_Service_code != 0)){
		return AUTH_SERVICE;
	}

	for(auth_lev = AUTH_ADMIN; auth_lev <= AUTH_DEBUG;auth_lev = (auth_lev<<1)){
		code = SCR_calc_licence_code(auth_lev);
		if((input_code & 0xFFF) == code){
			return auth_lev;
		}
	}

	return false;
}

PT_THREAD(SCR_auth_handler(process_event_t ev, process_data_t data)){
	PT_BEGIN(&menu_pt);
	if(keypad_pressed(ev)){
		uint8_t button = keypad_button(ev, data);
		if(button == C_RETURN){
			SCR_leave_menu(H_MENU_CANCEL);
		}else if((button == C_OK)||(cur_edit_pos == 4)){
			Cur_auth_lev = SCR_check_license_code(cur_int_val);
			if(!(Cur_auth_lev & cur_menu->Settings->auth_lev)){
				PT_SPAWN(&menu_pt,&message_pt,SCR_Message_screen(GET_TEXT(S_Wrong_Password)));
				SCR_leave_menu(H_MENU_CANCEL);
			}else{
				BUFFER_SCP_MSG_ALERT__SERVICE_MENU_ENTERED(Cur_auth_lev);
				SCR_enter_menu();
			}
		}else{
			if(cur_edit_pos < 4){
				cur_int_val *= 10;
				cur_int_val += button;
				cur_edit_pos ++;
				SCR_build_menu(false);
			}
		}
	}
	PT_END(&menu_pt);
}


#endif
