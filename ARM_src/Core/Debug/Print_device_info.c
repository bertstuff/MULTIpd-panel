#include <Board/Def_config.h>
#include <Core/Memory/IAP.h>
#include <Core/Process/process.h>
#include <core/Util/build_defs.h>
#include <LPC17xx.h>
#include <stdint.h>
#include <stdio.h>
#include <version.h>

void Serial_Print_Device_Info(void)
{  
  unsigned Serial_Number[5];

  printf("\r\n\n\nTerminal debug start\r\n");
  printf("Product: %s \r\n",PRODUCT_NAME);
  printf("Version V%d.%d.%s%s\r\n",MAJOR_VERSION,MINOR_VERSION,Build_Revision,Version_state);
  printf("Build date: %s\r\n",Build_Date);
  printf("Product nr: %s\r\n",PRODUCTNR);
  read_serial_number(Serial_Number);
  printf("Serial Number: %08X_%08X_%08X_%08X\n\r\r\n",Serial_Number[1],Serial_Number[2],Serial_Number[3],Serial_Number[4]);

  return;
}


