#include <Board/Def_config.h>
#if(AMS_MODULE == 1)
#include "AMS_command.h"
#include "AMS.h"
#include <Core/Util/build_defs.h>
#include <Processes/dev_reset.h>
#include <Core/Peripherals/Net_Dev_Init.h>
#include <Core/protocol/Net/SCP/SCP_Comm.h>
#include <Core/Clock/rtc_cal.h>

#define DATA_START_PTR &packet.raw
AMS_msg_t g_AMS_send_command = M_NONE;

const char * AMS_State[1] = {
		"Akkoord",				//0
};
const char * AMS_error[7] = {
		"Unknown command", //0,
		"Unknown command type", //1,
		"SD card error", //2,
		"File not found", //3,
		"FAT error", //4,
		"crc error" //5
};

/*prototypes*/

bool AMS_firmware_info(AMS_packet_t * packet);
bool AMS_reset_dev(AMS_packet_t * packet);
bool AMS_Debug_terminal(AMS_packet_t * packet);
bool AMS_FileShare(AMS_packet_t * packet);
bool AMS_ethernet_info(AMS_packet_t * packet);
bool AMS_SCP_info(AMS_packet_t * packet);
bool AMS_Time_info(AMS_packet_t * packet);


/*---------------------------------functions----------------------------------------------------*/
uint8_t * AMS_add_string(uint8_t * buf,char * format,...){
	va_list args;
	uint8_t bytes;

	va_start (args, format);
	bytes = vsnprintf ((char *)buf, 150, format, args);
	va_end (args);
	return buf + bytes;
}

uint8_t * AMS_add_int(uint8_t * buf,uint8_t bit,uint32_t value){
	memmove(buf,&value,(bit/8));
	return buf + (bit/8);
}

/*---------------------------------send_data----------------------------------------------------*/


AMS_packet_t AMS_State_msg(bool send, AMS_msg_t command,uint32_t state_nr){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.command = command;
	packet.cmd_type = T_STATE;
	packet.raw = malloc(150);
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_string(buf_ptr, (char *)AMS_State[state_nr]);
	buf_ptr = AMS_add_int(buf_ptr, 32, state_nr);
	packet.header.data_lenght = buf_ptr - packet.raw;

	if(send == true){
		lenght = AMS_packet_to_data(&packet);
		uip_send(uip_appdata, lenght);
		free(packet.raw);
	}
	return packet;
}


AMS_packet_t AMS_error_msg(bool send, AMS_msg_t command,uint32_t error_nr){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.command = command;
	packet.cmd_type = T_ERROR;
	packet.raw = malloc(150);
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_string(buf_ptr, (char *)AMS_error[error_nr]);
	buf_ptr = AMS_add_int(buf_ptr, 32, error_nr);
	packet.header.data_lenght = buf_ptr - packet.raw;

	if(send == true){
		lenght = AMS_packet_to_data(&packet);
		uip_send(uip_appdata, lenght);
		free(packet.raw);
	}
	return packet;
}

AMS_packet_t AMS_file_error_msg(bool send, AMS_msg_t command, FRESULT rc){
	AMS_packet_t packet;
	packet.command = M_NONE;

	switch(rc){
	case FR_DISK_ERR:
	case FR_INT_ERR:
	case FR_NOT_READY:
		return AMS_error_msg(send, command,2); //SD card error
	case FR_NO_FILE:
	case FR_NO_PATH:
	case FR_INVALID_NAME:
		return AMS_error_msg(send, command,3); //File not found
	case FR_DENIED:
	case FR_INVALID_OBJECT:
	case FR_WRITE_PROTECTED:
	case FR_INVALID_DRIVE:
	case FR_NOT_ENABLED:
	case FR_NO_FILESYSTEM:
	case FR_MKFS_ABORTED:
	case FR_TIMEOUT:
	case FR_LOCKED:
	case FR_NOT_ENOUGH_CORE:
	case FR_TOO_MANY_OPEN_FILES:
	case FR_INVALID_PARAMETER:
		return AMS_error_msg(send, command,4); //FAT error
	default:
		return packet;
		break;
	}
}


