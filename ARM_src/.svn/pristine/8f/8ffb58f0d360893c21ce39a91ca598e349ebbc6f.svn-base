#include <Board/Def_config.h>
#ifdef FLASH_MEM_MODULE
#include "flash_mem.h"
#include "IAP.h"
#include <Driver/include/lpc17xx_nvic.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <Core/Process/process.h>
#include <Core/Timer/etimer.h>
/*defines*/
#define CONFIG_FLASH_OFFSET 0x38000
#define CONFIG_FLASH_SECTOR	21
#define MEM_CHECK_VAL  0x4413

typedef struct{
	uint32_t MemCheck;
}Flash_mem_t;

/*global var*/
void * g_Ram_mem,* g_Def_Ram_mem;
uint32_t g_Ram_mem_size;

/*prototypes*/
bool Flash_mem_prepare(void);
void Flash_mem_load(void);
PROCESS(BOD_process, "BOD process");

/*functions*/
void Flash_mem_init(void * Ram_mem,void * Def_Ram_mem, uint32_t size){
	g_Ram_mem = Ram_mem;
	g_Def_Ram_mem = Def_Ram_mem;
	g_Ram_mem_size = size;


	while(1){
		Flash_mem_load();
		Flash_mem_prepare();
		if(((Flash_mem_t*)g_Ram_mem)->MemCheck != MEM_CHECK_VAL){
			memcpy(g_Ram_mem,g_Def_Ram_mem,g_Ram_mem_size);
			((Flash_mem_t*)g_Ram_mem)->MemCheck = MEM_CHECK_VAL;
			Flash_mem_save();
		}else{
			break;
		}
	}
	process_start(&BOD_process,NULL);

}

PROCESS_THREAD(BOD_process, ev, data)
{
	static struct etimer poll_tm;

	PROCESS_BEGIN();
	etimer_set(&poll_tm,CLOCK_SECOND * 10);
	PROCESS_WAIT_EVENT_UNTIL(etimer_expired(&poll_tm) == true);
	Enable_BOD();
	PROCESS_END();
}


void Flash_mem_load(void){
	void * Flash_loc;

	Flash_loc = (void *)CONFIG_FLASH_OFFSET;
	memcpy(g_Ram_mem,Flash_loc,g_Ram_mem_size);
}

bool Flash_mem_prepare(void){
	__e_iap_status iap_status;

	/* Init the IAP driver */
	iap_init();
	/* Prepare the sector for erase */
	iap_status = (__e_iap_status) iap_prepare_sector(CONFIG_FLASH_SECTOR,CONFIG_FLASH_SECTOR);
	if (iap_status != CMD_SUCCESS){
		return false;
	}

	 /* Erase the sector */
	 iap_status = (__e_iap_status) iap_erase_sector(CONFIG_FLASH_SECTOR,CONFIG_FLASH_SECTOR);
	 if (iap_status != CMD_SUCCESS){
		 return false;
	 }

	 /* Prepare the sector for writing */
	 iap_status = (__e_iap_status) iap_prepare_sector(CONFIG_FLASH_SECTOR,CONFIG_FLASH_SECTOR);
	 if (iap_status != CMD_SUCCESS){
		 return false;
	 }
	 return true;
}

bool Flash_mem_save(void){
	__e_iap_status iap_status;

	((Flash_mem_t*)g_Ram_mem)->MemCheck = MEM_CHECK_VAL;

	 /* write data to flash */
	 uint32_t Bytes_to_copy = 256;

	 if(g_Ram_mem_size > 1024){
		 Bytes_to_copy = 4096;
	 }else if(g_Ram_mem_size > 512){
		 Bytes_to_copy = 1024;
	 }else if(g_Ram_mem_size > 256){
		 Bytes_to_copy = 512;
	 }


	 iap_status = (__e_iap_status) iap_copy_ram_to_flash(g_Ram_mem,(void *)CONFIG_FLASH_OFFSET, Bytes_to_copy);
	 if (iap_status != CMD_SUCCESS){
		 return false;
	 }

	 return true;
}



void Enable_BOD(void){
	printf("BOD enable\r\n");
	NVIC_ClearPendingIRQ(BOD_IRQn);
	NVIC_EnableIRQ(BOD_IRQn);

}

void BOD_IRQHandler(void){
	/* save settings*/
	 Flash_mem_save();
	 printf("power fail!\r\n");
	 while(1){
		 ;
	 }
}
#endif
