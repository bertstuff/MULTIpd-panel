
#include "RFID.h"
#include "RFID_Pass.h"
#include <Device/Devices.h>
#include <core/Extensions/ini/minIni.h>
#include <core/Timer/etimer.h>
#include <Core/Debug/Error_check.h>
#include <Core/Peripherals/I2C_Dev_Init.h>
#include <Device/I2C/SC16IS740/SC16IS740.h>
#include <stdio.h>

#ifndef RFID_MAX_READERS
	#define RFID_MAX_READERS 2
#endif


//Mifare/hitag status flag defines
#define EEPROM_ERROR 1<<0 //EEPROM error (Internal EEPROM write error)
#define CARD_OK 1<<1 //Card OK (Card serial number matched to identity code list)
#define RX_OK 1<<2 //Rx OK (Card communication and acknowledgement OK)
#define RS232_ERROR 1<<3 //RS232 error (Host serial communication error)
#define MF_TYPE 1<<4 //Mifare type (0 = MF 1k byte card, 1 = MF 4k byte card)
#define UL_TYPE 1<<5 //Ultralight card  type (0 = MF standard 1k/4k card, SINGLE UID), 1 = MF Ultralight card, DOUBLE UID)
#define MFRC_ERROR 1<<6 //MFRC error (Mifare Internal or antenna fault)

//Hitag status flag defines
#define HTRC_ERROR 1<<5 //HTRC error (Hitag Internal or antenna fault)

//Mifare commands
#define RFID_ProgEprom 0x50	//0x50 (P) Programmeer Eprom register in RWD.
#define RFID_KeyStore 0x4B	//0x4B (K) Sla sleutel opslaan in RWD
#define RFID_WriteData 0x57	//0x57 (W) Write Data op kaart
#define RFID_ReadData 0x52	//0x52 (R) Read Data op kaart
#define RFID_IncValue 0x49	//0x49 (I) Increment data op kaart
#define RFID_Transfer 0x54	//0x54 (T) Transfer Data (copy)
#define RFID_Status 0x53	//0x53 (S) Status alleen opvragen
#define RFID_GET_UID 0x55		//0x55 (U) Unique IDentifier nummer opvragen
#define RFID_TypeID 0x78	//0x78 (x) Kaart Type ID nummer
#define RFID_OutPut 0x4F	//0x4F (O) Output control byte.
#define RFID_ReadRXData 0x10	//0x10 lees data uit van kaart
#define RFID_ReadTagBlock 0x72 //
#define RFID_WriteTagBlock 0x77 //
#define RFID_ReaderType 0x76	//0x76 (v) set reader type

//Mifare output pin
#define	F_RED_LED 	(1 << 0)
#define F_GREEN_LED (1 << 1)
#define F_BUZZER 	(1 << 2)

#define RFID_KEYS 1

process_event_t event_card_change;
process_event_t event_card_done;
process_event_t event_card_busy;

RFID_reader_error_t g_RFID_error = RFID_I2C_ERROR;

RFID_reader_t RFID_Reader[RFID_MAX_READERS];
uint8_t RFID_reader_cnt = 0;
uint8_t RFID_cur_reader = 0;

struct pt pt_RFID_send;
struct pt pt_RFID_send_data;
struct pt pt_RFID_sub;
struct pt pt_RFID_wait;

RFID_card_t * g_RFID_card;

const char RFID_keys[RFID_KEYS][6]={{0x22,0x31,0x56,0x39,0xed,0xc5}};

//prototype
PT_THREAD(RFID_hitag_init(void));
PT_THREAD(RFID_send_command(RFID_reader_t * reader, Status * Com_status, char command, char * Tx_data, uint8_t Tx_length, uint8_t RX_lenght));
PT_THREAD(RFID_get_uid(RFID_reader_t * reader));
PT_THREAD(RFID_led_buzzer_state(RFID_reader_t * reader, bool busy));
PT_THREAD(RFID_wait_for_data(RFID_reader_t * reader, Status * Com_status, uint8_t RX_lenght));
void RFID_error(RFID_reader_t * reader, RFID_reader_error_t reader_error);
char Higtag_type_byte(RFID_card_type_t cardtype);
uint8_t RFID_data_availible(RFID_reader_t * reader);
Status RFID_parse_flag_com_ok(RFID_reader_t * reader);

