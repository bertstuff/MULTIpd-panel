#include <Board/Def_config.h>
#if((MENU_MODULE == 1)&&(SERVICE_PASS_MODULE == 1))
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_handler.h>
#include <Core/Extensions/code/access_code.h>
#include <Device/I2C/RFID/RFID.h>
#include <Device/I2C/BUTTONS/Keypad_Parser.h>


PT_THREAD(SCR_write_pass_handler(process_event_t ev, process_data_t data));
void SCR_write_pass(bool enter_menu);

void SCR_save_refnr_menu_handler(menu_handler_t event){
	if(event == H_MENU_INIT){
		cur_int_val = Load_code_from_sd(256);
	}

	if(event == H_MENU_ACCEPT){
		Save_code_to_sd(256, cur_int_val);
	}
	return;
}

void SCR_Pass_Menu(struct Menu_t * Menu){
	struct Menu_item_t * Pass_item;
	struct Menu_item_t * refnr_item;
	struct Menu_item_t * write_pass_item;
	struct Menu_t * Menu_Pass;
	struct Menu_t * Menu_refnr;


	Pass_item = menu_add_menu_item(Menu,REF_TEXT(S_Auth_pass));
	Menu_Pass = menu_new_menu(REF_TEXT(S_Auth_pass), Pass_item);
	menu_set_menu_auth(Menu_Pass,AUTH_MANUFACTURE | AUTH_DEBUG);

	refnr_item = menu_add_menu_item(Menu_Pass,REF_TEXT(S_Refnr));
	Menu_refnr = menu_new_int_menu(REF_TEXT(S_Refnr),REF_TEXT(S_no_char),0,1000000000,0,&cur_int_val,sizeof(uint32_t), refnr_item);
	menu_set_menu_function(Menu_refnr, SCR_save_refnr_menu_handler);

	write_pass_item = menu_add_menu_item(Menu_Pass,REF_TEXT(S_write_pass));
	menu_new_custom_menu(REF_TEXT(S_write_pass), SCR_write_pass_handler, SCR_write_pass,write_pass_item);

}

void SCR_write_pass(bool enter_menu){
	Point_t point = {.x=120,.y=5};

	g_RFID_card = RFID_get_card(RFID_get_reader(0x00));

	Edip_Clear_Display();
	Edip_Set_Font(&H1Font);
	Edip_Draw_String(point,GET_TEXT(S_write_pass));
	point.y += 30;
	Edip_Set_Font(&StdFont);
	if((g_RFID_card->card_action.Info_write == false) &&(g_RFID_card->card_present == true)){
		Edip_Draw_String(point,"Write ok");
	}else{
		Edip_Draw_String(point,"Write failed");
	}
	return;
}

PT_THREAD(SCR_write_pass_handler(process_event_t ev, process_data_t data)){
	PT_BEGIN(&menu_pt);
	if(ev == PROCESS_EVENT_INIT){

	}

	if(keypad_pressed(ev)){
		uint8_t button= *((uint8_t *)data);
		if(button == C_RETURN){
			SCR_leave_menu(H_MENU_CANCEL);
			PT_EXIT(&menu_pt);
		}
	}

	if(ev == event_card_change){

		g_RFID_card = RFID_get_card(RFID_get_reader(0x00));
		if(g_RFID_card->card_present == true){
			g_RFID_card->card_info.auth_lev = AUTH_SERVICE;
			g_RFID_card->card_info.Language = g_Language;
			g_RFID_card->card_info.card_version = 0;
			g_RFID_card->card_action.Info_write = true;
			g_RFID_card->card_action.Unlock_code_write = true;
			PT_WAIT_UNTIL(&menu_pt,(ev == event_card_change)||(ev == event_card_done));
			SCR_write_pass(false);
		}
	}

	PT_END(&menu_pt);
}

#endif
