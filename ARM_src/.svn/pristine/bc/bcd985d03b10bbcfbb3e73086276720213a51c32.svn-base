#include <ctype.h>
#include <Core/Extensions/ini/minIni.h>
#include <Core/Peripherals/UART_Dev_Init.h>
#include <Core/Timer/etimer.h>
#include <Device/I2C/SC16IS740/SC16IS740.h>
#include <LPC17xx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//////////////Global variables////////////
  UART_config_type UART_config[8] = {
		  {.New_data = NULL, .Auto_CTS = false, .Auto_RTS = false},
		  {.New_data = NULL, .Auto_CTS = false, .Auto_RTS = false},
		  {.New_data = NULL, .Auto_CTS = false, .Auto_RTS = false},
		  {.New_data = NULL, .Auto_CTS = false, .Auto_RTS = false},
		  {.New_data = NULL, .Auto_CTS = false, .Auto_RTS = false},
		  {.New_data = NULL, .Auto_CTS = false, .Auto_RTS = false},
		  {.New_data = NULL, .Auto_CTS = false, .Auto_RTS = false},
		  {.New_data = NULL, .Auto_CTS = false, .Auto_RTS = false}};
  
  
//////////////local prototypes////////////
 PROCESS(Uart_process, "Uart process");
 void UART_Dev_Init(LPC_UART_TypeDef *UARTx, UART_config_type Config);
 void UART_IntErr(uint8_t bLSErrType);
/////////////Interupt handler functions////////////
  
 PROCESS_THREAD(Uart_process, ev, data)
 {
	static uint8_t addr_counter = 0;
	uint8_t RX_level;
	char * RX_buffer;
	Status I2C_status;
	uint8_t i;
	static struct etimer poll_tm;

	if(etimer_expired(&poll_tm)){
		etimer_set(&poll_tm,1);
	}

 	PROCESS_BEGIN();
	while(ev != PROCESS_EVENT_EXIT){
		PROCESS_WAIT_EVENT();
		if(addr_counter > 3){
			addr_counter = 0;
		}
		if(UART_config[4 + addr_counter].New_data != NULL){
			I2C_status = SerReadReg(addr_counter, SerREG_RXLVL, &RX_level);
			if(I2C_status != SUCCESS){
				printf("Uart: i2c failed addr %d\r\n",addr_counter);
			}
			if(RX_level != 0){
					RX_buffer = malloc(RX_level);
					if(RX_buffer == NULL){
						RX_level = 0;
					}
					I2C_status = SerIn(addr_counter, RX_buffer, RX_level);
					if(I2C_status != SUCCESS){
						printf("Uart: i2c failed addr %d\r\n",addr_counter);
					}
			}else{
				addr_counter++;
				goto no_data_label;
			}

			for(i = 0; i < RX_level; i++){
				(*UART_config[4 + addr_counter].New_data)(RX_buffer[i]);
			}
			free(RX_buffer);
		}
		no_data_label:
		addr_counter++;
	}
 	PROCESS_END();
 }

void UART_Handler(uint8_t uartnr, LPC_UART_TypeDef * UARTx){
  uint32_t intsrc, tmp, tmp1;

  //Determine the interrupt source
  intsrc = UART_GetIntId(UARTx);
  tmp = intsrc & UART_IIR_INTID_MASK;
  
  // Receive Line Status
  if (tmp == UART_IIR_INTID_RLS){
    // Check line status
    tmp1 = UART_GetLineStatus(UARTx);
    // Mask out the Receive Ready and Transmit Holding empty status
    tmp1 &= (UART_LSR_OE | UART_LSR_PE | UART_LSR_FE \
    | UART_LSR_BI | UART_LSR_RXFE);
    // If any error exist
    if (tmp1) {
      UART_IntErr(tmp1);
    }
  }
  //Receive data
  if ((tmp == UART_IIR_INTID_RDA) || (tmp == UART_IIR_INTID_CTI)){
    (*UART_config[uartnr].New_data)(UART_ReceiveByte(UARTx));
  }
  return;
}
 
#if(!(DEBUG_UART & DEBUG_UART1))
void UART1_IRQHandler(void){
 UART_Handler(1,(LPC_UART_TypeDef *)LPC_UART1);
 return;
}
#endif

#if(!(DEBUG_UART & DEBUG_UART2))
void UART2_IRQHandler(void){
 UART_Handler(2,LPC_UART2);  
 return;
}
#endif

#if(!(DEBUG_UART & DEBUG_UART3))
void UART3_IRQHandler(void){
 UART_Handler(3,LPC_UART3);
 return;
}
#endif