PROCESS(RFID_process, "Process for RFID card reader");

//global functions
RFID_reader_t * RFID_init(uint8_t Reader_addr, RFID_reader_type_t Reader_type){
	Status I2C_status;
	Ser_UART_Config_t Config;

	g_Devices_available.Multipass_available = ini_getbool("Device","Multipass_available",false,"Multipass available:",inifile_device);
	if(g_Devices_available.Multipass_available == true){
		g_Devices_available.Multipass_online = ini_getbool("Multipass","Multipass_Online",true,"Multipass online:",inifile_device);
		g_Devices_available.Multipass_offline = ini_getbool("Multipass","Multipass_Offline",false,"Multipass offline:",inifile_device);
	}else{
		return NULL;
	}
	if(g_Devices_available.Multipass_offline == true){
		g_RFID_card = RFID_get_card(RFID_get_reader(0x00));
		g_RFID_card->card_action.Credit_read = true;
		g_RFID_card->card_action.Info_read = true;
		g_RFID_card->card_action.Unlock_code_read = true;
	}

	if(RFID_reader_cnt == 0){
	   I2C_Dev_Init(RFID_MAX_SPEED_HZ);
	   event_card_busy = process_alloc_event(); //Claim a event number
	   event_card_change = process_alloc_event(); //Claim a event number
	   event_card_done = process_alloc_event(); //Claim a event number
	   error_check_add(TYPE_ENUM, &g_RFID_error, ERROR_RFID);
    }

    RFID_Reader[RFID_reader_cnt].reader_addr = Reader_addr;
    RFID_Reader[RFID_reader_cnt].reader_type = Reader_type;

    Config.Baudrate = 9600;
    Config.NumBits = 8;
    Config.StopBit = 1;
    Config.Parity = PAR_None;
    Config.AutoCTS = true;
    Config.AutoRTS = false;
    Config.RTSActive = false;

    I2C_status = SerInitUART(Reader_addr, Config);

    if(I2C_status != SUCCESS)
    {
    	RFID_error(&RFID_Reader[RFID_reader_cnt],RFID_I2C_ERROR);
    }

    process_start(&RFID_process,NULL);
    RFID_reader_cnt++;
    return &RFID_Reader[RFID_reader_cnt -1];
}

RFID_reader_t * RFID_get_reader(uint8_t RFID_addr){
	uint8_t i = 0;

	for(i = 0; i < RFID_MAX_READERS; i++){
		if(RFID_Reader[i].reader_addr == RFID_addr){
			return &RFID_Reader[i];
		}
	}
	return NULL;
}

void RFID_beep_sound(void){
	RFID_Reader[0].reader_beep_sound = true;
	return;
}