AMS_packet_t AMS_connect_msg(bool send){
	AMS_packet_t packet;
	uint32_t lenght;

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.header.data_lenght = 3;
	packet.raw = NULL;
	packet.command = M_CONNECT;
	packet.cmd_type = T_NONE;

	if(send == true){
		lenght = AMS_packet_to_data(&packet);
		uip_send(uip_appdata, lenght);
	}
	return packet;
}


AMS_packet_t AMS_servicepin_msg(bool send){

	AMS_packet_t packet;
	uint32_t lenght;

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.header.data_lenght = 3;
	packet.raw = NULL;
	packet.command = M_SERVICEPIN;
	packet.cmd_type = T_ACTIVE;

	if(send == true){
		lenght = AMS_packet_to_data(&packet);
		uip_send(uip_appdata, lenght);
	}
	return packet;
}

void AMS_Firmware_info_msg(void){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	uint8_t buffer[150];
	struct tm build_tm = get_build_date();

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.raw = buffer;
	packet.command = M_FIRMWARE_INFO;
	packet.cmd_type = T_INFO;
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_string(buf_ptr,PRODUCT_NAME);
	buf_ptr = AMS_add_int(buf_ptr, 8, MAJOR_VERSION);
	buf_ptr = AMS_add_int(buf_ptr, 8, MINOR_VERSION);
	buf_ptr = AMS_add_int(buf_ptr, 32, atoi(PRODUCTNR));
	buf_ptr = AMS_add_int(buf_ptr, 16, build_tm.tm_year);
	buf_ptr = AMS_add_int(buf_ptr, 8, build_tm.tm_mon);
	buf_ptr = AMS_add_int(buf_ptr, 8, build_tm.tm_mday);
	buf_ptr = AMS_add_int(buf_ptr, 8, build_tm.tm_hour);
	buf_ptr = AMS_add_int(buf_ptr, 8, build_tm.tm_min);
	packet.header.data_lenght = buf_ptr - buffer;

	lenght = AMS_packet_to_data(&packet);
	uip_send(uip_appdata, lenght);

	return;
}


AMS_packet_t AMS_FileShare_data_done_msg(bool send, uint16_t crc){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.header.data_lenght = 3;
	packet.raw = malloc(1);
	packet.command = M_FILESHARE;
	packet.cmd_type = T_DATA_DONE;

	buf_ptr = packet.raw;
	buf_ptr = AMS_add_int(buf_ptr, 16, crc);

	if(send == true){
		lenght = AMS_packet_to_data(&packet);
		uip_send(uip_appdata, lenght);
	}
	return packet;
}

void AMS_Ethernet_ip_info_msg(void){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	uint8_t buffer[150];

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.raw = buffer;
	packet.command = M_ETHERNET_INFO;
	packet.cmd_type = T_IP;
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.IP_address.u8[0]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.IP_address.u8[1]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.IP_address.u8[2]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.IP_address.u8[3]);
	packet.header.data_lenght = buf_ptr - buffer;

	lenght = AMS_packet_to_data(&packet);
	uip_send(uip_appdata, lenght);

	return;
}

void AMS_Ethernet_mac_info_msg(void){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	uint8_t buffer[150];

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.raw = buffer;
	packet.command = M_ETHERNET_INFO;
	packet.cmd_type = T_MAC;
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.MAC_Address.addr[0]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.MAC_Address.addr[1]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.MAC_Address.addr[2]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.MAC_Address.addr[3]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.MAC_Address.addr[4]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.MAC_Address.addr[5]);
	packet.header.data_lenght = buf_ptr - buffer;

	lenght = AMS_packet_to_data(&packet);
	uip_send(uip_appdata, lenght);

	return;
}

void AMS_Ethernet_netmask_info_msg(void){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	uint8_t buffer[150];

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.raw = buffer;
	packet.command = M_ETHERNET_INFO;
	packet.cmd_type = T_NETMASK;
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.NetMask.u8[0]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.NetMask.u8[1]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.NetMask.u8[2]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.NetMask.u8[3]);
	packet.header.data_lenght = buf_ptr - buffer;

	lenght = AMS_packet_to_data(&packet);
	uip_send(uip_appdata, lenght);

	return;
}

