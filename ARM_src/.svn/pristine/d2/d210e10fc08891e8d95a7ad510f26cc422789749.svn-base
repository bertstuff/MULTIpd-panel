#include "EDIP_240B_Clipboard.h"
#include "EDIP_240B_Send.h"
#include <stdlib.h>
#include <Core/Type/type.h>

///////////////Prototype/////////////////////////////////////////////

///////////////Global functions/////////////////////////////////////

//////////////Image settings//////////////////////////////////////
Status Edip_Screen_to_Clipboard(void){
  char buffer[3 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
    
  Data[0] = ESCLCD;
  Data[1] = 'C';
  Data[2] = 'B';

  return Edip_Send(buffer, 3, false);
}

Status Edip_Area_to_Clipboard(Point_t p1, Point_t p2){
  char buffer[7 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
    
  Data[0] = ESCLCD;
  Data[1] = 'C';
  Data[2] = 'S';
  Data[3] = p1.x;
  Data[4] = p1.y;
  Data[5] = p2.x;
  Data[6] = p2.y;
  
  return Edip_Send(buffer, 7, false);
}

Status Edip_Restore_from_Clipboard(void){
  char buffer[3 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
    
  Data[0] = ESCLCD;
  Data[1] = 'C';
  Data[2] = 'R';

  return Edip_Send(buffer, 3, false);
}

Status Edip_Copy_from_Clipboard(Point_t p1){
  char buffer[5 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
    
  Data[0] = ESCLCD;
  Data[1] = 'C';
  Data[2] = 'K';
  Data[3] = p1.x;
  Data[4] = p1.y;
   
  return Edip_Send(buffer, 5, false);
}

Status Edip_Image_from_Clipboard(Point_t p1){
  char buffer[5 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
    
  Data[0] = ESCLCD;
  Data[1] = 'U';
  Data[2] = 'C';
  Data[3] = p1.x;
  Data[4] = p1.y;
   
  return Edip_Send(buffer, 5, false);
}