PROCESS_THREAD(RFID_process, ev, data)
{
	static struct etimer poll_timer;
	static RFID_reader_t * reader;

	if(etimer_expired(&poll_timer)) {
		etimer_set(&poll_timer, CLOCK_SECOND);
	}

	PROCESS_BEGIN();
	PROCESS_YIELD();
	//PROCESS_PT_SPAWN(&pt_RFID_sub,RFID_keys_init());
	PROCESS_PT_SPAWN(&pt_RFID_sub,RFID_hitag_init());
	RFID_cur_reader = 0;
	while (ev != PROCESS_EVENT_EXIT){ //main loop  till exit
		RFID_cur_reader++;
	   if(RFID_cur_reader >= RFID_reader_cnt){
		   RFID_cur_reader = 0;
	   }
	   reader = &RFID_Reader[RFID_cur_reader];

	   //read uid from pass
	   PROCESS_PT_SPAWN(&pt_RFID_sub,RFID_led_buzzer_state(reader,false));
	   PROCESS_PT_SPAWN(&pt_RFID_sub,RFID_get_uid(reader));

	   if( reader->reader_card.card_present == false){
		   etimer_set(&poll_timer, CLOCK_SECOND);
		   PROCESS_YIELD();
		   //Is the card really gone?
		   PROCESS_PT_SPAWN(&pt_RFID_sub,RFID_get_uid(reader));
		   if( reader->reader_card.card_present == true){
			   continue;
		   }
		   if(reader->reader_card.card_changed == true){
			   reader->reader_card.card_changed = false;
			   reader->reader_card.card_credit.availible = false;
			   reader->reader_card.card_info.availible = false;
			   reader->reader_card.card_info.Language = NOT_SET;
			   reader->reader_card.card_unlock_code.availible = false;
			   reader->reader_card.card_unlock_code.ok = false;
			   process_post(PROCESS_BROADCAST,event_card_change,(void *)reader);
			   printf("RFID %d: Pass removed \r\n",reader->reader_addr);
		   }
		   continue; //No card jump to begin
	   }

	   if(reader->reader_card.card_changed == true){
		   reader->reader_card.card_changed = false;
		   PROCESS_PT_SPAWN(&pt_RFID_sub,RFID_led_buzzer_state(reader,true));
		   process_post(PROCESS_BROADCAST,event_card_busy,(void *)reader);

		   //read user info
		   if(reader->reader_card.card_action.Info_read){
			  PROCESS_PT_RFID_Get_Info(reader);
		   }

		   //check valid card
		   if(reader->reader_card.card_action.Unlock_code_read){
			   PROCESS_PT_RFID_Get_Unlock_code(reader);
		   }

		   //read credits form pass
		   if(reader->reader_card.card_action.Credit_read){
			   PROCESS_PT_RFID_Get_Credit(reader);
		   }

#if(BOARD_SCREEN == 1)
	   	   PROCESS_WAIT_UNTIL(SCR_is_busy() == false);
#endif

	   	   process_post(PROCESS_BROADCAST,event_card_change,(void *)reader);
	   	   PROCESS_WAIT_UNTIL(process_event_in_list(event_card_change)==false);

		   reader->reader_beep_sound = true;
		   printf("RFID %d: %sPass present [UID: %02x ",reader->reader_addr,((reader->reader_card.card_new == true)?"New ":""),reader->reader_card.card_UID.UID[0]);
		   printf("%02x ",reader->reader_card.card_UID.UID[1]);
		   printf("%02x ",reader->reader_card.card_UID.UID[2]);
		   printf("%02x ",reader->reader_card.card_UID.UID[3]);
		   printf("%02x ",reader->reader_card.card_UID.UID[4]);
		   printf("%02x ",reader->reader_card.card_UID.UID[5]);
		   printf("%02x] \r\n",reader->reader_card.card_UID.UID[6]);
	   }

	   //write user info
	   if(reader->reader_card.card_action.Info_write){
		   PROCESS_PT_RFID_Set_Info(reader);
		   process_post(PROCESS_BROADCAST,event_card_done,reader);
	   }

	   //write unlock code on card
	   if(reader->reader_card.card_action.Unlock_code_write){
		   PROCESS_PT_RFID_Set_Unlock_code(reader);
		   process_post(PROCESS_BROADCAST,event_card_done,reader);
	   }

	   //write credits on card
	   if(reader->reader_card.card_action.Credit_write){
		   PROCESS_PT_RFID_Set_Credit(reader);
		   process_post(PROCESS_BROADCAST,event_card_done,reader);
	   }
	   PROCESS_WAIT_UNTIL(process_event_in_list(event_card_done)==false);
	   PROCESS_PT_SPAWN(&pt_RFID_sub,RFID_led_buzzer_state(reader,false));
	}
	PROCESS_END();
}

///////////RFID Sub routines///////////

PT_THREAD(RFID_keys_init(void)){
	char data_byte[7];

	PT_BEGIN(&pt_RFID_sub) ;

	for(RFID_cur_reader= 0; RFID_cur_reader < RFID_reader_cnt; RFID_cur_reader++){
	   if(RFID_Reader[RFID_cur_reader].reader_type == READER_MIFARE){
		   data_byte[0] = 0x04; //Encryption off
		   memmove(&data_byte[1], RFID_keys[0], 6);
		   PT_SPAWN(&pt_RFID_sub, &pt_RFID_send, RFID_send_command(&RFID_Reader[RFID_cur_reader], &RFID_Reader[RFID_cur_reader].reader_com.com_status, RFID_KeyStore, data_byte, 7, 0));
	   }
   }

   PT_END(&pt_RFID_sub);
}

