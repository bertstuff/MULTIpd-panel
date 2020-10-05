#include "EDIP_240B_Display.h"
#include "EDIP_240B_Send.h"
#include "EDIP_240B_Touch.h"


///////////////Global functions/////////////////////////////////////

Status Edip_Clear_Display(void){
  char buffer[3 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
    
  Data[0] = ESCLCD;
  Data[1] = 'D';
  Data[2] = 'L';
  Edip_Delete_Touch_all();
  return Edip_Send(buffer, 3, false);
}

Status Edip_Display_off(void){
  char buffer[3 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
    
  Data[0] = ESCLCD;
  Data[1] = 'D';
  Data[2] = 'A';
   
  Edip_Send(buffer, 3, false);
  return Edip_Delete_Touch_all();
}

Status Edip_Display_on(void){
  char buffer[3 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
    
  Data[0] = ESCLCD;
  Data[1] = 'D';
  Data[2] = 'E';
   
  return Edip_Send(buffer, 3, false);
}

Status Edip_Set_Backlight(uint8_t mode){
  char buffer[7 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  Data[0] = ESCLCD;
  Data[1] = 'Y';
  Data[2] = 'L';
  Data[3] = mode;
   
  return Edip_Send(buffer, 4, false);
}

Status Edip_Set_Backlight_Brightness(uint8_t procent){
  char buffer[7 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  Data[0] = ESCLCD;
  Data[1] = 'Y';
  Data[2] = 'H';
  Data[3] = procent;
   
  return Edip_Send(buffer, 4, false);
}

Status Edip_Set_Timout(uint8_t timeout){
  char buffer[3 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];

  Data[0] = 'D';
  Data[1] = 64;
  Data[2] = timeout;

  return Edip_Send(buffer, 3, true);
}

