#include "Debug_Terminal.h"
#include <Board/Def_config.h>
#include <Core/Clock/rtc_cal.h>
#include <Core/Clock/Time_functions.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#if (BOARD_SD_CARD == 1)
	#include <Core/protocol/FAT/diskio.h>
	#include <Core/protocol/FAT/ff.h>
#endif

#define WRITEFUNC __sys_write

unsigned int last_error[5];
uint8_t error_counter;


int WRITEFUNC (int iFileHandle, char *pcBuffer, int iLength)
{
#if (SERIAL_TERMINAL_ENABLE == 1)
  Serial_Terminal_send(pcBuffer,iLength);
#endif
#if (TCP_TERMINAL_ENABLE == 1)
  TCP_Terminal_send(pcBuffer,iLength);
#endif
  return iLength;
}

int log_error( const char * format, ... ){
  char print_buffer[100];
  uint32_t return_address;
  int retval;

  asm("mov %0, lr" : "=r"(return_address));

  sprintf(print_buffer,"(ERROR 0x%x)",return_address);
  va_list args;
  va_start (args, format);
  strcat(print_buffer,format);
  retval = printf(print_buffer,args);
  va_end (args);

#if (BOARD_SD_CARD == 1)
#if(ERROR_LOG_ENABLE == 1)
  uint8_t i;
  //error already logged
    for(i = 0; i < 5; i++){
      if(last_error[i] == return_address){
        return retval;
      }
    }
    last_error[error_counter] = return_address;
    error_counter++;
    if(error_counter > 9){error_counter = 0;}

    //print to file
    struct tm rtc_time;
    rtc_cal_gettime(&rtc_time);
    rtc_time = *LocalTime(&rtc_time);
    FIL file;
    FRESULT rc;
    rc = f_open(&file,"Error.log",FA_OPEN_ALWAYS | FA_WRITE);
    if(rc == FR_OK){
	  f_lseek(&file, f_size(&file));
	  f_printf (&file,"(%d-%d-%d %d:%d:%d)",rtc_time.tm_mday,rtc_time.tm_mon+1,rtc_time.tm_year+1900,rtc_time.tm_hour,rtc_time.tm_min,rtc_time.tm_sec);
	  f_printf (&file,print_buffer,args);
	  rc = f_close(&file);
    }
#endif
#endif

  return retval;
}
