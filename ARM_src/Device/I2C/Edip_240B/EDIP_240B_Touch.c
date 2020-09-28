#include "EDIP_240B_Touch.h"
#include "EDIP_240B_Send.h"
#include <stdlib.h>

///////////////Global var/////////////////////////////////////
uint8_t last_key_event;
last_Bargraph_event_t last_Bargraph_event;
process_event_t event_edip_touch;
process_event_t event_edip_Bargraph_touch;
EDIP_Align_t Edip_label_Align = A_CENTER;
EDIP_font_t Edip_Cur_Label_Font = {.Align = A_CENTER,.Font = 0,.Zoom_X = 1,.Zoom_Y = 1,.Spacing = 0,.Angle = 0};
///////////////Prototypes/////////////////////////////////////

///////////////Global functions/////////////////////////////////////
void Edip_Touch_Init(void){
	event_edip_touch = process_alloc_event(); //Claim a event number
	event_edip_Bargraph_touch = process_alloc_event(); //Claim a event number
	return;
}

Status Edip_Send_Label_Font(EDIP_fontset_t setting, EDIP_font_t * Font){
  char buffer[5 + HDR];
  uint8_t bytes;
  char *Data; //Begin of data buffer
  Data = &buffer[DST];

  Data[0] = ESCLCD;
  Data[1] = 'A';

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
    default:
      return ERROR;
      break;
  }
  return Edip_Send(buffer, bytes, false);
}


Status Edip_Set_Label_Font(EDIP_font_t * Font){
  Status stat = SUCCESS;

  if((Font->Align != Edip_Cur_Label_Font.Align)||Edip_Refresh){
	if((Font->Align == A_LEFT)||(Font->Align == A_CENTER)||(Font->Align == A_RIGHT)){
		Edip_label_Align = Font->Align;
	}
  }
  if((Font->Font != Edip_Cur_Label_Font.Font)||Edip_Refresh){
    stat = Edip_Send_Label_Font(FONT,Font);
  }
  if((Font->Zoom_X != Edip_Cur_Label_Font.Zoom_X)||(Font->Zoom_Y != Edip_Cur_Label_Font.Zoom_Y)||Edip_Refresh){
    stat |= Edip_Send_Label_Font(ZOOM_X,Font);
  }
  if((Font->Spacing != Edip_Cur_Label_Font.Spacing)||Edip_Refresh){
    stat |= Edip_Send_Label_Font(SPACING,Font);
  }
  if((Font->Angle != Edip_Cur_Label_Font.Angle)||Edip_Refresh){
    stat |= Edip_Send_Label_Font(ANGLE, Font);
  }
  Edip_Cur_Label_Font = *Font;
  return stat;
}

Status Edip_Touch_Button(EDIP_Button_t Type, Point_t p1, Point_t p2, uint8_t press_nr, uint8_t release_nr, char * format,...){
  char buffer[60 + HDR];
  uint8_t bytes; 
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  va_list args;
  va_start (args, format);
  bytes = vsnprintf (&Data[10], 50, format, args);
  va_end (args);
  
  Data[0] = ESCLCD;
  Data[1] = 'A';
  if(Type == KEY)
  {
    Data[2] = 'T';
  }else{
    Data[2] = 'K';
  }
  Data[3] = p1.x;
  Data[4] = p1.y;
  Data[5] = p2.x;
  Data[6] = p2.y;
  Data[7] = press_nr;
  Data[8] = release_nr;
  Data[9] = Edip_label_Align;
  Data[bytes + 10] = 0x00;
  
  return Edip_Send(buffer, bytes+11, false);
}

Status Edip_Touch_Image(EDIP_Button_t Type, Point_t p1, uint8_t image_nr, uint8_t press_nr, uint8_t release_nr, char * format,...){
  char buffer[60 + HDR];
  uint8_t bytes;
  char *Data; //Begin of data buffer
  Data = &buffer[DST];

  va_list args;
  va_start (args, format);
  bytes = vsnprintf (&Data[9], 51, format, args);
  va_end (args);

  Data[0] = ESCLCD;
  Data[1] = 'A';
  if(Type == KEY)
  {
    Data[2] = 'U';
  }else{
    Data[2] = 'J';
  }
  Data[3] = p1.x;
  Data[4] = p1.y;
  Data[5] = image_nr;
  Data[6] = press_nr;
  Data[7] = release_nr;
  Data[8] = Edip_label_Align;
  Data[bytes + 9] = 0x00;

  return Edip_Send(buffer, bytes+10, false);
}

Status Edip_Touch_Set_Status(uint8_t touch_nr, bool status)
{
	char buffer[4 + HDR];
	char *Data; //Begin of data buffer
	Data = &buffer[DST];

	Data[0] = ESCLCD;
	Data[1] = 'A';
	Data[2] = 'P';
	Data[3] = touch_nr;
	Data[4] = (status == true? 1:0);

	return Edip_Send(buffer, 5, false);
}

Status Edip_Touch_graph(EDIP_BarGraph_t * Bar)
{
	char buffer[4 + HDR];
	char *Data; //Begin of data buffer
	Data = &buffer[DST];

	Data[0] = ESCLCD;
	Data[1] = 'A';
	Data[2] = 'B';
	Data[3] = Bar->BarNr;

	return Edip_Send(buffer, 4, false);
}

Status Edip_Radiogroup_Start(uint8_t groupNr)
{
	char buffer[4 + HDR];
	char *Data; //Begin of data buffer
	Data = &buffer[DST];

	Data[0] = ESCLCD;
	Data[1] = 'A';
	Data[2] = 'R';
	Data[3] = groupNr;

	return Edip_Send(buffer, 4, false);
}

Status Edip_Radiogroup_End(void){
	return 	Edip_Radiogroup_Start(0);
}

Status Exec_Touch_handler(uint8_t Key ){
  last_key_event = Key;
  process_post(PROCESS_BROADCAST, event_edip_touch, &last_key_event);
  return SUCCESS;
}

Status Exec_Bargraph_Touch_handler(uint8_t Bar_nr , uint8_t Value){
  last_Bargraph_event.Bar_nr = Bar_nr;
  last_Bargraph_event.Value = Value;
  process_post(PROCESS_BROADCAST, event_edip_Bargraph_touch, &last_Bargraph_event);
  return SUCCESS;
}

Status Edip_Delete_Touch(uint8_t touch_nr)
{
	Edip_Radiogroup_End();
	char buffer[5 + HDR];
	char *Data; //Begin of data buffer
	Data = &buffer[DST];

	Data[0] = ESCLCD;
	Data[1] = 'A';
	Data[2] = 'L';
	Data[3] = touch_nr;
	Data[4] = 0;

	return Edip_Send(buffer, 5, false);
}

Status Edip_Delete_Touch_all(void)
{
	return Edip_Delete_Touch(0);
}
