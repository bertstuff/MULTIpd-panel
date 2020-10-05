#include <Board/Def_config.h>
#if(MENU_MODULE == 1)
/*Include*/

#include <Core/Process/process.h>
#include <Core/Thread/pt.h>
#include <Core/Type/type.h>
#include <Device/I2C/BUTTONS/Keypad_Parser.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Display.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Draw.h>
#include <Device/I2C/Edip_240B/EDIP_240B_Text.h>
#include <Device/UART/MDB-C21/MDB-ARB-C21-7TUBES.h>
#include <Global.h>
#include <stdint.h>
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/Menu/SCR_Menu_handler.h>
#include <Screen/SCR_Init.h>
#include <Screen/SCR_Text.h>
#include <Screen/Transaction/SCR_Transaction_Receipt.h>
#include <Screen/Transaction/SCR_Transaction_statistics.h>
#include <Screen/Transaction/SCR_Transaction_init.h>
#include <Core/Util/math_util.h>
#include <processes/IO_process.h>


/*prototypes*/
void SCR_tube_state(bool enter_menu);
void SCR_draw_filling_menu(bool enter_menu);
void SCR_statistics(bool enter_menu);
PT_THREAD(SCR_filling_handler(process_event_t ev, process_data_t data));
PT_THREAD(SCR_emptying_handler(process_event_t ev, process_data_t data));
PT_THREAD(SCR_statistics_handler(process_event_t ev, process_data_t data));

/*global var*/
extern int32_t select_val;

//--------------------------------------------exit handler------------------------------------------------------------//

//--------------------------------------------Menu------------------------------------------------------------//
void SCR_Cash_Menu(struct Menu_t * Menu){
	struct Menu_t * Menu_cash;
	struct Menu_item_t * Item_filling,* Item_Emptying, * Item_cash;

	Item_cash = menu_add_menu_item(Menu,REF_TEXT(S_Cash2));

	Menu_cash = menu_new_menu(REF_TEXT(S_Cash2), Item_cash);
	Item_filling = menu_add_menu_item(Menu_cash,REF_TEXT(S_Filling));
	Item_Emptying = menu_add_menu_item(Menu_cash,REF_TEXT(S_Emptying));

	menu_new_custom_menu(REF_TEXT(S_Filling), SCR_filling_handler, SCR_draw_filling_menu, Item_filling);
	menu_new_custom_menu(REF_TEXT(S_Emptying), SCR_emptying_handler, SCR_draw_filling_menu, Item_Emptying);
	return;
}
//-------------------------------------------- custom Menu------------------------------------------------------------//

PT_THREAD(SCR_filling_handler(process_event_t ev, process_data_t data)){
	PT_BEGIN(&menu_pt);
	if(ev == PROCESS_EVENT_INIT){
		MDB_Ontvang(true);
	}

	if(keypad_pressed(ev)){
		uint8_t button= *((uint8_t *)data);
		if(button == C_RETURN){
			MDB_Ontvang(false);
			SCR_leave_menu(H_MENU_CANCEL);
			PT_EXIT(&menu_pt);
		}
	}

	if(ev == event_coin_inserted){
		*((uint16_t *)data) = 0;
		g_trans_statistics.Coins_in_cash_box += (Cash_Box);
		Cash_Box = 0;
		SCR_draw_filling_menu(false);

	}
	PT_END(&menu_pt);
}

PT_THREAD(SCR_emptying_handler(process_event_t ev, process_data_t data)){
	PT_BEGIN(&menu_pt);
	if(ev == PROCESS_EVENT_INIT){
		MDB_Betaal(200);
	}

	if(keypad_pressed(ev)){
		uint8_t button= *((uint8_t *)data);
		if(button == C_RETURN){
			MDB_Ontvang(false);
			SCR_leave_menu(H_MENU_CANCEL);
			PT_EXIT(&menu_pt);
		}
	}

	if(ev == event_coin_payed){
		if(*((uint16_t *)data) != 0){
			*((uint16_t *)data) = 0;
			MDB_Betaal(200);
		}else{
			*((uint16_t *)data) = 0;
			MDB_Betaal(0);
		}
		SCR_draw_filling_menu(false);
	}
	PT_END(&menu_pt);
}


void SCR_draw_filling_menu(bool enter_menu){
	Point_t point = {.x=120,.y=10};
	if(enter_menu == true){
		Edip_Clear_Display();
		Edip_Set_Font(&StdFont);

		Edip_Set_Font(&H1Font);
		Edip_Draw_String(point,"%s",GET_TEXT(cur_menu->Name));
	}
	SCR_tube_state(enter_menu);
	return;
}


void SCR_tube_state(bool enter_menu){

	Point_t point = {.x=30,.y=67};
	uint8_t i;
	float f_coinvalue;
	Point_t line1 = {.x=0,.y=42};
	Point_t line2 = {.x=0,.y=85};

	Edip_Change_Font_Setting(&StdFont,MODE,M_REPLACE);
	if(enter_menu == true){
		Edip_Set_Font(&StdFont);
		Edip_Draw_Line( line1, line2);

		point.y = 47;
		point.x = 30;

		for(i = 0; i < 7;i++){
			if(CoinValue[i] != 0){
				f_coinvalue = (CoinValue[i] * CoinSF);
				f_coinvalue /= 100;
				Edip_Draw_String(point,"%c%.2f",C_EURO,f_coinvalue);
				line1.x += 60;
				line2.x += 60;
				Edip_Draw_Line( line1, line2);
				point.x += 60;
				if(i == 3){
					point.y += 43;
					line1.y += 43;
					line2.y += 43;
					point.x = 30;
					line1.x = 0;
					line2.x = 0;
					Edip_Draw_Line( line1, line2);

				}
			}
		}
		line1.x = 0;
		line1.y = 85;
		line2.x = 240;
		line2.y = 85;
		Edip_Draw_Line( line1, line2);
		line1.y = 42;
		line2.y = 42;
		Edip_Draw_Line( line1, line2);
	}
	point.x=30;
	point.y=67;

	Edip_Set_Font(&StdFont);
	for(i = 0; i < 7;i++){
		if(CoinValue[i] != 0){
			Edip_Draw_String(point," %d ",CoinTube[i]);
			point.x += 60;
			if(i == 3){
				point.x = 30;
				point.y = 110;
			}
		}
	}
}
#endif
