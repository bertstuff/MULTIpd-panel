#include "SCP_Device.h"
#include <Core/Memory/list.h>
#include <Core/Debug/Debug_Terminal.h>
#include <Core/Memory/cmem.h>
#include <Core/Process/process.h>
#include <Core/protocol/Net/dns/resolv.h>
#include <Core/Peripherals/Net_Dev_Init.h>
#include <Core/Extensions/ini/minIni.h>
#include <stdbool.h>

LIST(SCP_device_list);
PROCESS(SCP_Device_process, "SCP find devices");

void * SCP_Resolv_handel = NULL;
void * SCP_Backup_Resolv_handel = NULL;
process_event_t event_SCP_device_update;
const char inifile_SCP_devices[] = "SD1:/SCP_dev.cfg";

SCP_Device_t * SCP_create_new_device(uint16_t DevID);

void SCP_Device_init(void){
	uint8_t i = 0;
	SCP_Device_t * new_device;
	char Section_buffer[30];

	process_start(&SCP_Device_process, NULL);
	event_SCP_device_update = process_alloc_event(); //Claim a event number
	while(1){
		load_device:
		if(ini_getsection(i, Section_buffer, sizeof(Section_buffer), inifile_SCP_devices) > 0){
			new_device = SCP_create_new_device(ini_getl(Section_buffer,"DevID", 0, "Device ID", inifile_SCP_devices));
			new_device->Port = ini_getl(Section_buffer,"Port", 15233, "Port of device", inifile_SCP_devices);
			ini_resolv_ipaddr(&new_device->Resolv_handel,Section_buffer, "IP_URL", "multipass.custommate.eu", "URL or IP of device:", inifile_SCP_devices);//start ip resolver
			i++;
		}else{
			FIL fp;
			FRESULT rs;
			rs = f_open(&fp, inifile_SCP_devices, FA_READ);
			if(rs == FR_OK){
				f_close(&fp);
			}else if(rs == FR_NO_FILE){
				log_error("SCP: create device file\r\n");
				ini_gets("Server Communication", "Server_URL", "multipass.custommate.eu",NULL,Section_buffer,30,inifile_net);
				ini_puts("Server","IP_URL",Section_buffer,"URL or IP of device:",inifile_SCP_devices);
				ini_gets("Server Communication", "Server_MultiP_Port","15233",NULL,Section_buffer,30,inifile_net);
				ini_puts("Server","Port",Section_buffer,"Port of device",inifile_SCP_devices);
				ini_putl("Server","DevID",0,"Device id",inifile_SCP_devices);
				if(ini_getbool("Server Communication","backup_server_available",false,NULL, inifile_net)){
					ini_gets("Server Communication", "Backup_Server_URL", "multipass.custommate.eu",NULL,Section_buffer,30,inifile_net);
					ini_puts("Backup server","IP_URL",Section_buffer,"URL or IP of device:",inifile_SCP_devices);
					ini_gets("Server Communication", "Backup_Server_Port","15233",NULL,Section_buffer,30,inifile_net);
					ini_puts("Backup server","Port",Section_buffer,"Port of device",inifile_SCP_devices);
					ini_putl("Backup server","DevID",0,"Device id",inifile_SCP_devices);
				}
				goto load_device;
			}else{
				log_error("SCP: Cant open device file\r\n");
			}
			break;
		}
	}
}

void SCP_Device_found(SCP_Device_t * Device){
	SCP_Device_t * Device_loop = list_head(SCP_device_list);

	Device->Order = 0;

	while(Device_loop != NULL){
		if(Device_loop != Device){
			if(Device_loop->Info.SCP_devID == Device->Info.SCP_devID){
				Device->Order++;
			}
		}

		Device_loop = list_item_next(Device_loop);
	}

	printf("SCP[%d]: Device known DevID %d on %d.%d.%d.%d\r\n",clock_time(), Device->Info.SCP_devID, uip_ipaddr_to_quad(&Device->Ip));
	BUFFER_SCP_MSG_BOOT(SCP_Get_device_info(),Device->Info.SCP_devID);
	return;
}

SCP_Device_t * SCP_create_new_device(uint16_t DevID){
	SCP_Device_t *  new_device;

	new_device = cmem_malloc(sizeof(SCP_Device_t));
	if(new_device == NULL){
		return NULL;
	}
	memset(new_device,0,sizeof(SCP_Device_t));
	new_device->Info.SCP_devID = DevID;
	new_device->State = DEV_UNKNOWN;
	new_device->next_order = NULL;
	new_device->prev_order = NULL;
	new_device->Attempts = 3;
	SCP_Device_t * Device = list_head(SCP_device_list);
	if(DevID == 0){
		new_device->Info.SCP_grID = SERVER;
	}
	while(Device != NULL){
		if(Device->Info.SCP_devID == DevID){
			new_device->Order++;
			new_device->prev_order = Device;
			if(new_device->next_order == NULL){
				Device->next_order = new_device;
			}
		}

		Device = list_item_next(Device);
	}
	list_add(SCP_device_list, new_device);
	return new_device;
}

void SCP_Device_delete(SCP_Device_t * Device){
	list_remove(SCP_device_list, Device);
	cmem_free(Device);
	return;
}

