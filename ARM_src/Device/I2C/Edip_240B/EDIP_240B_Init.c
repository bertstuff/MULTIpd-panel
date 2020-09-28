#include "EDIP_240B_Init.h"
#include "EDIP_240B_Send.h"
#include "EDIP_240B_Touch.h"
#include "EDIP_240B_Display.h"
#include "EDIP_240B_IO.h"
#include <Core/Peripherals/I2C_Dev_Init.h>
#include <Board/Def_config.h>

///////////////Prototypes//////////////////////////////////////
PROCESS(EDIP_process, "Process for EDIP screen");
void Edip_init_LCD(void);
void Edip_Select_LCD(uint8_t Address);

///////////////Global variables/////////////////////////////////////
bool Edip_Init;
static struct etimer Edip_poll_tm;
static struct timer Edip_Startup_tm;

///////////////Global functions/////////////////////////////////////

PROCESS_THREAD(EDIP_process, ev, data)
{
  if(etimer_expired(&Edip_poll_tm)) {
    etimer_set(&Edip_poll_tm, CLOCK_SECOND/5);
  }
  PROCESS_BEGIN();
  Edip_Init = false;
  timer_set(&Edip_Startup_tm, CLOCK_SECOND*2);
  PROCESS_WAIT_EVENT_UNTIL(timer_expired(&Edip_Startup_tm));
  Edip_init_LCD();
  while (ev != PROCESS_EVENT_EXIT){ //loop till exit
    PROCESS_YIELD();
#ifdef TOUCH_MODULE
    Edip_Receive();
#endif
    EDIP_IO_interval_handler();
  }
  PROCESS_END();
}

void Edip_init_LCD(void){  
  I2C_Dev_Init(EDIP_MAX_SPEED_HZ);
  Edip_Refresh = true;
  Edip_Select_LCD(0);
  Edip_Touch_Init();
  Edip_Init = true;
  Edip_Set_Timout(20);
  Edip_Display_on();
  Edip_io_init();
}

void Edip_Select_LCD(uint8_t Address){
  if(Address < 10){
      Address |= 0xB0 ;
  }
  Edip_address = Address;
  Edip_Refresh = true;
}
