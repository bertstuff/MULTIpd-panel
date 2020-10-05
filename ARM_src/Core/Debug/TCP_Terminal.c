#include "Debug_Terminal.h"
#include <Board/Def_config.h>
#include <Core/Process/process.h>
#include <stdbool.h>
#include <string.h>
#include <Core/protocol/Net/uip.h>
#include <Core/protocol/Net/psock.h>

#if (TCP_TERMINAL_ENABLE == 1)
static struct psock ps_debug;
struct uip_conn *Terminal_conn = NULL;

uint8_t Terminal_buffer[100];

bool g_Terminal_new_data;
PROCESS(TCP_Terminal_process, "Debug terminal");

void TCP_Terminal_init(void){
    process_start(&TCP_Terminal_process,NULL);
}


void TCP_Terminal_send(char * data,int lenght){
	if(Terminal_conn == NULL){return;}
	if((strlen((char *)Terminal_buffer)+lenght) > (sizeof(Terminal_buffer)-3)){
		if((strlen((char *)Terminal_buffer) + 3) > sizeof(Terminal_buffer)){
			strcat((char*)Terminal_buffer,"!!");
		}
		return;
	}
	strcat((char*)Terminal_buffer,data);
	g_Terminal_new_data = true;
	return;
}

static PT_THREAD(handle_Terminal_connection(struct psock *p)){
	PSOCK_BEGIN(p);
	PSOCK_SEND_STR(p,"Enter password:\r\n");
	PSOCK_READTO(p,'\n');
	if(strstr((char *)Terminal_buffer,"Navmate1") != NULL){
		PSOCK_SEND_STR(p,"Password ok! start Terminal.\r\n");

		while(1){
			PSOCK_WAIT_UNTIL(p,g_Terminal_new_data == true);
			PSOCK_SEND_STR(p,(char *)Terminal_buffer);
			Terminal_buffer[0]='\0';
			g_Terminal_new_data = false;
		}
	}
	PSOCK_SEND_STR(p,"Wrong password\r\n");
	PSOCK_END(p);
}

PROCESS_THREAD(TCP_Terminal_process, ev, data)
{
  PROCESS_BEGIN();

  tcp_listen(UIP_HTONS(2000));

  printf("Debug: Terminal started\r\n");
  while (ev != PROCESS_EVENT_EXIT){ //loop till exit
	  PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
	  if(uip_connected()) {
		  Terminal_conn = uip_conn;
		  PSOCK_INIT(&ps_debug, Terminal_buffer, sizeof(Terminal_buffer));
		  do{
			  handle_Terminal_connection(&ps_debug);
			  PROCESS_WAIT_EVENT_UNTIL(ev == tcpip_event);
		  } while(!(uip_closed() || uip_aborted() || uip_timedout()));
	  }
	  Terminal_conn = NULL;
  }
  PROCESS_END();
}
#endif