void UART_IntErr(uint8_t bLSErrType)
{
  return;
}
/////////////Global functions////////////
Status UART_poort_Init(UART_select_t Uart,UART_config_type Config){
	Ser_UART_Config_t Cnfg;

	if(Uart < sizeof(UART_config)){
    	UART_config[Uart] = Config;
    }
	switch(Uart){
	case UART1_P0:
		UART1_P0_Dev_Init(Config);
		break;
	case UART1_P2:
		UART1_P2_Dev_Init(Config);
		break;
	case UART2_P2:
		UART2_P2_Dev_Init(Config);
		break;
	case UART3_P4:
		UART3_P4_Dev_Init(Config);
		break;
	case I2C_UART0:
	case I2C_UART1:
	case I2C_UART2:
	case I2C_UART3:
		process_start(&Uart_process,NULL);
		Cnfg.AutoCTS = Config.Auto_CTS;
		Cnfg.AutoRTS = Config.Auto_RTS;
		Cnfg.Baudrate = Config.settings.Baud_rate;
		Cnfg.NumBits = Config.settings.Databits + 5;
		Cnfg.Parity = Config.settings.Parity;
		Cnfg.RS485 = false;
		Cnfg.RTSActive = false;
		Cnfg.StopBit = 1;
		return SerInitUART(Uart & 0x03, Cnfg);
	default:
		break;
	}
	return SUCCESS;
}

uint32_t UART_send(UART_select_t Uart, uint8_t *data, uint8_t lenght){
	switch(Uart){
	case UART1_P0:
	case UART1_P2:
		return UART_Send((LPC_UART_TypeDef *)LPC_UART1, data, lenght, BLOCKING);
		break;
	case UART2_P2:
		return UART_Send(LPC_UART2, data, lenght, BLOCKING);
		break;
	case UART3_P4:
		return UART_Send(LPC_UART3, data, lenght, BLOCKING);
		break;
	case I2C_UART0:
	case I2C_UART1:
	case I2C_UART2:
	case I2C_UART3:
		if(SerOut(Uart & 0x03, (char *) data, lenght)==SUCCESS){
			return lenght;
		}else{
			return 0;
		}
		break;
	default:
		break;
	}
	return 0;
}

int32_t UART_receive(UART_select_t Uart, uint8_t *data,uint8_t  lenght){
	Status I2C_status;
	uint8_t RX_level = 0;

	switch(Uart){
	case UART1_P0:
	case UART1_P2:
		return UART_Receive((LPC_UART_TypeDef *)LPC_UART1, data, lenght, BLOCKING);
		break;
	case UART2_P2:
		return UART_Receive(LPC_UART2, data, lenght, BLOCKING);
		break;
	case UART3_P4:
		return UART_Receive(LPC_UART3, data, lenght, BLOCKING);
		break;
	case I2C_UART0:
	case I2C_UART1:
	case I2C_UART2:
	case I2C_UART3:
		I2C_status = SerReadReg(Uart & 0x03, SerREG_RXLVL, &RX_level);

		if(I2C_status != SUCCESS){
			printf("Uart: i2c failed addr %d\r\n",Uart & 0x03);
			return -1;
		}
		if(RX_level != 0){
			lenght = (RX_level > lenght)?lenght:RX_level;
			I2C_status = SerIn(Uart & 0x03, (char *)data, lenght);
			if(I2C_status != SUCCESS){
				return -1;
			}
			return lenght;
		}else{
			return 0;
		}
		break;
	default:
		break;
	}

	return -1;
}

void UART1_P0_Dev_Init(UART_config_type Config)
{  
    PINSEL_CFG_Type pinsel;  
  
    UART_Dev_Init((LPC_UART_TypeDef *)LPC_UART1, Config);
    //TX
    pinsel.Portnum = PINSEL_PORT_0;
    pinsel.Pinnum = PINSEL_PIN_15;
    pinsel.Funcnum = PINSEL_FUNC_1;
    pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;
    pinsel.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
    PINSEL_ConfigPin(&pinsel);

    //RX
    pinsel.Pinnum = PINSEL_PIN_16;
    PINSEL_ConfigPin(&pinsel);
    
    //Modem
    if(Config.Auto_RTS){
    	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_RTS, ENABLE);
    }

    if(Config.Auto_CTS){
    	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_CTS, ENABLE);
    }

    //Interrupt enable
    if(Config.New_data != NULL){
    	NVIC_EnableIRQ(UART1_IRQn);
    }
    return;
}

void UART1_P2_Dev_Init(UART_config_type Config)
{  
    PINSEL_CFG_Type pinsel;  

    UART_Dev_Init((LPC_UART_TypeDef *)LPC_UART1, Config);
    //TX
    pinsel.Portnum = PINSEL_PORT_2;
    pinsel.Pinnum = PINSEL_PIN_0;
    pinsel.Funcnum = PINSEL_FUNC_2;
    pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;
    pinsel.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
    PINSEL_ConfigPin(&pinsel);

    //RX
    pinsel.Pinnum = PINSEL_PIN_1;
    PINSEL_ConfigPin(&pinsel);
    
    //Modem
    if(Config.Auto_RTS){
    	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_RTS, ENABLE);
    }

    if(Config.Auto_CTS){
    	UART_FullModemConfigMode(LPC_UART1, UART1_MODEM_MODE_AUTO_CTS, ENABLE);
    }

    //Interrupt enable
    NVIC_EnableIRQ(UART1_IRQn);  
    return;
}