SCP_Device_t * SCP_Device_connection(struct uip_conn * conn){
	SCP_Device_t * Device = list_head(SCP_device_list);

	while(Device != NULL){
		if(memcmp(&Device->Ip, &uip_conn->ripaddr, sizeof(uip_ipaddr_t))==0){
			return Device;
		}

		Device = list_item_next(Device);
	}
	return NULL;
}

bool SCP_Device_Known(uint16_t DevID){
	SCP_Device_t * Device = list_head(SCP_device_list);

	while(Device != NULL){
		if(Device->Info.SCP_devID == DevID){
			return true;
		}

		Device = list_item_next(Device);
	}
	return false;
}


void SCP_Device_Find(uint16_t DevID){

	return;
}

SCP_Device_t * SCP_Loop_known_devices(SCP_Device_t * Device){
	SCP_Device_t * Device_found;
	if(Device == NULL){
		Device_found = list_head(SCP_device_list);
	}else{
		Device_found = list_item_next(Device);
	}
	while(Device_found){
		if(Device_found->State > DEV_UNKNOWN){
			return Device_found;
		}else{
			Device_found = list_item_next(Device_found);
		}
	}
	return NULL;
}

SCP_Device_t * SCP_Loop_active_devices(SCP_Device_t * Device){
	SCP_Device_t * Device_loop = NULL;
	SCP_Device_t * Device_found = NULL;

	if(Device == NULL){
		Device_loop = list_head(SCP_device_list);
	}else{
		Device_loop = list_item_next(Device);
	}
	while(Device_loop){
		if(Device_loop->Order == 0){
			while(1){
				if(Device_loop == NULL){
					return Device_found;
				}
				if(Device_loop->Attempts == 0){
					Device_found = Device_loop;
					Device_loop = Device_loop->next_order;
				}else{
					if(Device_loop->State > DEV_UNKNOWN){
						return Device_loop;
					}else{
						return Device_found;
					}
				}
			}
		}else{
			Device_loop = list_item_next(Device_loop);
		}
	}
	return NULL;
}

uint8_t SCP_number_of_known_devices(void){
	uint8_t number = 0 ;
	SCP_Device_t * Device_found;
	Device_found = list_head(SCP_device_list);
	while(Device_found){
		if(Device_found->State > DEV_UNKNOWN){
			number++;
		}
		Device_found = list_item_next(Device_found);
	}
	return number;
}


bool SCP_Device_Sock_Begin(SCP_Device_t * Device){
	if(Device->Buffer != NULL){
		cmem_free(Device->Buffer);
	}
	Device->Buffer = cmem_malloc(SCP_BUFFER_SIZE);
	if(Device->Buffer == NULL){
		return false;
	}
	PSOCK_INIT(&Device->Socket, Device->Buffer, SCP_BUFFER_SIZE);
	Device->Cur_Packet = NULL;
	return true;
}

bool SCP_Device_Sock_End(SCP_Device_t * Device){
	cmem_free(Device->Buffer);
	Device->Buffer = NULL;
	if(Device->Cur_Packet != NULL){
		Device->Cur_Packet->send = false;
		Device->Cur_Packet = NULL;
	}
	return true;
}

bool SCP_Device_prev_online(SCP_Device_t * Device){
	while(Device->prev_order){
		if(Device->prev_order->Attempts > 0){
			return true;
		}
		Device = Device->prev_order;
	}
	return false;
}

PROCESS_THREAD(SCP_Device_process, ev, data)
{

	PROCESS_BEGIN();
	while(1){
		PROCESS_WAIT_EVENT();

		if(ev == resolv_event_found){
			SCP_Device_t * Device = list_head(SCP_device_list);
			while(Device != NULL){
				if(get_resolv_ipaddr(&Device->Resolv_handel, &Device->Ip, data) && (Device->Resolv_handel != NULL)){
					Device->Resolv_handel = NULL;
					Device->State = DEV_DISCONNECTED;
					printf("SCP: New device %d: %d.%d.%d.%d on port %d \r\n",Device->Info.SCP_devID,uip_ipaddr_to_quad(&Device->Ip), Device->Port);
					Device->Keep_connection = true;
					BUFFER_SCP_MSG_BOOT(SCP_Get_device_info(),Device->Info.SCP_devID);
					//process_post(PROCESS_BROADCAST, event_SCP_device_update, Device);
					break;
				}
				Device = list_item_next(Device);
			}
		}
	}
	PROCESS_END();
}


uint8_t SCP_Devices_of_types(uint16_t * Devices_list, uint8_t Devices_list_size, uint8_t num, ...){
	 SCP_Device_t * Device_loop;
	 uint8_t device_type;
	 uint8_t nr_devices = 0;
	 uint8_t i;

	 va_list valist;
	 va_start(valist, num);
	 for (i = 0; i < num; i++) {
		 Device_loop = list_head(SCP_device_list);
		 device_type = va_arg(valist, int);
		 while(Device_loop){
			 if((Device_loop->Info.SCP_grID == device_type)&&(Device_loop->Order == 0)){
				 Devices_list[nr_devices] = Device_loop->Info.SCP_devID;
				 nr_devices++;
				 if(nr_devices == Devices_list_size){
					 return nr_devices;
				 }
			 }
			 Device_loop = list_item_next(Device_loop);
		 }
     }
	 va_end(valist);
	 return nr_devices;
}
