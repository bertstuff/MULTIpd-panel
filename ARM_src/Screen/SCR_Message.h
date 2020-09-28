#include <Board/Def_config.h>
#if(BOARD_SCREEN == 1)
#ifndef __SCR_MESSAGE_H__
#define __SCR_MESSAGE_H__

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <Core/Thread/pt.h>
/* global variable*/
extern struct pt message_pt;
extern bool g_connecting;

/* local functions*/
extern PT_THREAD(SCR_Message_screen(char * format,...));

/* global API functions*/
/*
 * @brief present message for a few seconds
 * @param pt thread pointer
 * @param format string to present
 */
#define PT_MESSAGE_SCREEN(pt,format, ...) PT_SPAWN(pt, &message_pt,SCR_Message_screen(format, ## __VA_ARGS__))
#define PROCESS_PT_MESSAGE_SCREEN(format, ...) PROCESS_PT_SPAWN(&message_pt,SCR_Message_screen(format, ## __VA_ARGS__));


/*
 * @brief present please wait screen
 * @param state enable or disable wait screen
 */
void Please_wait_screen(bool state);


/*
 * @brief show a busy bar. call this function every second
 */
void SCR_bussy_bar(void);

#endif
#endif
