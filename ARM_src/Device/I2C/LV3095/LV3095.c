#include "LV3095.h"
#include <Device/Devices.h>
#include <core/Extensions/ini/minIni.h>
#include <Device/I2C/SC16IS740/SC16IS740.h>
#include <Device/I2C/RFID/RFID.h>
#include <Core/Peripherals/I2C_Dev_Init.h>
#include <Core/Debug/Debug_Terminal.h>
#include <Core/Debug/Error_check.h>
#include <Core/Timer/etimer.h>
#include <core/Thread/pt.h>
#include <Board/Def_config.h>
#if(PAYING_MODULE == 1)
#include <Screen/SCR_Text.h>
#endif

#define RFID_OutPut 0x4F	//0x4F (O) Output control byte.

////////Global variables
process_event_t event_QR_present;
uint8_t QR_addr;
QR_User_t Current_QR = {.type = QR_USER_TYPE};
QR_txt_t Current_TXT = {.type = QR_TEXT_TYPE,.max_lenght = 20,.buffer = NULL};
Status QR_I2C_status = SUCCESS;
bool QR_enabled;


struct pt pt_QR_send;
/////////prototypes///////////////////
bool QR_Read_new_Data(void);
bool QR_parse_UID(char * data);
bool QR_parse_LANG(char * data);
bool QR_parse_data(char * data);
void QR_trigger(bool enable);
PROCESS(QR_process, "QR reader");

void QR_failed(void){
	log_error("QR reader: i2c comm failed\r\n");
	return;
}

void QR_init(bool addr_jpr){
	Ser_UART_Config_t Config;
	g_Devices_available.QR_available = ini_getbool("Device","QR_available",false,"QR available:",inifile_device);
	if(g_Devices_available.QR_available == false){
		return;
	}
	event_QR_present = process_alloc_event(); //Claim a event number

	error_check_add(TYPE_STATUS,&QR_I2C_status,ERROR_QR_I2C);

	I2C_Dev_Init(QR_MAX_SPEED_HZ);
	QR_addr = addr_jpr?0x03:0x02;

	Config.Baudrate = 9600;
	Config.NumBits = 8;
	Config.StopBit = 1;
	Config.Parity = PAR_None;
	Config.AutoCTS = false;
	Config.AutoRTS = false;
	Config.RTSActive = false;
	Config.RS485 = false;

	QR_I2C_status = SerInitUART(QR_addr, Config);
	if(QR_I2C_status != SUCCESS)
	{
		QR_failed();
	}

	process_start(&QR_process,NULL);
	QR_trigger(false);
	Current_QR.Language = NOT_SET;
}

void QR_RFID_init(bool addr_jpr){
	RFID_init(addr_jpr==true?0x01:0x00,READER_MIFARE);
	QR_init(addr_jpr);
	return;
}

void QR_trigger(bool enable){
	if(SerSetRTS(QR_addr,enable) == ERROR){
		QR_failed();
	}
	return;
}

void QR_enable(bool enable){
	QR_trigger(false);
	QR_enabled = enable;
	printf("QR: %s  \r\n",(enable == true)?"enable":"disable");
}

PROCESS_THREAD(QR_process, ev, data)
{
	static struct etimer poll_tm;
	static struct etimer puls_tm;

   if(etimer_expired(&poll_tm)) {
     etimer_set(&poll_tm, CLOCK_SECOND/2);
   }

   PROCESS_BEGIN();
   //QR_init(false);
   while (ev != PROCESS_EVENT_EXIT){ //main loop  till exit
	   PROCESS_WAIT_EVENT();
	   if(ev == PROCESS_EVENT_TIMER){
		   while(QR_enabled == true){
			   QR_trigger(false);
			   etimer_set(&puls_tm,CLOCK_SECOND/100);
			   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&puls_tm));
			   QR_trigger(true);
			   etimer_set(&puls_tm,CLOCK_SECOND);
			   PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&puls_tm));
			   if(QR_Read_new_Data()){
				   QR_enabled = false;
				   QR_trigger(false);
				   RFID_beep_sound();
			   }
			   QR_trigger(false);
		   }
		   QR_trigger(false);
	   }
   }
   printf("QR: exit \r\n");
   PROCESS_END();
}

uint16_t QR_crc_cal_by_bit(char * ptr, uint16_t len){
	uint16_t crc = 0;
	uint8_t i;
	while(len-- != 0){
		for(i = 0x80; i != 0; i /= 2){
			crc *= 2;
			if((crc&0x10000) !=0)
			crc ^= 0x11021;
			if((*ptr&i) != 0)
			crc ^= 0x1021;
		}
		ptr++;
	}
	return crc;
}

