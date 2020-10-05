#include "EDIP_240B_Bargraph.h"
#include "EDIP_240B_Send.h"
#include <Core/Memory/list.h>

///////////////Global variables////////////////////////////////////
LIST(GraphList);
///////////////Prototypes//////////////////////////////////////////
uint8_t Edip_Find_Unused_GraphNr(void);
///////////////Global functions/////////////////////////////////////

Status Edip_New_Bar_Graph_Style(EDIP_BarGraph_Style_t * Style, EDIP_Align_t place, EDIP_BarType_t Type, EDIP_Pattern_t Pattern, uint8_t Line_width){
  if(((place == 'L')||(place == 'R')||(place == 'O')||(place == 'U')) == false){return ERROR;}
  Style->place = place;
  Style->Type = Type;
  Style->Pattern = Pattern;
  Style->Line_width = Line_width;
  
  return SUCCESS;
}

Status Edip_New_Bar_Graph(EDIP_BarGraph_t * Bar, EDIP_BarGraph_Style_t Style, Point_t p1, Point_t p2, uint8_t Low_Val, uint8_t High_Val){
  char buffer[12 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];

  Bar->BarNr = Edip_Find_Unused_GraphNr();
  
  if(Bar->BarNr > 32){return ERROR;}
  Bar->Val = 0;
  
  Data[0] = ESCLCD;
  Data[1] = 'B';
  Data[2] = Style.place;
  Data[3] = Bar->BarNr;
  Data[4] = p1.x;
  Data[5] = p1.y;
  Data[6] = p2.x;
  Data[7] = p2.y;
  Data[8] = Low_Val;
  Data[9] = High_Val;
  Data[10] = Style.Type;
  if(Style.Type < G_LINE ){
    Data[11] = Style.Pattern;
  }else{
    Data[11] = Style.Line_width;
  }
  if(Edip_Send(buffer, 12, false) == ERROR){return ERROR;};
  list_add(GraphList,Bar);
  return SUCCESS;
}

Status Edip_Bar_Graph_Value(EDIP_BarGraph_t * Bar, uint8_t Value){
  char buffer[5 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];
  
  if(Bar->Val != Value){
    Bar->Val = Value;
    Data[0] = ESCLCD;
    Data[1] = 'B';
    Data[2] = 'A';
    Data[3] = Bar->BarNr;
    Data[4] = Bar->Val;
    if(Edip_Send(buffer, 5, false) == ERROR){return ERROR;};
  }
  return SUCCESS;
}

Status Edip_Bar_Graph_Redraw(EDIP_BarGraph_t * Bar){
  char buffer[4 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];

  Data[0] = ESCLCD;
  Data[1] = 'B';
  Data[2] = 'Z';
  Data[3] = Bar->BarNr;

  return Edip_Send(buffer, 4, false);
}

Status Edip_Bar_Graph_Delete(EDIP_BarGraph_t * Bar, bool ClearScreen){
  char buffer[5 + HDR];
  char *Data; //Begin of data buffer
  Data = &buffer[DST];

  Data[0] = ESCLCD;
  Data[1] = 'B';
  Data[2] = 'D';
  Data[3] = Bar->BarNr;
  Data[4] = ClearScreen;
  
  if(Edip_Send(buffer, 5, false) == ERROR){return ERROR;}
  list_remove(GraphList, Bar);	
  return SUCCESS;
}


///////////////////////////local functions/////////////////////////
uint8_t Edip_Find_Unused_GraphNr(void)
{
  uint8_t Graph_number = 1;
  EDIP_BarGraph_t * list_p;
  list_p = list_head(GraphList);
  do{
    if(Graph_number == list_p->BarNr){
      Graph_number++;
      list_p = list_head(GraphList);
    }
  }while(list_item_next(list_p) != NULL);
  return Graph_number;
}