void UART2_P2_Dev_Init(UART_config_type Config)
{       
    
    PINSEL_CFG_Type pinsel;
    
    UART_Dev_Init(LPC_UART2, Config);
    //TX
    pinsel.Portnum = PINSEL_PORT_2;
    pinsel.Pinnum = PINSEL_PIN_8;
    pinsel.Funcnum = PINSEL_FUNC_2;
    pinsel.Pinmode = PINSEL_PINMODE_TRISTATE;
    pinsel.OpenDrain = PINSEL_PINMODE_OPENDRAIN;
    PINSEL_ConfigPin(&pinsel);

    //RX
    pinsel.Pinnum = PINSEL_PIN_9;
    PINSEL_ConfigPin(&pinsel);
    
    if(Config.Auto_CTS||Config.Auto_RTS){
    	printf("Uart: don't support Modem control\r\n");
    }

    //Interrupt enable
    NVIC_EnableIRQ(UART2_IRQn);    
    return;
}

void UART3_P4_Dev_Init(UART_config_type Config)
{       
    
    PINSEL_CFG_Type pinsel;
    
    UART_Dev_Init(LPC_UART3, Config);
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
    
    if(Config.Auto_CTS||Config.Auto_RTS){
    	printf("Uart: don't support Modem control\r\n");
    }

    //Interrupt enable
    NVIC_EnableIRQ(UART3_IRQn);
        
    return;
}

void UART1_RS485_P0_Dev_Init(UART_config_type Config, uint8_t address){
  UART1_RS485_CTRLCFG_Type RS485_config;
  
  UART1_P0_Dev_Init(Config);
  
  RS485_config.NormalMultiDropMode_State = ENABLE; /*!< Normal MultiDrop mode State*/													
	RS485_config.Rx_State = ENABLE;		/*!< Receiver State*/
	RS485_config.AutoAddrDetect_State = ENABLE;		/*!< Auto Address Detect mode state*/
	RS485_config.AutoDirCtrl_State = ENABLE;			/*!< Auto Direction Control State*/
	RS485_config.DirCtrlPin = UART1_RS485_DIRCTRL_RTS;		/*!< If direction control is enabled*/												
	RS485_config.DirCtrlPol_Level = 1;					/*!< Polarity of the direction control signal*/
	RS485_config.MatchAddrValue = address;					/*!< address match value for RS-485/EIA-485 mode, 8-bit long */
	RS485_config.DelayValue = 5;						/*!< delay time is in periods of the baud clock, 8-bit long */
  
  UART_RS485Config(LPC_UART1, &RS485_config);
  return;
}

UART_select_t ini_getuart(const TCHAR *Section, const TCHAR *Key, const TCHAR *DefValue, const TCHAR *Description, const TCHAR *Filename){
	char str[50];
	char * strb = str;
	char * srcp;

	ini_gets(Section ,Key , DefValue, Description, str, sizeof(str), Filename);
	//skip space
	while(*strb == ' '){
		strb++;
	}

	if(isdigit(*strb)){
		return str[0]-'0';
	}else{
		if(strstr(strb,"Basis_EBM_COM1") != NULL){
			return Basis_EBM_COM1;
		}

		if(strstr(strb,"Basis_EBM_COM2") != NULL){
			return Basis_EBM_COM2;
		}

		if(strstr(strb,"Basis_EBM_COM3") != NULL){
			return Basis_EBM_COM3;
		}

		srcp = strstr(strb,"I2C_UART");
		if(srcp != NULL){
			return I2C_UART0 + (srcp[8]-'0');
		}

	}
	printf("Uart: Com [%s] not found",str);
	return 0;
}

//////////////////////////////local functions////////////////////////////////////
 void UART_Dev_Init(LPC_UART_TypeDef *UARTx, UART_config_type Config)
 {
    UART_FIFO_CFG_Type F_CFG;
    
    //Set Baud

    UART_Init(UARTx, &Config.settings);
    
    //FIFO
    UART_FIFOConfigStructInit(&F_CFG);
    UART_FIFOConfig(UARTx, &F_CFG);
    
    //Interrupt enable
    if(Config.New_data != NULL){
		UART_IntConfig(UARTx, UART_INTCFG_RBR, ENABLE);
		UART_IntConfig(UARTx, UART_INTCFG_RLS, ENABLE);
    }
	UART_TxCmd(UARTx,ENABLE);
    return;
}