//!Not tested yet!
PT_THREAD(QR_write_register(Status * stat ,uint16_t reg, uint8_t * data, uint8_t len)){

	static struct etimer QR_Response_delay_t;
	char tx_buffer[100];
	char rx_buffer[100];
	uint16_t crc;

	PT_BEGIN(&pt_QR_send) ;
	*stat = SUCCESS;
	tx_buffer[0] = 0x7E;//prefix1
	tx_buffer[1] = 0x00;//prefix1
	tx_buffer[2] = 0x08;//type
	tx_buffer[3] = len;//lenght(lens)
	tx_buffer[4] = (reg >> 8); //register High byte
	tx_buffer[5] = reg; //register low byte
	memmove(&tx_buffer[6],data,len);
	crc = QR_crc_cal_by_bit(&tx_buffer[3],len+3);
	tx_buffer[len+6] = (crc >> 8); //crc
	tx_buffer[len+7] = crc; //crc
	QR_I2C_status =  SerOut(QR_addr,tx_buffer,len+8);
	if(QR_I2C_status != SUCCESS){
		*stat = ERROR;
		QR_failed();
		PT_EXIT(&pt_QR_send);
	}

	etimer_set(&QR_Response_delay_t, CLOCK_SECOND / 5);
	PT_WAIT_UNTIL(&pt_QR_send, etimer_expired(&QR_Response_delay_t));
	QR_I2C_status = SerIn(QR_addr, rx_buffer, 7);
	if((rx_buffer[0] != 0x02)||(rx_buffer[1] != 0x00)||(QR_I2C_status == ERROR)){
		*stat = ERROR;
		PT_EXIT(&pt_QR_send);
	}

	if(rx_buffer[2] == 0x00){
		*stat = SUCCESS;
	}else{
		*stat = ERROR;
	}

	PT_END(&pt_QR_send);
}

bool QR_Read_new_Data(void){

	uint8_t RX_level;
	char * RX_buffer;
	bool new_QR;

	QR_I2C_status = SerReadReg(QR_addr, SerREG_RXLVL, &RX_level);
	if(QR_I2C_status != SUCCESS){
			QR_failed();
	}
	if(RX_level == 0){
		return false;
	}
	RX_buffer = malloc(RX_level+1);
	if(RX_buffer == NULL){
		return false;
	}
	QR_I2C_status |= SerIn(QR_addr, RX_buffer, RX_level);
	if(QR_I2C_status != SUCCESS){
			QR_failed();
	}
	RX_buffer[RX_level]='\0';
	new_QR = QR_parse_data(RX_buffer);
	free(RX_buffer);
	return new_QR;
}

bool QR_parse_TXT(char * data){
	char * start;
	char * end;
	uint8_t len;
	const char * start_str = "<TXT=";

	start = strstr (data,start_str);
	end = strstr (start,">");
	if((start == NULL) || (end == NULL)){return false;}
	start += strlen(start_str);
	len =  end - start +1;
	if(len > Current_TXT.max_lenght){len = Current_TXT.max_lenght;}

	if(Current_TXT.buffer != NULL){
		free(Current_TXT.buffer);
	}

	Current_TXT.buffer = malloc(len);
	memmove(Current_TXT.buffer,start,len-1);
	Current_TXT.buffer[len-1]='\0';
	Current_TXT.lenght = strlen(Current_TXT.buffer);
	Current_TXT.type = QR_TEXT_TYPE;
	return true;
}

bool QR_parse_UID(char * data){
	char * start;
	char * end;
	uint8_t i;
	char conv[3];
	const char * start_str = "<ID=";

	start = strstr (data,start_str);
	end = strstr (start,">");
	if((start == NULL) || (end == NULL)){return false;}
	start += strlen(start_str);
	if((start + 14) != end){return false;}
	for(i = 0;i < 7;i++){
		memmove(conv,start+(2*i),2);
		conv[2] = '\0';
		Current_QR.UID.UID[i] = (uint8_t)strtol(conv, NULL, 16);
		Current_QR.UID.type = QR_UID;
	}
	return true;
}

bool QR_parse_LANG(char * data){
	char * start;
	char * end;
	char conv[3];
	const char * start_str = "<LNG=";

	start = strstr (data,start_str);
	end = strstr (start,">");
	if((start == NULL) || (end == NULL)){return false;}
	start += strlen(start_str);
	if((start + 1) != end){return false;}
	memmove(conv,start,1);
	conv[2] = '\0';
	Current_QR.Language = (uint8_t)strtol(conv, NULL, 16);
	if(Current_QR.Language >= LANG_CNT){
		Current_QR.Language = NL;
	}
	return true;
}

bool QR_parse_tommy(char *data){
	uint8_t nr = 0;
	while(*data){
		if(*data == '|'){
			nr++;
		}
		data++;
	}
	if(nr == 2){
		return true;
	}
	return false;
}

bool QR_parse_data(char * data){

	if(QR_parse_UID(data) == true){
		 if(QR_parse_LANG(data) == false){
			 Current_QR.Language = NOT_SET;
		 }
		 Current_QR.txt = QR_parse_TXT(data);

		 printf("QR: present [UID: %02x ",Current_QR.UID.UID[0]);
		 printf("%02x ",Current_QR.UID.UID[1]);
		 printf("%02x ",Current_QR.UID.UID[2]);
		 printf("%02x ",Current_QR.UID.UID[3]);
		 printf("%02x ",Current_QR.UID.UID[4]);
		 printf("%02x ",Current_QR.UID.UID[5]);
		 printf("%02x] \r\n",Current_QR.UID.UID[6]);
		 //Let other processes know that there is a new QR code
		 process_post(PROCESS_BROADCAST,event_QR_present,&Current_QR);

		 return true;
	}

	if(QR_parse_TXT(data) == true){
		memset(Current_QR.UID.UID,0,7);
		process_post(PROCESS_BROADCAST,event_QR_present,&Current_TXT);
		return true;
	}

	if(QR_parse_tommy(data) == true){
		printf("QR-tommy:%s\r\n",data);
		return true;
	}
	printf("QR:%s",data);
	return false;
}


QR_User_t QR_get_current_user(void){
	if(Current_QR.Language >= LANG_CNT){
		Current_QR.Language = NL;
	}
	return Current_QR;
}

char * QR_get_txt(void){
	return Current_TXT.buffer;
}
