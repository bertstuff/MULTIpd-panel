#include "EDIP_240B_Text.h"
#include "EDIP_240B_Send.h"

EDIP_Align_t Edip_Align;
EDIP_font_t Edip_Cur_Font;

///////////////Prototype/////////////////////////////////////////////
Status Edip_Send_Font(EDIP_fontset_t setting, EDIP_font_t * Font);
///////////////Global functions/////////////////////////////////////
void Edip_New_Font(EDIP_font_t * Font){
    Font->Align = A_LEFT;
    Font->Font = 0;
    Font->Zoom_X = 1;
    Font->Zoom_Y = 1;
    Font->Spacing = 2;
    Font->Angle = R_0DEG;
    Font->Mode = M_REPLACE;
    Font->Blink = B_OFF;
}

Status Edip_Change_Font_Setting(EDIP_font_t * Font, EDIP_fontset_t setting, uint8_t value){
  
  switch(setting){
    case ALIGN:
      if(! ((value == 'L') || (value == 'C') || (value == 'R'))){return ERROR;}
      Font->Align = (EDIP_Align_t ) value;
      break;
    case FONT:
      if(INRANGE(value,0,16) == false){return ERROR;}
      Font->Font = (uint8_t ) value;
      break;
    case ZOOM_X:
      if(INRANGE(value,1,4) == false){return ERROR;}
      Font->Zoom_X = (uint8_t ) value;
      break;
    case ZOOM_Y:
      if(INRANGE(value,1,4) == false){return ERROR;}
      Font->Zoom_Y = (uint8_t ) value;
      break;      
    case SPACING:
      Font->Spacing = (uint8_t ) value;
      break;
    case ANGLE:
      if(INRANGE(value,0,1) == false){return ERROR;}
      Font->Angle = (EDIP_Rotate_t ) value;
      break;
    case MODE:
      if(INRANGE(value,1,5) == false){return ERROR;}
      Font->Mode = (EDIP_Mode_t ) value;
      break;
    case BLINK:
      if(INRANGE(value,0,2) == false){return ERROR;}
      Font->Blink = (EDIP_Blink_t ) value;
      break;
    default:
      return ERROR;
      break;
  }
  return SUCCESS;
}

Status Edip_Set_Font(EDIP_font_t * Font){
  Status stat = SUCCESS;
  
  if((Font->Align != Edip_Cur_Font.Align)||Edip_Refresh){
    Edip_Align = Font->Align;
  }
  if((Font->Font != Edip_Cur_Font.Font)||Edip_Refresh){
    stat = Edip_Send_Font(FONT,Font);
  }
  if((Font->Zoom_X != Edip_Cur_Font.Zoom_X)||(Font->Zoom_Y != Edip_Cur_Font.Zoom_Y)||Edip_Refresh){
    stat |= Edip_Send_Font(ZOOM_X,Font); 
  }  
  if((Font->Spacing != Edip_Cur_Font.Spacing)||Edip_Refresh){
    stat |= Edip_Send_Font(SPACING,Font); 
  }  
  if((Font->Angle != Edip_Cur_Font.Angle)||Edip_Refresh){
    stat |= Edip_Send_Font(ANGLE, Font);   
  }
  if((Font->Mode != Edip_Cur_Font.Mode)||Edip_Refresh){
    stat |= Edip_Send_Font(MODE, Font);   
  }  
  if((Font->Blink != Edip_Cur_Font.Blink)||Edip_Refresh){
    stat |= Edip_Send_Font(BLINK, Font);   
  }
  Edip_Cur_Font = *Font;
  return stat;
}

///////////////////string functions////////////////////////////////
Status Edip_Draw_String(Point_t p1, char * format,...){
  char buffer[100 + HDR];
  uint8_t bytes; 
  char * Data; //Begin of data buffer
  char * find_ptr;
  Data = &buffer[DST];
  

  va_list args;
  va_start (args, format);
  bytes = vsnprintf (&Data[5], 100, format, args);
  find_ptr = &Data[5];
  while(1){
	  find_ptr = strchr ( find_ptr,'\r');
	  if(find_ptr == NULL){
		  break;
	  }else{
		memmove(find_ptr,find_ptr+1,strlen(find_ptr));
	  }
  }
  find_ptr = &Data[5];
  while(1){
	  find_ptr = strchr( find_ptr,'\n');
			  if(find_ptr == NULL){
				  break;
			  }else{
				  *find_ptr = '|';
			  }
  }
  va_end (args);

  Data[0] = ESCLCD;
  Data[1] = 'Z';
  Data[2] = Edip_Align;
  Data[3] = p1.x;
  Data[4] = p1.y;
  Data[bytes + 5] = 0x00;

  return Edip_Send(buffer, bytes+6, false);
}

Status Edip_Draw_String_va_list(Point_t p1, char * format, va_list args){
  char buffer[150 + HDR];
  uint8_t bytes;
  char *Data; //Begin of data buffer
  Data = &buffer[DST];

  bytes = vsnprintf (&Data[5], 90, format, args);

  Data[0] = ESCLCD;
  Data[1] = 'Z';
  Data[2] = Edip_Align;
  Data[3] = p1.x;
  Data[4] = p1.y;
  Data[bytes + 5] = 0x00;

  return Edip_Send(buffer, bytes+6, false);
}

//////////////////////////////local functions//////////////////////

Status Edip_Send_Font(EDIP_fontset_t setting, EDIP_font_t * Font){
  char buffer[5 + HDR];
  uint8_t bytes;
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  Data[0] = ESCLCD;
  Data[1] = 'Z';
  
  switch(setting){
    case FONT:
      Data[2] = 'F';
      Data[3] = (char)Font->Font;
      bytes = 4;
      break;
    case ZOOM_X:
    case ZOOM_Y:
      Data[2] = 'Z';
      Data[3] = (char)Font->Zoom_X; //Zoom_X
      Data[4] = (char)Font->Zoom_Y; //Zoom_Y
      bytes = 5;
      break;      
    case SPACING:
      Data[2] = 'Y';
      Data[3] = (char)Font->Spacing;
      bytes = 4;
      break;
    case ANGLE:
      Data[2] = 'W';
      Data[3] = (char)Font->Angle;
      bytes = 4;
      break;
    case MODE:
      Data[2] = 'V';
      Data[3] = (char)Font->Mode;
      bytes = 4;
      break;
    case BLINK:
      Data[2] = 'B';
      Data[3] = (char)Font->Blink;
      bytes = 4;
      break;
    default:
      return ERROR;
      break;
  }
  return Edip_Send(buffer, bytes, false);
}
