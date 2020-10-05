#include "Terminal_functions.h"
#include <Power.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>

extern uint16_t g_Calibrate_counter;
extern uint32_t g_Calibrate_val;

void Device_terminal_functions(char * buffer){
	char * search;
	search = strstr(buffer,"RELAY ON");
	if(search != NULL){
		Relay_enable(atoi(search+8));
	}

	search = strstr(buffer,"RELAY OFF");
	if(search != NULL){
		Relay_disable(atoi(search+9));
	}

	search = strstr(buffer,"TEST");
	if(search != NULL){
		process_start(&MULTIpdtest_process,NULL);
	}


	/*search = strstr(buffer,"CALIBRATE");
	if(search != NULL){
		g_Calibrate_counter = 0;
		g_Calibrate_val = 0;
	}*/


	if((buffer[0] == 'Y')||(buffer[0] == 'N')){
		process_post(&MULTIpdtest_process,event_test_input,buffer);
	}
	return;
}