PT_THREAD(RFID_hitag_init(void)){
	char data_byte[2];

	PT_BEGIN(&pt_RFID_sub) ;
	for(RFID_cur_reader= 0; RFID_cur_reader < RFID_reader_cnt; RFID_cur_reader++){
	   if(RFID_Reader[RFID_cur_reader].reader_type >= READER_HIGTAG_2){
		   data_byte[0] = 0x03; //Encryption off
		   data_byte[1] = 0x00;
		   PT_SPAWN(&pt_RFID_sub, &pt_RFID_send, RFID_send_command(&RFID_Reader[RFID_cur_reader], &RFID_Reader[RFID_cur_reader].reader_com.com_status, RFID_ProgEprom, data_byte, 2, 0));
		   data_byte[0] = Higtag_type_byte(RFID_Reader[RFID_cur_reader].reader_type);
		   PT_SPAWN(&pt_RFID_sub, &pt_RFID_send, RFID_send_command(&RFID_Reader[RFID_cur_reader], &RFID_Reader[RFID_cur_reader].reader_com.com_status, RFID_ReaderType, data_byte, 1, 0));
	   }
   }
   PT_END(&pt_RFID_sub);
}


PT_THREAD(RFID_wait_for_data(RFID_reader_t * reader, Status * Com_status, uint8_t RX_lenght)){
	static struct etimer RFID_Response_timeout_t;

	PT_BEGIN(&pt_RFID_wait) ;
	etimer_set(&RFID_Response_timeout_t, CLOCK_SECOND *2);
	while(RFID_data_availible(reader) < (RX_lenght)){
		process_poll(PROCESS_CURRENT());
		PT_YIELD(&pt_RFID_wait);
		if(etimer_expired(&RFID_Response_timeout_t)){
			RFID_error(reader,RFID_TIMEOUT_ERROR);
			*Com_status = ERROR;
			PT_EXIT(&pt_RFID_wait);
		}
	}
	PT_END(&pt_RFID_wait);
}
/**
 *@brief  sub routine thread for sending data to the rfid reader
 **/
PT_THREAD(RFID_send_command(RFID_reader_t * reader, Status * Com_status, char command, char * Tx_data, uint8_t Tx_length, uint8_t RX_lenght)){
	char tmp[1];
    Status I2C_status;

    PT_BEGIN(&pt_RFID_send) ;
    while(RFID_data_availible(reader) > 0 ){
    	I2C_status = SerIn(reader->reader_addr,tmp,1);
    }
    I2C_status = SerOut(reader->reader_addr, &command,1);  //commando sturen naar RWD
    if(I2C_status != SUCCESS)
    {
    	RFID_error(reader,RFID_I2C_ERROR);
    	*Com_status = ERROR;
    	PT_EXIT(&pt_RFID_send);
    }

	if(Tx_length > 0)
	{
		I2C_status = SerOut(reader->reader_addr, Tx_data, Tx_length);  //data sturen naar RWD
		if(I2C_status != SUCCESS)
		{
			RFID_error(reader,RFID_I2C_ERROR);
			*Com_status = ERROR;
			PT_EXIT(&pt_RFID_send);
		}
	}
	*Com_status = SUCCESS;
	PT_SPAWN(&pt_RFID_send,&pt_RFID_wait,RFID_wait_for_data(reader, Com_status,RX_lenght +1));
	if(*Com_status == ERROR){
		PT_EXIT(&pt_RFID_send);
	}

	*Com_status = RFID_parse_flag_com_ok(reader);

    PT_END(&pt_RFID_send);
}

PT_THREAD(RFID_Write_Data(RFID_reader_t * reader, Status * com_status, int Block_address ,Key_t Key_type ,int Key_code_number,char * data))
{
   char RFID_Buffer[18];

   PT_BEGIN(&pt_RFID_send_data) ;
   if(reader->reader_type == READER_MIFARE){
	   RFID_Buffer[0] = Block_address;
	   RFID_Buffer[1] = (Key_type << 7) | Key_code_number;
	   memcpy(&RFID_Buffer[2],data,16);
   }else{
	   RFID_Buffer[0] = (Block_address*4);
	   memcpy(&RFID_Buffer[1],data,4);
   }
   PT_SPAWN(&pt_RFID_send_data,&pt_RFID_send, RFID_send_command(reader, com_status, RFID_WriteData, RFID_Buffer, (reader->reader_type == READER_MIFARE)? 18 : 5, 0));

   PT_END(&pt_RFID_send_data);
}

