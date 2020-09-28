#include "Debug_Terminal.h"
#include <Board/Def_config.h>
#include <Driver/include/lpc17xx_uart.h>
#include <Driver/include/lpc17xx_pinsel.h>
#include <stdbool.h>
#include <string.h>
#include <Core/Memory/IAP.h>
#include <Core/Util/build_defs.h>
#include <Device/I2C/LV3095/LV3095.h>
#include <Core/Memory/ringbuf.h>
#include <Core/Clock/Time_functions.h>
#include <Core/Clock/rtc_cal.h>
#include "Print_device_info.h"
#include <cr_section_macros.h>
#include <Core/Memory/cmem.h>
#include <Processes/dev_reset.h>
#define DEBUG_KEYPAD

#ifdef DEBUG_KEYPAD
	#include <Device/I2C/BUTTONS/Buttons_3x4.h>
#endif

__BSS(RAM2) char input_buffer[32];
struct ringbuf debug_buffer;
uint8_t pay_event;
int32_t terminal_event_data;

void Device_terminal_functions(char * buffer) __attribute__((weak));

void Serial_Terminal_send(char * data,int lenght){

#if(DEBUG_UART & DEBUG_UART0)
	UART_Send((LPC_UART_TypeDef * )LPC_UART0 ,(uint8_t *)data ,lenght , BLOCKING);
#endif
#if(DEBUG_UART & DEBUG_UART3)
	UART_Send((LPC_UART_TypeDef * )LPC_UART3 ,(uint8_t *)data ,lenght , BLOCKING);
#endif

	return;
}

void serial_debug_input(char byte){
	uint8_t i;
	char * search;

	ringbuf_put(&debug_buffer, byte);
	if(byte == '\n'){
		char buffer[32];
		i = 0;
		while(1){
			uint8_t buffer_byte	 = ringbuf_get(&debug_buffer);
			if(buffer_byte == '\n'){
				buffer[i] = '\0';
				break;
			}else{
				buffer[i] = buffer_byte;
			}
			i++;
			if(i == sizeof(buffer)){
				return;
			}
		}

		search = strstr(buffer,"SET TIME");
		if(search != NULL){
			struct tm time;
			time = string_to_time(search+8);
			printf ( "Debug: The current date/time is: %s\r\n", asctime (LocalTime(&time)) );
			rtc_cal_init(&time);
			return;
		}
		search = strstr(buffer,"GET TIME");
		if(search != NULL){
			struct tm time;
			rtc_cal_gettime(&time);
			printf ( "Debug: The current date/time is: %s\r\n", asctime (LocalTime(&time)) );
			return;
		}

		search = strstr(buffer,"RESET ARM");
		if(search != NULL){
			Hard_reset();
		}

		search = strstr(buffer,"SHOW MEM");
		if(search != NULL){
			cmem_print();
		}
		
		if(Device_terminal_functions != NULL){
			Device_terminal_functions(buffer);
		}
	}

	return;
}
#if(DEBUG_UART & DEBUG_UART3)
void UART3_IRQHandler(void){
	  uint32_t intsrc, tmp, tmp1;

	  //Determine the interrupt source

	  intsrc = UART_GetIntId(LPC_UART3);
	  tmp = intsrc & UART_IIR_INTID_MASK;

	  // Receive Line Status
	  if (tmp == UART_IIR_INTID_RLS){
	    // Check line status
		tmp1 = UART_GetLineStatus(LPC_UART3);
	    // Mask out the Receive Ready and Transmit Holding empty status
	    tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
	    | UART_LSR_BI | UART_LSR_RXFE);
	    // If any error exist
	    if (tmp1) {
	      //UART_IntErr(tmp1);
	    }
	  }
	  //Receive data
	  if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
		  serial_debug_input(UART_ReceiveByte(LPC_UART3));
	  }
	  return;
}
#endif
#if(DEBUG_UART & DEBUG_UART0)
void UART0_IRQHandler(void){
	  uint32_t intsrc, tmp, tmp1;

	  //Determine the interrupt source
	  intsrc = UART_GetIntId(LPC_UART0);
	  tmp = intsrc & UART_IIR_INTID_MASK;

	  // Receive Line Status
	  if (tmp == UART_IIR_INTID_RLS){
	    // Check line status

		tmp1 = UART_GetLineStatus(LPC_UART0);
	    // Mask out the Receive Ready and Transmit Holding empty status
	    tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
	    | UART_LSR_BI | UART_LSR_RXFE);
	    // If any error exist
	    if (tmp1) {
	      //UART_IntErr(tmp1);
	    }
	  }
	  //Receive data
	  if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
		  serial_debug_input(UART_ReceiveByte(LPC_UART0));
	  }
	  return;
}
#endif