void AMS_Ethernet_def_gateway_info_msg(void){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	uint8_t buffer[150];

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.raw = buffer;
	packet.command = M_ETHERNET_INFO;
	packet.cmd_type = T_DEF_GATEWAY;
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.Def_Gateway.u8[0]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.Def_Gateway.u8[1]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.Def_Gateway.u8[2]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.Def_Gateway.u8[3]);
	packet.header.data_lenght = buf_ptr - buffer;

	lenght = AMS_packet_to_data(&packet);
	uip_send(uip_appdata, lenght);

	return;
}

void AMS_Ethernet_dns_info_msg(void){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	uint8_t buffer[150];

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.raw = buffer;
	packet.command = M_ETHERNET_INFO;
	packet.cmd_type = T_DNS;
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.DNS_Address.u8[0]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.DNS_Address.u8[1]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.DNS_Address.u8[2]);
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.DNS_Address.u8[3]);
	packet.header.data_lenght = buf_ptr - buffer;

	lenght = AMS_packet_to_data(&packet);
	uip_send(uip_appdata, lenght);

	return;
}

void AMS_Ethernet_dhcp_info_msg(void){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	uint8_t buffer[150];

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.raw = buffer;
	packet.command = M_ETHERNET_INFO;
	packet.cmd_type = T_DHCP;
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_int(buf_ptr, 8, Net_settings.dhcp?1:0);
	packet.header.data_lenght = buf_ptr - buffer;

	lenght = AMS_packet_to_data(&packet);
	uip_send(uip_appdata, lenght);

	return;
}

void AMS_SCP_info_msg(void){

	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	uint8_t buffer[150];
	SCP_DevInfo_t devinfo;

	devinfo = SCP_Get_device_info();

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.raw = buffer;
	packet.command = M_SCP_INFO;
	packet.cmd_type = T_INFO;
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_int(buf_ptr, 16, devinfo.SCP_devID );
	buf_ptr = AMS_add_int(buf_ptr, 8, devinfo.SCP_locnr);
	buf_ptr = AMS_add_int(buf_ptr, 8, devinfo.SCP_modnr);
	buf_ptr = AMS_add_int(buf_ptr, 8, devinfo.SCP_grID);
	packet.header.data_lenght = buf_ptr - buffer;

	lenght = AMS_packet_to_data(&packet);
	uip_send(uip_appdata, lenght);

	return;
}

void AMS_Time_info_msg(void){
	AMS_packet_t packet;
	uint32_t lenght;
	uint8_t * buf_ptr;
	uint8_t buffer[150];
	struct tm Cur_time;

	rtc_cal_gettime(&Cur_time);

	packet.encrypted = true;
	packet.header.conn_type = COMMAND;
	packet.raw = buffer;
	packet.command = M_TIME_INFO;
	packet.cmd_type = T_INFO;
	buf_ptr = packet.raw;
	buf_ptr = AMS_add_int(buf_ptr, 16, Cur_time.tm_year );
	buf_ptr = AMS_add_int(buf_ptr, 8, Cur_time.tm_mon);
	buf_ptr = AMS_add_int(buf_ptr, 8, Cur_time.tm_mday);
	buf_ptr = AMS_add_int(buf_ptr, 8, Cur_time.tm_hour);
	buf_ptr = AMS_add_int(buf_ptr, 8, Cur_time.tm_min);
	buf_ptr = AMS_add_int(buf_ptr, 8, Cur_time.tm_sec);
	packet.header.data_lenght = buf_ptr - buffer;

	lenght = AMS_packet_to_data(&packet);
	uip_send(uip_appdata, lenght);

	return;
}
/*------------------------------execute command-------------------------------------------------*/
bool AMS_execute_command(AMS_packet_t * packet){
	switch(packet->command){
	case M_CONNECT:
		break;
	case M_WINK:
		break;
	case M_SERVICEPIN:
		break;
	case M_FIRMWARE_INFO:
		return AMS_firmware_info(packet);
	case M_RESET:
		return AMS_reset_dev(packet);
	case M_DEBUG_TERMINAL:
		return AMS_reset_dev(packet);
	case M_FILESHARE:
		return AMS_FileShare(packet);
	case M_ETHERNET_INFO:
		return AMS_ethernet_info(packet);
	case M_SCP_INFO:
		return 	AMS_SCP_info(packet);;
	case M_TIME_INFO:
		return AMS_Time_info(packet);
	default:
		AMS_error_msg(true,packet->command,0);
		return false;
	}
	return true;
}

