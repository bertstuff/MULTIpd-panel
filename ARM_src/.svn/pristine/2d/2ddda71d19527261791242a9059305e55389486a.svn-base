#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#include <Global.h>
#include <stdint.h>
#include <Screen/Menu/SCR_Menu_func.h>
#include <Screen/SCR_Text.h>

/*global var*/
extern int32_t select_val;
extern int32_t g_max_change_value;
extern bool g_bill_accept[4];
extern int32_t g_min_change_level[4];

//--------------------------------------------Menu------------------------------------------------------------//
void SCR_Bill_settings_Menu(struct Menu_t * Menu_Settings){
	struct Menu_t * Menu_Bill_accept,* Menu_Min_change_level,* Menu_Cash_accept,* Menu_cash_select,* Menu_Max_change_value;

	struct Menu_item_t * Item_Bill_accept,* Item_Min_change_level,* Item_Cash_accept;
	struct Menu_item_t * Item_Cash5,* Item_Cash10,* Item_Cash20,* Item_Cash50,* Item_Max_change_value;

	Item_Cash_accept = menu_add_menu_item(Menu_Settings,REF_TEXT(S_Cash_accept));

	Menu_Cash_accept = menu_new_menu(REF_TEXT(S_Cash), Item_Cash_accept);
	Item_Cash5 = menu_add_menu_item(Menu_Cash_accept,REF_TEXT(S_5euro));
	Item_Cash10 = menu_add_menu_item(Menu_Cash_accept,REF_TEXT(S_10euro));
	Item_Cash20 = menu_add_menu_item(Menu_Cash_accept,REF_TEXT(S_20euro));
	Item_Cash50 = menu_add_menu_item(Menu_Cash_accept,REF_TEXT(S_50euro));
	Item_Max_change_value = menu_add_menu_item(Menu_Cash_accept,REF_TEXT(S_Max_change_value));

	//5euro
	Menu_cash_select = menu_new_menu(REF_TEXT(S_5euro), Item_Cash5);
	Item_Bill_accept = menu_add_menu_item(Menu_cash_select,REF_TEXT(S_Bill_accept));
	Item_Min_change_level = menu_add_menu_item(Menu_cash_select,REF_TEXT(S_Min_change_level));

	Menu_Bill_accept = menu_new_enum_menu(REF_TEXT(S_Bill_accept),&select_val,sizeof(uint32_t),Item_Bill_accept);
	menu_add_enum_item(Menu_Bill_accept, REF_TEXT(S_Accept), 1);
	menu_add_enum_item(Menu_Bill_accept, REF_TEXT(S_Reject), 0);
	menu_add_ini_link(Menu_Bill_accept,"Bill accept","5euro_Accept",inifile_machine);

	Menu_Min_change_level = menu_new_int_menu(REF_TEXT(S_change),REF_TEXT(S_no_char),500,9999,2,&g_min_change_level[0],sizeof(int32_t), Item_Min_change_level);
	menu_add_ini_link(Menu_Min_change_level,"Bill accept","5euro_Change_level",inifile_machine);

	//10euro
	Menu_cash_select = menu_new_menu(REF_TEXT(S_10euro), Item_Cash10);
	Item_Bill_accept = menu_add_menu_item(Menu_cash_select,REF_TEXT(S_Bill_accept));
	Item_Min_change_level = menu_add_menu_item(Menu_cash_select,REF_TEXT(S_Min_change_level));

	Menu_Bill_accept = menu_new_enum_menu(REF_TEXT(S_Bill_accept),&select_val,sizeof(uint32_t),Item_Bill_accept);
	menu_add_enum_item(Menu_Bill_accept, REF_TEXT(S_Accept), 1);
	menu_add_enum_item(Menu_Bill_accept, REF_TEXT(S_Reject), 0);
	menu_add_ini_link(Menu_Bill_accept,"Bill accept","10euro_Accept",inifile_machine);

	Menu_Min_change_level = menu_new_int_menu(REF_TEXT(S_change),REF_TEXT(S_no_char),1000,9999,2,&g_min_change_level[1],sizeof(int32_t), Item_Min_change_level);
	menu_add_ini_link(Menu_Min_change_level,"Bill accept","10euro_Change_level",inifile_machine);

	//20euro
	Menu_cash_select = menu_new_menu(REF_TEXT(S_20euro), Item_Cash20);
	Item_Bill_accept = menu_add_menu_item(Menu_cash_select,REF_TEXT(S_Bill_accept));
	Item_Min_change_level = menu_add_menu_item(Menu_cash_select,REF_TEXT(S_Min_change_level));

	Menu_Bill_accept = menu_new_enum_menu(REF_TEXT(S_Bill_accept),&select_val,sizeof(uint32_t),Item_Bill_accept);
	menu_add_enum_item(Menu_Bill_accept, REF_TEXT(S_Accept), 1);
	menu_add_enum_item(Menu_Bill_accept, REF_TEXT(S_Reject), 0);
	menu_add_ini_link(Menu_Bill_accept,"Bill accept","20euro_Accept",inifile_machine);

	Menu_Min_change_level = menu_new_int_menu(REF_TEXT(S_change),REF_TEXT(S_no_char),2000,9999,2,&g_min_change_level[2],sizeof(int32_t), Item_Min_change_level);
	menu_add_ini_link(Menu_Min_change_level,"Bill accept","20euro_Change_level",inifile_machine);

	//50euro
	Menu_cash_select = menu_new_menu(REF_TEXT(S_50euro), Item_Cash50);
	Item_Bill_accept = menu_add_menu_item(Menu_cash_select,REF_TEXT(S_Bill_accept));
	Item_Min_change_level = menu_add_menu_item(Menu_cash_select,REF_TEXT(S_Min_change_level));

	Menu_Bill_accept = menu_new_enum_menu(REF_TEXT(S_Bill_accept),&select_val,sizeof(uint32_t),Item_Bill_accept);
	menu_add_enum_item(Menu_Bill_accept, REF_TEXT(S_Accept), 1);
	menu_add_enum_item(Menu_Bill_accept, REF_TEXT(S_Reject), 0);
	menu_add_ini_link(Menu_Bill_accept,"Bill accept","50euro_Accept",inifile_machine);

	Menu_Min_change_level = menu_new_int_menu(REF_TEXT(S_change),REF_TEXT(S_no_char),1000,9999,2,&g_min_change_level[3],sizeof(int32_t), Item_Min_change_level);
	menu_add_ini_link(Menu_Min_change_level,"Bill accept","50euro_Change_level",inifile_machine);

	//Max return value
	Menu_Max_change_value = menu_new_int_menu(REF_TEXT(S_change),REF_TEXT(S_no_char),100,9999,2,&g_max_change_value,sizeof(int32_t), Item_Max_change_value);
	menu_add_ini_link(Menu_Max_change_value,"Bill accept","Max_return_value",inifile_machine);

}
#endif
