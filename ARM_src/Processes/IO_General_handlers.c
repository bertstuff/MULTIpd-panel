#include <Board/Def_config.h>
#include <Core/Clock/clock.h>
#include <Core/protocol/Net/SCP/SCP_msg.h>
#include <processes/IO_General_handlers.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <Screen/Menu/SCR_Menu_func.h>

#ifdef I_CABINET_DOOR
void Cabinet_switch(bool state_change, struct IO_input_list_t * input){

	if(!state_change) return;
	if(input->init) return;
	if(input->state){
		printf("IO poll:deur dicht\r\n");
		BUFFER_SCP_MSG_DEURDICHT();
	}else{
		printf("IO poll:deur open\r\n");
		BUFFER_SCP_MSG_DEUROPEN();
	}
}
#endif

#ifdef O_CABINET_ALARM
void Cabinet_switch_alarm(bool state_change, struct IO_input_list_t * input){
	if(IO_get_output_state(O_CABINET_ALARM)){
		if(menu_get_authorization_level() > AUTH_NO){
			IO_set_output_state(O_CABINET_ALARM,false);
		}
	}

	if(!state_change) return;
	if(input->init) return;
	Cabinet_switch(state_change,input);
	if(input->state == false){
		if(IO_get_input_state(I_CABINET_KEY)==false){
			IO_set_output_state(O_CABINET_ALARM,true);
			BUFFER_SCP_MSG_ALERT__CABINET_UNAUTHORIZED_OPEND();
		}
	}else{
		IO_set_output_state_delayed(O_CABINET_ALARM,false,CLOCK_SECOND * 20);
	}
	return;
}
#endif

#ifdef I_CASHBOX_OPEN
void Cashbox_switch_alarm(bool state_change, struct IO_input_list_t * input){
	if(IO_get_output_state(O_CABINET_ALARM)){
		if(menu_get_authorization_level() > AUTH_NO){
			IO_set_output_state(O_CABINET_ALARM,false);
		}
	}

	if(!state_change) return;
	if(input->init) return;
	if(input->state == false){
		if(menu_get_authorization_level() > AUTH_NO){
			IO_set_output_state(O_CABINET_ALARM,false);
		}else{
			IO_set_output_state(O_CABINET_ALARM,true);
			BUFFER_SCP_MSG_ALERT__CASHBOX_UNAUTHORIZED_OPEND();
		}
	}else{
		IO_set_output_state_delayed(O_CABINET_ALARM,false,CLOCK_SECOND * 20);
	}
}
#endif