bool AMS_firmware_info(AMS_packet_t * packet){
	if(packet->cmd_type == T_GET_INFO){
		AMS_Firmware_info_msg();
	}else{
		AMS_error_msg(true,M_FIRMWARE_INFO,1);
		return false;
	}
	return true;
}

bool AMS_reset_dev(AMS_packet_t * packet){
	switch(packet->cmd_type){
	case T_SAVE_RESET: Save_reset();
		break;
	case T_HARD_RESET: Hard_reset();
		break;
	default:
		AMS_error_msg(true,M_RESET,1);
		return false;
	}
	return true;
}

bool AMS_Debug_terminal(AMS_packet_t * packet){
	g_AMS_file_port = packet->raw[0] << 8;
	g_AMS_file_port |= packet->raw[1] << 0;
	switch(packet->cmd_type){
	case T_START:
		break;
	case T_STOP:
		break;
	default:
		AMS_error_msg(true,M_DEBUG_TERMINAL,1);
		break;
	}
	return true;
}

bool AMS_FileShare(AMS_packet_t * packet){
	FRESULT rc;
	char * path;
	uint8_t * parse_ptr;

	parse_ptr = packet->raw;
	path = (char *)packet->raw;
	parse_ptr += strlen(path);
	g_AMS_file_port = parse_ptr[1] << 8;
	g_AMS_file_port |= parse_ptr[2] << 0;


	g_AMS_data_type = packet->cmd_type;
	switch(packet->cmd_type){
	case T_STATE:
		break;
	case T_ERROR:
		printf("AMS: %s \r\n",path);
		break;
	case T_DOWNLOAD:
		g_AMS_crc = parse_ptr[3] << 8;
		g_AMS_crc |= parse_ptr[4] << 0;
	case T_UPLOAD:
		g_data_file = malloc(strlen(path));
		strcpy(g_data_file,path);
		packet->encrypted = false;
		g_AMS_data_packet.header.conn_type = FILE_DATA;
		printf("AMS: fileshare file:%s port:%d\r\n",path,g_AMS_file_port);
		process_start(&AMS_data_process, NULL);
		break;
	case T_LIST:
		break;
	case T_DELETE:
		rc = f_unlink(path);
		if(rc != FR_OK){
			printf("AMS: failed t (%d)\r\n",rc);
		}
		break;
	default:
		AMS_error_msg(true,M_FILESHARE,1);
		break;
	}

	return true;
}

bool AMS_ethernet_info(AMS_packet_t * packet){
	switch(packet->cmd_type){
	case T_GET_IP:
		AMS_Ethernet_ip_info_msg();
		break;
	case T_GET_MAC:
		AMS_Ethernet_mac_info_msg();
		break;
	case T_GET_NETMASK:
		AMS_Ethernet_netmask_info_msg();
		break;
	case T_GET_DEF_GATEWAY:
		AMS_Ethernet_def_gateway_info_msg();
		break;
	case T_GET_DNS:
		AMS_Ethernet_dns_info_msg();
		break;
	case T_GET_DHCP:
		AMS_Ethernet_dhcp_info_msg();
	default:
		AMS_error_msg(true,M_ETHERNET_INFO,1);
		return false;
	}
	return true;
}

bool AMS_SCP_info(AMS_packet_t * packet){
	if(packet->cmd_type == T_GET_INFO){
		AMS_SCP_info_msg();
	}else{
		AMS_error_msg(true,M_SCP_INFO,1);
		return false;
	}
	return true;
}

bool AMS_Time_info(AMS_packet_t * packet){
	if(packet->cmd_type == T_GET_INFO){
		AMS_Time_info_msg();
	}else{
		AMS_error_msg(true,M_TIME_INFO,1);
		return false;
	}
	return true;
}
#endif