void Serial_Terminal_init(void)
{  
  PINSEL_CFG_Type pinsel;
  UART_CFG_Type UART_CFG;
  UART_FIFO_CFG_Type F_CFG;

  pay_event = process_alloc_event(); //Claim a event number

#if(DEBUG_UART & DEBUG_UART0)
  //TX
  pinsel.Portnum = PINSEL_PORT_0;
  pinsel.Pinnum = PINSEL_PIN_2;
  pinsel.Funcnum = PINSEL_FUNC_1;
  pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;
  pinsel.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
  PINSEL_ConfigPin(&pinsel);

  //RX
  pinsel.Pinnum = PINSEL_PIN_3;
  PINSEL_ConfigPin(&pinsel);

  //Set Baud
  UART_ConfigStructInit(&UART_CFG);
  UART_CFG.Baud_rate = 115200;
  UART_Init((LPC_UART_TypeDef * )LPC_UART0, &UART_CFG);

  //FIFO
  UART_FIFOConfigStructInit(&F_CFG);
  UART_FIFOConfig(LPC_UART0, &F_CFG);

  //Interrupt enable
  UART_IntConfig(LPC_UART0, UART_INTCFG_RLS, ENABLE);
  UART_IntConfig(LPC_UART0, UART_INTCFG_RBR, ENABLE);
  UART_TxCmd(LPC_UART0,ENABLE);

  //Interrupt enable
  NVIC_EnableIRQ(UART0_IRQn);
  ringbuf_init(&debug_buffer, (uint8_t *)input_buffer,32);
#endif

#if(DEBUG_UART & DEBUG_UART3)
	#if(DEBUG_UART3_PORT == 0)
  //TX
  pinsel.Portnum = PINSEL_PORT_0;
  pinsel.Pinnum = PINSEL_PIN_0;
  pinsel.Funcnum = PINSEL_FUNC_2;
  pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;
  pinsel.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
  PINSEL_ConfigPin(&pinsel);

  //RX
  pinsel.Pinnum = PINSEL_PIN_1;
  PINSEL_ConfigPin(&pinsel);
	#endif
	#if(DEBUG_UART3_PORT == 4)
  //TX
  pinsel.Portnum = PINSEL_PORT_4;
  pinsel.Pinnum = PINSEL_PIN_28;
  pinsel.Funcnum = PINSEL_FUNC_3;
  pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;
  pinsel.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
  PINSEL_ConfigPin(&pinsel);

  //RX
  pinsel.Pinnum = PINSEL_PIN_29;
  PINSEL_ConfigPin(&pinsel);
	#endif


  //Set Baud
  UART_ConfigStructInit(&UART_CFG);	
  UART_CFG.Baud_rate = 115200;
  UART_Init(LPC_UART3, &UART_CFG);

  //FIFO
  UART_FIFOConfigStructInit(&F_CFG);
  UART_FIFOConfig(LPC_UART3, &F_CFG);

  //todo:fix this
  //Interrupt enable
  UART_IntConfig(LPC_UART3, UART_INTCFG_RLS, ENABLE);
  UART_IntConfig(LPC_UART3, UART_INTCFG_RBR, ENABLE);
  UART_TxCmd(LPC_UART3,ENABLE);

  //Interrupt enable
  NVIC_EnableIRQ(UART3_IRQn);
  ringbuf_init(&debug_buffer, (uint8_t *)input_buffer,32);
#endif
  Serial_Print_Device_Info();
  return;
}