PT_THREAD(RFID_Read_Data(RFID_reader_t * reader, Status * com_status, int Block_address ,Key_t Key_type ,int Key_code_number,char * data))
{
   Status I2C_status;
   char RFID_Buffer[18];

   PT_BEGIN(&pt_RFID_send_data) ;
   RFID_Buffer[0] = (reader->reader_type == READER_MIFARE)?Block_address : Block_address*4;
   RFID_Buffer[1] = (Key_type << 7) | Key_code_number;
   PT_SPAWN(&pt_RFID_send_data,&pt_RFID_send,RFID_send_command(reader, com_status, RFID_ReadData, RFID_Buffer, (reader->reader_type == READER_MIFARE)? 2 : 1,(reader->reader_type == READER_MIFARE)? 16: 4));
   if(*com_status == SUCCESS){
	   I2C_status = SerIn(reader->reader_addr, data, (reader->reader_type == READER_MIFARE)? 16: 4);
	   if(I2C_status != SUCCESS){
		   RFID_error(reader,RFID_I2C_ERROR);
		   *com_status = false;
	   }
   }

   PT_END(&pt_RFID_send_data);
}

PT_THREAD(RFID_get_uid(RFID_reader_t * reader)){
	static Status com_status;
	char Buffer[7];
	char RxBuffer[7];
	uint8_t uid_len = 0;
	Status I2C_status;

	PT_BEGIN(&pt_RFID_sub) ;

	if(reader->reader_type == READER_MIFARE){
		PT_SPAWN(&pt_RFID_sub, &pt_RFID_send, RFID_send_command(reader,&com_status, RFID_GET_UID, NULL, 0, 0));
		if(reader->reader_card.card_present == true){
			PT_SPAWN(&pt_RFID_sub,&pt_RFID_wait,RFID_wait_for_data(reader, &com_status, 7));
			uid_len = 7;
		}else{
			PT_EXIT(&pt_RFID_sub);
		}
	}else{
		Buffer[0] = 0;
		PT_SPAWN(&pt_RFID_sub, &pt_RFID_send, RFID_send_command(reader,&com_status, RFID_ReadData, Buffer, 1, 0));
		if(reader->reader_card.card_present == true){
			PT_SPAWN(&pt_RFID_sub,&pt_RFID_wait,RFID_wait_for_data(reader, &com_status, 4));
			uid_len = 4;
		}else{
			PT_EXIT(&pt_RFID_sub);
		}
	}


	if(com_status == SUCCESS)
	{
		memset( RxBuffer, 0, 7); //init on 0
		I2C_status = SerIn(reader->reader_addr, RxBuffer, uid_len);
	  	if(I2C_status != SUCCESS){
	  		 RFID_error(reader,RFID_I2C_ERROR);
	  	}else{
			if(reader->reader_card.card_changed == true){
				if(memcmp(RxBuffer,reader->reader_card.card_UID.UID,7) != 0){
					reader->reader_card.card_new = true;
				}else{
					reader->reader_card.card_new = false;
				}
				memset (reader->reader_card.card_UID.UID, 0, 7); //init on 0
				reader->reader_card.card_UID.type = RFID_UID;
				memmove(reader->reader_card.card_UID.UID,RxBuffer,uid_len);
			}
	  		PT_EXIT(&pt_RFID_sub);
	  	}
	}

	PT_END(&pt_RFID_sub);
}

