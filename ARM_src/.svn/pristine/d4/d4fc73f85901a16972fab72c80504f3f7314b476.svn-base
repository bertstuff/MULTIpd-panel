#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

	#include "SCR_Menu_settings.h"

	#include <time.h>
	#include <Core/Extensions/ini/minIni.h>

	#include "Menu_handler/SCR_Menu_handler_auth.h"
	#include "Menu_handler/SCR_Menu_handler_enum.h"
	#include "Menu_handler/SCR_Menu_handler_int.h"
	#include "Menu_handler/SCR_Menu_handler_menu.h"
	#include "Menu_handler/SCR_Menu_handler_text.h"
	#include "Menu_handler/SCR_Menu_handler_time.h"

	bool SCR_load_setting(struct Menu_t * Menu){
		uint8_t i;
		int32_t tmp_val;
		char tmp_section[75];

		switch(cur_menu->Type){
		case M_AUTH:  break;
		case M_MENU:
			break;
		case M_INT:
			//get ini
			if(Menu->Filename != NULL){
				cur_int_val = ini_getl(Menu->Section, Menu->Key, 0, NULL, Menu->Filename);
				break;
			}

			//get var
			if(Menu->Variable != NULL){
				cur_int_val = 0;
				memmove(&cur_int_val,Menu->Variable,Menu->Varsize);
			}
			break;
		case M_ENUM:
			if(Menu->Filename != NULL){
				if((Menu->Settings != NULL)&&(Menu->Settings->list_nr)){
					sprintf(tmp_section,"%s%d",Menu->Section,Menu->Settings->list_nr);
					tmp_val = ini_getl(tmp_section, Menu->Key, 0, NULL, Menu->Filename);
				}else{
					tmp_val = ini_getl(Menu->Section, Menu->Key, 0, NULL, Menu->Filename);
				}
			}else if(Menu->Variable != NULL){
				tmp_val = 0;
				memmove(&tmp_val,Menu->Variable,Menu->Varsize);
			}else{
				break;
			}

			//Find current value of enum

			//find begin of enum list
			while(1){
				if(((struct Menu_enum_t *)Menu->Menu)->prev != NULL){
					Menu->Menu = ((struct Menu_enum_t *)Menu->Menu)->prev;
				}else{
					break;
				}
			}

			//parse list for current value
			for(i = 0; i < Menu->Items; i++){
				if(tmp_val == (((struct Menu_enum_t *)Menu->Menu)->Value)){
					break;
				}else{
					if(((struct Menu_enum_t *)Menu->Menu)->next != NULL){
						Menu->Menu = ((struct Menu_enum_t *)Menu->Menu)->next;
					}else{
						break;
					}
				}
			}

			break;
		case M_TIME:
			//get ini
			if(Menu->Filename != NULL){
				struct tm Def_time;
				cur_time_val = ini_getDateTime(Menu->Section,Menu->Key,Def_time,NULL, Menu->Filename);
				break;
			}
			//get var
			if(Menu->Variable != NULL){
				cur_time_val = *((struct tm *)Menu->Variable);
			}
			break;
		default:
			break;
		}
		return true;
	}

	bool SCR_save_setting(struct Menu_t * Menu){
		char tmp_section[75];
		switch(Menu->Type){
		case M_INT:
			//save to var
			if(Menu->Variable != NULL){
				memmove(Menu->Variable,&cur_int_val,Menu->Varsize);
			}

			//save to ini
			if(Menu->Filename != NULL){
				return ini_putl(Menu->Section, Menu->Key, cur_int_val, NULL, Menu->Filename);
			}
			return true;
			break;
		case M_ENUM:
			//save to var
			if(Menu->Variable != NULL){
				//*((int32_t *)Menu->Variable) = (((struct Menu_enum_t *)Menu->Menu)->Value);
				memmove(Menu->Variable,&(((struct Menu_enum_t *)Menu->Menu)->Value),Menu->Varsize);
			}

			//save to ini
			if(Menu->Filename != NULL){
				if((Menu->Settings != NULL)&&(Menu->Settings->list_nr)){
					sprintf(tmp_section,"%s%d",Menu->Section,Menu->Settings->list_nr);
					return ini_putl(tmp_section, Menu->Key, (((struct Menu_enum_t *)Menu->Menu)->Value), NULL, Menu->Filename);
				}else{
					return ini_putl(Menu->Section, Menu->Key, (((struct Menu_enum_t *)Menu->Menu)->Value), NULL, Menu->Filename);
				}
			}
			return true;
			break;
		case M_TIME:
			//save to var
			if(Menu->Variable != NULL){
				*((struct tm *)Menu->Variable) = cur_time_val;
			}
			//save to ini
			if(Menu->Filename != NULL){
				sprintf(tmp_section,"%02d:%02d",cur_time_val.tm_hour,cur_time_val.tm_min);
				return ini_puts(Menu->Section, Menu->Key,tmp_section, NULL, Menu->Filename);
			}
			return true;
			break;
		default:
			break;
		}
		return false;
	}
#endif
