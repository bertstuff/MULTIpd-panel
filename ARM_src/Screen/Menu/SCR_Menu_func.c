#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#include "SCR_Menu_func.h"
#include "Menu_handler/SCR_Menu_handler_auth.h"
#include "SCR_Menu_handler.h"
#include <Core/Util/U_string.h>
#include <Core/Memory/cmem.h>


	void * prev_mem = NULL; //pointer to linked list to remove whole menu
	
	void menu_set_authorization_level(uint8_t auth_lev){
		Cur_auth_lev = auth_lev;
	}

	uint8_t menu_get_authorization_level(void){
		return Cur_auth_lev;
	}

	void menu_set_service_code(uint32_t user_code){
		cur_Service_code = user_code;
		return;
	}

	void menu_set_admin_code(uint32_t admin_code){
		cur_admin_code = admin_code;
		return;
	}

	struct Menu_t * menu_new_menu(F_TEXT(Menu_Name), struct Menu_item_t * Prev_Menu){
		struct Menu_t * Menu;

		Menu = cmem_malloc(sizeof(struct Menu_t));
		if(Menu == NULL){return NULL;}

		Menu->Items = 0;
		Menu->Name = Menu_Name;
		Menu->Settings = NULL;
		Menu->Type = M_MENU;
		Menu->Menu = NULL;
		Menu->Filename = NULL;
		Menu->Section = NULL;
		Menu->Key = NULL;
		Menu->Variable = NULL;
		if(Prev_Menu == NULL){
			Menu->prev_menu = NULL;
			cur_menu = Menu;
		}else{
			Menu->prev_menu = Prev_Menu->Menu;
			Prev_Menu->next_menu = Menu;
		}
		Menu->prev_mem = prev_mem;
		prev_mem = Menu;


		return Menu;
	}

	struct menu_settings_t * menu_set_menu_function(struct Menu_t * Menu, void (*Handler)(menu_handler_t event)){
		struct menu_settings_t * Setting;

		if(Menu->Settings == NULL){
			Setting = cmem_malloc(sizeof(struct menu_settings_t));
			if(Setting == NULL){return NULL;}

			Setting->auth_lev = AUTH_NO;
			Setting->exit_message = NULL;
			Setting->prev_mem = prev_mem;
			Setting->list_nr = 0;
			prev_mem = Setting;
		}else{
			Setting = Menu->Settings;
		}

		Setting->Handler = Handler;
		Menu->Settings = Setting;
		return Setting;
	}

	struct menu_settings_t * menu_set_menu_auth(struct Menu_t * Menu, uint8_t auth_lev){
		struct menu_settings_t * Setting;

		if(Menu->Settings == NULL){
			Setting = cmem_malloc(sizeof(struct menu_settings_t));
			if(Setting == NULL){return NULL;}

			Setting->Handler = NULL;
			Setting->exit_message = NULL;
			Setting->prev_mem = prev_mem;
			Setting->list_nr = 0;
			prev_mem = Setting;
		}else{
			Setting = Menu->Settings;
		}

		Setting->auth_lev = auth_lev;
		Menu->Settings = Setting;
		return Setting;
	}

	struct menu_settings_t * menu_set_exit_message(struct Menu_t * Menu,F_TEXT(message)){
		struct menu_settings_t * Setting;

		if(Menu->Settings == NULL){
			Setting = cmem_malloc(sizeof(struct menu_settings_t));
			if(Setting == NULL){return NULL;}

			Setting->Handler = NULL;
			Setting->auth_lev = AUTH_NO;
			Setting->prev_mem = prev_mem;
			Setting->list_nr = 0;
			prev_mem = Setting;
		}else{
			Setting = Menu->Settings;

		}
		if(message == NULL){
			Setting->exit_message = NULL;
		}else{
			Setting->exit_message = message;
		}
		Menu->Settings = Setting;
		return Setting;
	}

	struct menu_settings_t * menu_set_menu_nr(struct Menu_t * Menu, uint8_t Menu_nr){
		struct menu_settings_t * Setting;

		if(Menu->Settings == NULL){
			Setting = cmem_malloc(sizeof(struct menu_settings_t));
			if(Setting == NULL){return NULL;}

			Setting->Handler = NULL;
			Setting->auth_lev = AUTH_NO;
			Setting->prev_mem = prev_mem;
			Setting->exit_message = NULL;
			prev_mem = Setting;
		}else{
			Setting = Menu->Settings;
		}
		Setting->list_nr = Menu_nr;
		Menu->Settings = Setting;
		return Setting;
	}

	struct Menu_item_t * menu_add_menu_item(struct Menu_t * Menu, F_TEXT(Item_Name), ...){
		struct Menu_item_t * Item;
		Item = cmem_malloc(sizeof(struct Menu_item_t));
		if(Item == NULL){return NULL;}

		Item->Name = Item_Name;
		if(((struct Menu_item_t *)Menu->Menu) != NULL){
			((struct Menu_item_t *)Menu->Menu)->next = Item;
		}
		Item->prev = (struct Menu_item_t *)Menu->Menu;
		Item->next = NULL;
		Item->next_menu = NULL;
		Item->Menu = Menu;
		Item->prev_mem = prev_mem;
		prev_mem = Item;

		Menu->Items++;
		Menu->Menu = (void *)Item;
		Item->Menu_nr = Menu->Items;
		return Item;
	}

	struct Menu_t * menu_new_int_menu( F_TEXT(Menu_Name), F_TEXT(type), int32_t min, int32_t max, uint8_t decimals, void * Variable, uint8_t Varsize, struct Menu_item_t * Prev_Menu){
		struct Menu_t * Menu;
		struct Menu_int_t * Int_menu;

		Menu = menu_new_menu(Menu_Name,Prev_Menu);
		if(Menu == NULL){return NULL;}

		Int_menu = cmem_malloc(sizeof(struct Menu_int_t));
		if(Int_menu == NULL){return NULL;}

		Int_menu->type = type;
		Int_menu->min = min;
		Int_menu->max = max;
		Int_menu->decimals = decimals;
		Int_menu->prev_mem = prev_mem;
		prev_mem = Int_menu;

		Menu->Type = M_INT;
		Menu->Items++;
		Menu->Menu = (void *)Int_menu;

		Menu->Variable = Variable;
		Menu->Varsize = Varsize;
		return Menu;
	}

	struct Menu_t * menu_new_enum_menu(F_TEXT(Menu_Name), void * Variable, uint8_t Varsize, struct Menu_item_t * Prev_Menu){
		struct Menu_t * Menu;

		Menu = menu_new_menu(Menu_Name,Prev_Menu);
		if(Menu == NULL){return NULL;}
		Menu->Type = M_ENUM;
		Menu->Variable = Variable;
		Menu->Varsize = Varsize;
		return Menu;
	}

	struct Menu_enum_t * menu_add_enum_item(struct Menu_t * Menu, F_TEXT(Enum_Name), int32_t Value){
		struct Menu_enum_t * Enum_Item;
		Enum_Item = cmem_malloc(sizeof(struct Menu_enum_t));
		if(Enum_Item == NULL){return NULL;}

		Enum_Item->Name = Enum_Name;
		if(((struct Menu_enum_t *)Menu->Menu) != NULL){
			((struct Menu_enum_t *)Menu->Menu)->next = Enum_Item;
		}
		Enum_Item->prev = (struct Menu_enum_t *)Menu->Menu;
		Enum_Item->next = NULL;
		Enum_Item->Value = Value;
		Enum_Item->prev_mem = prev_mem;
		prev_mem = Enum_Item;

		Menu->Items++;
		Menu->Menu = (void *)Enum_Item;

		return Enum_Item;
	}

	struct Menu_t * menu_new_time_menu(F_TEXT(Menu_Name), struct tm * Variable, struct Menu_item_t * Prev_Menu){
		struct Menu_t * Menu;
		struct Menu_time_t * Time_menu;

		Menu = menu_new_menu(Menu_Name,Prev_Menu);
		if(Menu == NULL){return NULL;}

		Time_menu = cmem_malloc(sizeof(struct Menu_time_t));
		if(Time_menu == NULL){return NULL;}

		Time_menu->prev_mem = prev_mem;
		prev_mem = Time_menu;

		Menu->Type = M_TIME;
		Menu->Items++;
		Menu->Menu = (void *)Time_menu;

		Menu->Variable = Variable;

		return Menu;
	}

	struct Menu_t * menu_new_text_menu(F_TEXT(Menu_Name), struct Menu_item_t * Prev_Menu){
		struct Menu_t * Menu;

		Menu = menu_new_menu(Menu_Name,Prev_Menu);
		if(Menu == NULL){return NULL;}
		Menu->Type = M_TEXT;

		return Menu;
	}


	struct Menu_text_t * menu_add_text_item(struct Menu_t * Menu,F_TEXT(text)){
		struct Menu_text_t * text_Item;
		text_Item = cmem_malloc(sizeof(struct Menu_text_t));
		if(text_Item == NULL){return NULL;}

		if(((struct Menu_text_t *)Menu->Menu) != NULL){
			((struct Menu_text_t *)Menu->Menu)->next = text_Item;
		}


		text_Item->prev = (struct Menu_text_t *)Menu->Menu;
		text_Item->next = NULL;
		text_Item->func = NULL;
		text_Item->text = text;
		text_Item->prev_mem = prev_mem;
		prev_mem = text_Item;

		Menu->Items++;
		Menu->Menu = (void *)text_Item;

		return text_Item;
	}

	struct Menu_t * menu_new_custom_menu(F_TEXT(Menu_Name),	PT_THREAD((* Menu_handler)(process_event_t ev, process_data_t data)), void (*Menu_draw)(bool enter_menu), struct Menu_item_t * Prev_Menu){
		struct Menu_t * Menu;
		struct Menu_custom_t * Custom_menu;

		Menu = menu_new_menu(Menu_Name,Prev_Menu);
		if(Menu == NULL){return NULL;}
		Custom_menu = cmem_malloc(sizeof(struct Menu_custom_t));
		if(Custom_menu == NULL){return NULL;}

		Custom_menu->Menu_handler = Menu_handler;
		Custom_menu->Menu_draw = Menu_draw;
		Custom_menu->prev_mem = prev_mem;
		prev_mem = Custom_menu;

		Menu->Type = M_CUSTOM;
		Menu->Items++;
		Menu->Menu = (void *)Custom_menu;
		return Menu;
	}

	void menu_add_ini_link(struct Menu_t * Menu, const char *Section, const char *Key, const char *Filename){
		switch(Menu->Type){
		case M_INT:
		case M_ENUM:
		case M_TIME:
			Menu->Section = Section;
			Menu->Key = Key;
			Menu->Filename = Filename;
			break;
		default:
			break;
		}
		return;
	}

	char * menu_malloc_string(char * format, ...){
		char string[90];
		struct Menu_memory_t * mem;
		uint8_t bytes;
		uint16_t malloc_bytes;
		va_list args;
		char * string_ptr;

		va_start (args, format);
		bytes = vsnprintf (string, 90, format, args) +1;
		malloc_bytes = (sizeof(Menu_memory_t) + bytes);
		mem = cmem_malloc(malloc_bytes);
		if(mem == NULL){
			return NULL;
		}
		string_ptr = ((char *)mem) + sizeof(Menu_memory_t);
		memmove(string_ptr,string,bytes);
		mem->prev_mem = prev_mem;
		prev_mem = mem;
		va_end (args);
		return ( char *)(((char*)mem) + sizeof(Menu_memory_t));
	}


	struct Menu_t * menu_new_question_menu( F_TEXT(Menu_Name), F_TEXT(question), struct Menu_item_t * Prev_Menu){
		struct Menu_t * Menu;
		struct Menu_question_t * question_menu;

		Menu = menu_new_menu(Menu_Name,Prev_Menu);
		if(Menu == NULL){return NULL;}

		question_menu = cmem_malloc(sizeof(struct Menu_question_t));
		if(question_menu == NULL){return NULL;}

		question_menu->question = question;
		question_menu->prev_mem = prev_mem;
		prev_mem = question_menu;

		Menu->Type = M_QUESTION;
		Menu->Items++;
		Menu->Menu = (void *)question_menu;

		Menu->Variable = NULL;
		Menu->Varsize = 0;
		return Menu;
	}

	void Release_menu(void){
		struct Menu_memory_t * Mem;
		while(1){
			Mem = (struct Menu_memory_t*)prev_mem;
			prev_mem = Mem->prev_mem;
			cmem_free(Mem);
			if(prev_mem == NULL){
				break;
			}
		}
		return;
	}
#endif
