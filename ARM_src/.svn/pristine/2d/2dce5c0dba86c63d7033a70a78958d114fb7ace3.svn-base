#include <Board/Def_config.h>
#include "SCR_Transaction_statistics.h"
#if (PAYING_MODULE == 1)
	#include <stdio.h>
	#include <core/protocol/FAT/ff.h>

	Transaction_statistics_t g_trans_statistics;
	const char inifile_stat[] = "SD1:/Statistics.log";

	void Transaction_statistics_load(void){
		  FIL file;
		  FRESULT rc;
		  UINT br;

		  if(f_open(&file,inifile_stat,FA_OPEN_ALWAYS | FA_READ) == FR_OK){
		    rc = f_read(&file,(void *)&g_trans_statistics,sizeof(Transaction_statistics_t),&br);
		    if(br == sizeof(Transaction_statistics_t) && (rc == FR_OK)){
			  rc = f_close(&file);
			  return;
		    }
		    rc = f_close(&file);
		  }
		  printf("Statistics: failed to load\r\n");
		  return;
	}

	void Transaction_statistics_save(void){
	  FIL file;
	  FRESULT rc;
	  UINT bw;

	  rc = 	f_mount(&SD_Fatfs,"SD1:",1);
	  if(rc == FR_OK){
		if(f_open(&file,inifile_stat,FA_CREATE_ALWAYS | FA_WRITE) == FR_OK){
		  rc = f_write(&file,(void *)&g_trans_statistics,sizeof(Transaction_statistics_t),&bw);
		  if(bw == sizeof(Transaction_statistics_t) && (rc == FR_OK)){
			  rc = f_close(&file);
			  return;
		  }
		  rc = f_close(&file);
		}
	  }
	  printf("Statistics: failed to save\r\n");
	  return;
	}




#endif
