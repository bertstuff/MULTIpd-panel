#include "EDIP_240B_Draw.h"
#include "EDIP_240B_Send.h"

Status Edip_Set_Line_Thickness(uint8_t Width_px ,uint8_t Height_px){
  char buffer[7 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  Data[0] = ESCLCD;
  Data[1] = 'G';
  Data[2] = 'Z';
  Data[3] = Width_px;
  Data[4] = Height_px;
   
  return Edip_Send(buffer, 5, false);
}

Status Edip_Set_Draw_mode(EDIP_Mode_t Mode){
  char buffer[7 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  if(INRANGE(Mode,1,3) == false){return ERROR;}
  
  Data[0] = ESCLCD;
  Data[1] = 'G';
  Data[2] = 'Z';
  Data[3] = Mode;
   
  return Edip_Send(buffer, 5, false);
}

Status Edip_Draw_Rectangle(Point_t p1, Point_t p2){
  char buffer[7 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  Data[0] = ESCLCD;
  Data[1] = 'G';
  Data[2] = 'R';
  Data[3] = p1.x;
  Data[4] = p1.y;
  Data[5] = p2.x;
  Data[6] = p2.y;
    
  return Edip_Send(buffer, 7, false);
}

Status Edip_Draw_Fill_Rectangle(Point_t p1, Point_t p2){
  char buffer[7 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];

  Data[0] = ESCLCD;
  Data[1] = 'R';
  Data[2] = 'S';
  Data[3] = p1.x;
  Data[4] = p1.y;
  Data[5] = p2.x;
  Data[6] = p2.y;

  return Edip_Send(buffer, 7, false);
}

Status Edip_Draw_Line(Point_t p1, Point_t p2){
  char buffer[7 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  Data[0] = ESCLCD;
  Data[1] = 'G';
  Data[2] = 'D';
  Data[3] = p1.x;
  Data[4] = p1.y;
  Data[5] = p2.x;
  Data[6] = p2.y;
    
  return Edip_Send(buffer, 7, false);
}

Status Edip_Draw_Multipoint_line(Point_t * p_array, uint8_t points){
  uint8_t i;
  char buffer[5 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  Edip_Draw_Line(p_array[0], p_array[1]);
  
  for(i = 2; i < points; i++){
    Data[0] = ESCLCD;
    Data[1] = 'G';
    Data[2] = 'W';
    Data[3] = p_array[i].x;
    Data[4] = p_array[i].y; 
    if(Edip_Send(buffer, 5, false) == ERROR){
      return ERROR;
    }
  }
  
  return SUCCESS;
}

Status Edip_Draw_Point(Point_t p1){
  char buffer[7 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  Data[0] = ESCLCD;
  Data[1] = 'G';
  Data[2] = 'P';
  Data[3] = p1.x;
  Data[4] = p1.y;
    
  return Edip_Send(buffer, 5, false);
}