PT_THREAD(RFID_led_buzzer_state(RFID_reader_t * reader, bool busy)){
	static struct etimer RFID_fb_beep_t;
	char output_state = 0x00;
	char inv_output_state;

	PT_BEGIN(&pt_RFID_sub) ;

	if(reader->reader_type != READER_MIFARE){
		PT_EXIT(&pt_RFID_sub);
	}

	if(reader->reader_card.card_present == true){
		output_state |= F_GREEN_LED;
		if(busy == true){
			output_state |= F_RED_LED;
		}
	}else{
		output_state |= F_RED_LED;
	}

	if(reader->reader_beep_sound == true){
		output_state |= F_BUZZER;
	}

	inv_output_state = ~output_state;
	PT_SPAWN(&pt_RFID_sub, &pt_RFID_send, RFID_send_command(reader,&reader->reader_com.com_status, RFID_OutPut, &inv_output_state, 1, 0));

	if(reader->reader_beep_sound == true){
		etimer_set(&RFID_fb_beep_t,CLOCK_SECOND/5);//Set beep length
		PT_WAIT_UNTIL(&pt_RFID_sub, etimer_expired(&RFID_fb_beep_t));
		 //stop beep
		output_state &= ~F_BUZZER;
		inv_output_state = ~output_state;
		PT_SPAWN(&pt_RFID_sub, &pt_RFID_send, RFID_send_command(reader,&reader->reader_com.com_status, RFID_OutPut, &inv_output_state, 1, 0));
		reader->reader_beep_sound = false;
	}


	PT_END(&pt_RFID_sub);
}
 ///////////RFID Local functions///////////

Status RFID_parse_flag_com_ok(RFID_reader_t * reader){
	Status I2C_status;
	char status_flag;

	I2C_status = SerIn(reader->reader_addr, &status_flag, 1); //read status flag
	if(I2C_status != SUCCESS)
	{
		RFID_error(reader,RFID_I2C_ERROR);
		return ERROR;
	}

	if(status_flag & ((reader->reader_type == READER_MIFARE)?MFRC_ERROR:HTRC_ERROR)){
		RFID_error(reader,RFID_ANTENNA_ERROR);
		return ERROR;
	}


	if(status_flag & EEPROM_ERROR){
		RFID_error(reader,RFID_EEPROM_ERROR);
		return ERROR;
	}

	if(status_flag & RS232_ERROR){
		RFID_error(reader,RFID_RS232_ERROR);
		return ERROR;
	}

	if(status_flag & RX_OK){
		RFID_error(reader,RFID_NO_ERROR);
		if(reader->reader_card.card_present == false){
			reader->reader_card.card_changed = true;
			reader->reader_card.card_present = true;
		}
		if((reader->reader_type == READER_MIFARE)){
			if(status_flag & UL_TYPE){
				reader->reader_card.card_type = MIFARE_UL;
			}else{
				if(status_flag & MF_TYPE){
					reader->reader_card.card_type = MIFARE_4k;
				}else{
					reader->reader_card.card_type = MIFARE_1k;
				}
			}
		}else{
			reader->reader_card.card_type = reader->reader_type;
		}
		return SUCCESS;
	}else{
		RFID_error(reader,RFID_NO_ERROR);
		if(reader->reader_card.card_present == true){
			reader->reader_card.card_changed = true;
			reader->reader_card.card_present = false;
		}
		return SUCCESS;
	}
	return ERROR;
}

char Higtag_type_byte(RFID_card_type_t cardtype){
	 switch(cardtype){
	 	 case READER_HIGTAG_2:
	 		return 0x01;
	 	 case READER_HIGTAG_1S:
	 		return 0x02;
	 	 case READER_EM400XMC200:
	 		return 0x03;
	 	 default:
	 		return 0x00;
	 }
 }

uint8_t RFID_data_availible(RFID_reader_t * reader){
	  unsigned char statusval;
	  SerReadReg(reader->reader_addr,SerREG_RXLVL, &statusval);
	  return statusval;
}

/**
*@brief function to handle errors
**/
void RFID_error(RFID_reader_t * reader, RFID_reader_error_t reader_error)
 {
	reader->reader_error = reader_error;
    g_RFID_error = reader_error;
    if(reader_error != RFID_NO_ERROR){
    	printf("RFID %d:",reader->reader_addr);
    }
	switch(reader_error){
	case RFID_I2C_ERROR:
		printf("I2C failed \r\n");
		break;
	case RFID_RS232_ERROR:
		printf("RS232 error \r\n");
		break;
	case RFID_TIMEOUT_ERROR:
		printf("Timeout error \r\n");
		break;
	case RFID_ANTENNA_ERROR:
		printf("Antenna error \r\n");
		break;
	case RFID_EEPROM_ERROR:
		printf("Eeprom error \r\n");
		break;
	case RFID_NO_ERROR:
		if(reader->reader_error != RFID_NO_ERROR){
			g_RFID_error = RFID_NO_ERROR;
		}
		break;
	}
   return;
 }
