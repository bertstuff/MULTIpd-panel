#include <Board/Def_config.h>
#if(MENU_MODULE == 1)

#ifndef __SCR_MENU_HANDLER_H__
#define __SCR_MENU_HANDLER_H__

 #include <stdio.h>
 #include <stdint.h>
 #include <stdbool.h>
 #include <Core/Process/process.h>
 #include <Core/Thread/pt.h>
 #include <Global.h>
 #include <stdarg.h>
 #include <Screen/SCR_Text.h>
 #include <Core/Memory/cmem.h>

 /*------------Types-------------*/
 typedef enum{
 	H_MENU_INIT,
 	H_MENU_VAL_CHANGE,
 	H_MENU_CANCEL,
 	H_MENU_ACCEPT
 }menu_handler_t;

 typedef enum{
 	M_MENU,
 	M_INT,
 	M_ENUM,
 	M_TIME,
 	M_AUTH,
 	M_TEXT,
 	M_CUSTOM,
 	M_MESSAGE,
 	M_QUESTION
 }menu_type_t;

#define AUTH_NO 0
#define AUTH_ADMIN 1
#define AUTH_SERVICE 2
#define AUTH_MANUFACTURE 4
#define AUTH_DEBUG 8

 struct menu_settings_t{
	void * prev_mem; 							//pointer for freeing menu
 	void (*Handler)(menu_handler_t event);		//Custom Handler
 	uint8_t auth_lev;					//Authorization level of menu
 	F_TEXT(exit_message);				//show exit message
 	uint8_t list_nr;					//Numbered menu item
 }menu_settings_t;

 struct Menu_t{
	void * prev_mem;							//pointer for freeing menu
	F_TEXT(Name); //Name of menu
 	menu_type_t Type;							//Menu type
 	uint8_t Items;								//Number of items in this menu
 	void * Menu;								//current selected menu
 	struct menu_settings_t * Settings;			//menu settings
 	struct Menu_t * prev_menu;					//pointer to previous menu

 	void * Variable;
 	uint8_t Varsize;
 	const char *Section;
 	const char *Key;
 	const char *Filename;
 }Menu_t;

 /*------------Menu types------------*/
 struct Menu_item_t{
	void * prev_mem;
 	uint8_t Menu_nr;
 	F_TEXT(Name);
 	struct Menu_item_t * next;
 	struct Menu_item_t * prev;
 	struct Menu_t * next_menu;
 	struct Menu_t * Menu;
 }Menu_item_t;

 struct Menu_int_t{
	void * prev_mem;
	F_TEXT(type);
 	int32_t min;
 	int32_t max;
 	uint8_t decimals;
 }Menu_int_t;

 struct Menu_enum_t{
	void * prev_mem;
	F_TEXT(Name);
 	struct Menu_enum_t * next;
 	struct Menu_enum_t * prev;
 	int32_t Value;
 }Menu_enum_t;

 struct Menu_time_t{
	void * prev_mem;
 }Menu_time_t;

 struct Menu_auth_t{
	 void * prev_mem;
	 struct Menu_t * next_menu;
 }Menu_auth_t;

 struct Menu_text_t{
	void * prev_mem;
	F_TEXT(text);
 	char * (*func)(char * buffer);
 	struct Menu_text_t * next;
 	struct Menu_text_t * prev;
 }Menu_text_t;

 struct Menu_custom_t{
	void * prev_mem;
	PT_THREAD((* Menu_handler)(process_event_t ev, process_data_t data));
	void (*Menu_draw)(bool enter_menu);
 }Menu_custom_t;

 struct Menu_message_t{
	 void * prev_mem;
	 struct Menu_t * next_menu;
 }Menu_message_t;

 struct Menu_question_t{
	void * prev_mem;
	F_TEXT(question);
 }Menu_bool_t;

 struct Menu_memory_t{
	 void * prev_mem;
 }Menu_memory_t;

 /*------------Global var------------*/
 extern struct pt menu_pt;
 extern uint8_t cur_edit_pos;
 extern struct Menu_t * cur_menu;

 /*------------local prototypes------------*/
 bool SCR_check_authorization_level(void);
 void SCR_enter_menu(void);
 void SCR_leave_menu(menu_handler_t event);

 /*------------global prototypes------------*/
 PROCESS_NAME(SCR_Menu_process);


#endif //__SCR_MENU_HANDLER_H__
#endif
