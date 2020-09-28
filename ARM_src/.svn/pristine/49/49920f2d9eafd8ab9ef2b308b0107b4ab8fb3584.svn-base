#include <Board/Def_config.h>
#if(BOARD_SCREEN == 1)

#include "LCD_Alphanumeric.h"
#include <Driver/include/lpc17xx_gpio.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <Core/Timer/etimer.h>
#include <Device/I2C/8574/I2C_8574.h>
#include <Core/Peripherals/I2C_Dev_Init.h>
#include <Screen/SCR_Init.h>


#ifndef LCD_ALPHANUMERIC_SETTINGS
#define LCD_ALPHANUMERIC_SETTINGS
	#define LCD_DB0_PORT 2
	#define LCD_DB0_PIN (1<<8)
	#define LCD_DB1_PORT 2
	#define LCD_DB1_PIN (1<<7)
	#define LCD_DB2_PORT 2
	#define LCD_DB2_PIN (1<<6)
	#define LCD_DB3_PORT 2
	#define LCD_DB3_PIN (1<<5)
	#define LCD_DB4_PORT 2
	#define LCD_DB4_PIN (1<<4)
	#define LCD_DB5_PORT 2
	#define LCD_DB5_PIN (1<<3)
	#define LCD_DB6_PORT 2
	#define LCD_DB6_PIN (1<<2)
	#define LCD_DB7_PORT 2
	#define LCD_DB7_PIN (1<<1)
	#define LCD_E_PORT 2
	#define LCD_E_PIN (1<<9)
	#define LCD_RS_PORT 2
	#define LCD_RS_PIN (1<<12)
	#define LCD_RW_PORT 2
	#define LCD_RW_PIN (1<<11)
#endif

#define LCD_I2C_DB0_PIN 0
#define LCD_I2C_DB1_PIN 1
#define LCD_I2C_DB2_PIN 2
#define LCD_I2C_DB3_PIN 3
#define LCD_I2C_E_PIN 6
#define LCD_I2C_RS_PIN 5
#define LCD_I2C_RW_PIN 4


#define f_CLEAR_DISPLAY (1<<0)
#define f_RETURN_HOME (1<<1)
#define f_ENTRY_MODE_SET (1<<2)
#define f_DISPLAY_CONTROL (1<<3)
#define f_CURSOR_SHIFT (1<<4)
#define f_FUNCTION_SET (1<<5)
#define f_SET_CGRAM (1<<6)
#define f_SET_DDRAM (1<<7)

#define _BV(DATA,BIT) (DATA & ( 1<<BIT ))? true:false

#define SCROLL_ARRAY 4
#define SIZE_X 0
#define SIZE_Y 1

/*variables*/
char Displ_data[2][16];
LCD_scroll_text Scroll_data[SCROLL_ARRAY];
bool blink;
uint16_t blink_data[2];
Point_t Curcor_pos;
int16_t lcd_addr; //if < 0 than gpio device
uint8_t lcd_i2c_data;
LCD_function_set_t LCD_settings;
uint8_t lcd_size[2] = {16, 2};

/*prototype*/
PROCESS(LCD_Alphanumeric_process,"LCD_Alphanumeric process");
void LCD_Alphanumeric_Clear_scroll_data(void);
void LCD_Alphanumeric_Scroll_handler(bool first_build);
Status LCD_Alphanumeric_Draw_Char(uint8_t character);
Status LCD_Alphanumeric_Send(LCD_data_pack data);
Status LCD_Alphanumeric_GPIO_Send(LCD_data_pack data);
Status LCD_Alphanumeric_I2C_Send(LCD_data_pack data);
Status LCD_Alphanumeric_Init_common(LCD_function_set_t settings);
Status LCD_Alphanumeric_Function_Set(LCD_function_set_t settings);



/*Local functions*/
void LCD_Alphanumeric_Clear_scroll_data(void){
	uint8_t i;
	for(i = 0; i<SCROLL_ARRAY ;i++){
		if(Scroll_data[i].used == true){
			free(Scroll_data[i].text);
			Scroll_data[i].used = false;
		}
	}
}

void LCD_Alphanumeric_Scroll_handler(bool first_build){
	uint8_t i;
	uint8_t lenght;
	Point_t Prev_Curcor_pos;

	Prev_Curcor_pos= Curcor_pos;
	char buffer[200];
	char lcd_buffer[17];
	for(i = 0;i<SCROLL_ARRAY;i++){
		if(Scroll_data[i].used ==false){
			break;
		}

		lenght = Scroll_data[i].lenght;
		if(Scroll_data[i].lenght > (lcd_size[SIZE_X] - Scroll_data[i].pos.x)){
			lenght =  lcd_size[SIZE_X] - Scroll_data[i].pos.x;
		}
		lenght++;

		if(strlen(Scroll_data[i].text)<=Scroll_data[i].lenght){
			//Don't scroll text
			if(first_build == true){
				snprintf(lcd_buffer,lenght,"%s",Scroll_data[i].text);
				LCD_Alphanumeric_Draw_String(Scroll_data[i].pos,"%s",lcd_buffer);
				Scroll_data[i].scroll_pos = 0;
			}
			continue;
		}

		//Scroll text
		snprintf(buffer,200,"%s     %s",Scroll_data[i].text,Scroll_data[i].text);
		snprintf(lcd_buffer,lenght,"%s",&buffer[Scroll_data[i].scroll_pos]);
		LCD_Alphanumeric_Draw_String(Scroll_data[i].pos,"%s",lcd_buffer);
		LCD_Alphanumeric_Set_Cursor_addr(Prev_Curcor_pos);
		Scroll_data[i].scroll_pos++;


		if(Scroll_data[i].scroll_pos > (strlen(Scroll_data[i].text)+4)){
			Scroll_data[i].scroll_pos = 0;
		}
	}
	return;
}
void LCD_Alphanumeric_blink_handler(void){
	Point_t pos = {.x = 0,.y = 0};

	if((blink_data[0] == 0)&&(blink_data[1] == 0)){
		return;
	}

	blink = !blink; //toggle blink

	for(pos.y=0; pos.y<1; pos.y++){
		LCD_Alphanumeric_Set_Cursor_addr(pos);
		for(pos.x=0; pos.x<lcd_size[SIZE_X]; pos.x++){
			if(blink_data[pos.y] & (1 << pos.x)){
				if(blink == true){
					LCD_Alphanumeric_Draw_Char(Displ_data[pos.y][pos.x]);
				}else{
					LCD_Alphanumeric_Draw_Char(' ');
				}
			}else{
				LCD_Alphanumeric_Display_Shift(SHIFT_CURCOR,DIR_RIGHT);
			}
		}
	}

}

PROCESS_THREAD(LCD_Alphanumeric_process, ev, data){
	static struct etimer Poll_tm;

	if(etimer_expired(&Poll_tm)){
		etimer_set(&Poll_tm,CLOCK_SECOND/2);
	}

	PROCESS_BEGIN();
	while (ev != PROCESS_EVENT_EXIT){
		PROCESS_WAIT_EVENT();
		if(ev == PROCESS_EVENT_TIMER){
			LCD_Alphanumeric_Scroll_handler(false);
			LCD_Alphanumeric_blink_handler();
		}
	}
	PROCESS_END();
}

void LCD_Alphanumeric_Delay(uint16_t us){
	uint32_t i ,us_timer;

	//This buzzy waiting delay is not very precise
	//us_timer = us * 14; //(6 clock cycles for one loop at 100MHZ)
	us_timer = us * 140;
	for(i = 0; i < us_timer; i++){
	 __asm volatile
	  (
	    "nop \n"
	    "nop \n"
	    "nop \n"
	    "nop \n"
	    "nop \n"
		"nop \n"
	  );
	}
	  return;
}

/* global functions*/
Status LCD_Alphanumeric_Clear_Display(void){
	LCD_data_pack data;
	LCD_Alphanumeric_Cursor_Home();
	LCD_Alphanumeric_Entry_Mode(SHIFT_CURCOR,DIR_RIGHT);
	LCD_Alphanumeric_Function_Set(LCD_settings);
	LCD_Alphanumeric_Display_Control(true, false, false);

	LCD_Alphanumeric_Clear_scroll_data();
	blink_data[0] = 0;
	blink_data[1] = 0;
	data.RS = 0;
	data.RW = 0;
	data.data = f_CLEAR_DISPLAY;
	data.delay = 1640;
	return LCD_Alphanumeric_Send(data);
}

Status LCD_Alphanumeric_Cursor_Home(){
	LCD_data_pack data;
	data.RS = 0;
	data.RW = 0;
	data.data = f_RETURN_HOME;
	data.delay = 1640;
	return LCD_Alphanumeric_Send(data);
}

Status LCD_Alphanumeric_Entry_Mode(LCD_Shift_Mode Mode,LCD_Direction Dir){
	LCD_data_pack data;
	data.RS = 0;
	data.RW = 0;
	data.data = (f_ENTRY_MODE_SET|(Dir << 1)|(Mode << 0));
	data.delay = 40;
	return LCD_Alphanumeric_Send(data);
}

Status LCD_Alphanumeric_Display_Control(bool Display_on, bool Cursor_on, bool Cursor_blink){
	LCD_data_pack data;
	data.RS = 0;
	data.RW = 0;
	data.data = (f_DISPLAY_CONTROL | ((Display_on?1:0) << 2) | ((Cursor_on?1:0) << 1) | ((Cursor_blink?1:0) << 0));
	data.delay = 40;
	return LCD_Alphanumeric_Send(data);
}

Status LCD_Alphanumeric_Display_Shift(LCD_Shift_Mode Mode,LCD_Direction Dir){
	LCD_data_pack data;
	data.RS = 0;
	data.RW = 0;
	data.data = (f_CURSOR_SHIFT|(Mode << 3)|(Dir << 2));
	data.delay = 40;
	return LCD_Alphanumeric_Send(data);
}

Status LCD_Alphanumeric_Function_Set(LCD_function_set_t settings){
	LCD_data_pack data;

	if((settings.Fontsize == 11)||(settings.Fontsize == 10)||(settings.Fontsize == 1))
		settings.Fontsize = 1;
	else
		settings.Fontsize = 0;

	data.RS = 0;
	data.RW = 0;
	data.data = (f_FUNCTION_SET|((settings.Data_bits == 8)?(1<<4):0)|((settings.Data_lines == 2)?(1<<3):0)|(settings.Fontsize << 2));
	data.delay = 40;
	return LCD_Alphanumeric_Send(data);
}

Status LCD_Alphanumeric_Set_CGRAM_addr(uint8_t mem_loc,uint8_t line){
	LCD_data_pack data;
	data.RS = 0;
	data.RW = 0;
	data.data = f_SET_CGRAM | ((mem_loc & 0x7)<<3) | (line & 0x7);
	data.delay = 40;
	return LCD_Alphanumeric_Send(data);
}

//LCD_Alphanumeric_Set_DDRAM_addr
Status LCD_Alphanumeric_Set_Cursor_addr(Point_t pos){
	LCD_data_pack data;
	data.RS = 0;
	data.RW = 0;
	data.data = f_SET_DDRAM | (pos.x + (pos.y * 0x40));
	data.delay = 40;
	Curcor_pos = pos;
	return LCD_Alphanumeric_Send(data);
}

Status LCD_Alphanumeric_Draw_Char(uint8_t character){
	LCD_data_pack data;

	data.RS = 1;
	data.RW = 0;
	data.data = character;
	data.delay = 40;
	return LCD_Alphanumeric_Send(data);
}

Status LCD_Alphanumeric_Load_Symbol(uint8_t mem_loc,const uint8_t symbol[8]){
	uint8_t i = 0;
	for(i=0;i<8;i++){
		if(LCD_Alphanumeric_Set_CGRAM_addr(mem_loc,i) == ERROR){return ERROR;}
		if(LCD_Alphanumeric_Draw_Char(symbol[i]) == ERROR){return ERROR;}
	}
	return SUCCESS;
}

uint8_t LCD_Alphanumeric_parse_format_specifier(char * buffer,Point_t * pos){
	 char * specifier;
	 char * search;
	 uint8_t i;
	 LCD_scroll_text * scroll;

	 buffer++; //skip % char
	 specifier = buffer;

	 while(1){ //find specifier
		 if(isdigit(specifier[0])){
			 specifier++;
		 }else{
			 break;
		 }
	 }


	 switch(specifier[0]){
	 case '#'://Scroll specifier
		 i = 0;
		 while(1){
			 if(Scroll_data[i].used == false){//find empty place in scroll array
				 break;
			 }else{
				 if((Scroll_data[i].pos.x == pos->x)&&(Scroll_data[i].pos.y == pos->y)){//Replace scrolling text on the same place
					 free(Scroll_data[i].text);
					 break;
				 }
			 }
			 i++;
			 if(i == SCROLL_ARRAY){
				 return 0;
			 }
		 }
		 scroll = &Scroll_data[i];

		 //set scroll settings
		 specifier[0] = '\0';
		 scroll->lenght = atoi(buffer);
		 scroll->used = true;
		 scroll->scroll_pos = 0;
		 search = specifier;
		 //copy text to scroll buffer
		 while(1){
			search++;
			if(search[0] == '\0'){break;}
			if(!((search[0] == '#')&&(search[1] == '>'))){continue;}//search end off scroll text
			scroll->pos = *pos;
			search[0] = '\0';
			scroll->text = malloc(strlen(specifier+1));
			strcpy (scroll->text,specifier+1);
			pos->x += scroll->lenght;
			LCD_Alphanumeric_Scroll_handler(true);
			return ((search + 1) - (buffer - 1));
		 }
		 break;
	 case '~':
		 i = 0;
		 search = specifier;
		 while(1){
			search++;
			if(search[0] == '\0'){break;}
			if((search[0] == '~')&&(search[1] == '>')){break;}//search end off blink text
			blink_data[pos->y] |= (1 << (pos->x + i));
			i++;
		 }
		 return 1;
		 break;
	 default:
		 break;
	 }

	 return 0;
}

Status LCD_Alphanumeric_Draw_String(Point_t pos, char * format,...){
	 char buffer[100];
	 uint8_t bytes, i, skip;

	 //Set position
	 if(LCD_Alphanumeric_Set_Cursor_addr(pos) == ERROR){return ERROR;}

	 //Draw string
	 va_list args;
	 va_start (args, format);
	 bytes = vsnprintf(buffer, 100, format, args);
	 va_end (args);
	 for(i = 0; i < bytes; i++){
		 switch(buffer[i]){
		 case '\n':
			 pos.y++;
			 LCD_Alphanumeric_Set_Cursor_addr(pos);
			 break;
		 case '\r':
			 pos.x = 0;
			 LCD_Alphanumeric_Set_Cursor_addr(pos);
			 break;
		 case '<':
			 skip = LCD_Alphanumeric_parse_format_specifier(&buffer[i],&pos);
			 i += skip;
			 LCD_Alphanumeric_Set_Cursor_addr(pos);
			 if(skip == 0){
				goto add_char;
			 }
			 break;
		 case '~':
		 case '#':
			 if(buffer[i+1]!='>'){
				 goto add_char;
			 }
			 i++;
			 break;
		 case C_EURO:
			 buffer[i] = 1;
		 default:
			 add_char:
			 LCD_Alphanumeric_Draw_Char(buffer[i]);
			 if((pos.x < lcd_size[SIZE_X]) && (pos.y < lcd_size[SIZE_Y])){
				 Displ_data[pos.y][pos.x] = buffer[i];
			 }
			 pos.x++;
			 if(pos.x > (lcd_size[SIZE_X]-1)){ pos.x = (lcd_size[SIZE_X]-1);}
			 break;
		 }
	 }
	Curcor_pos = pos;
	return SUCCESS;
}

Status LCD_Alphanumeric_Send(LCD_data_pack data){
	if(lcd_addr == GPIO_ADDR){
		return LCD_Alphanumeric_GPIO_Send(data);
	}else{
		return LCD_Alphanumeric_I2C_Send(data);
	}
}


/////////////////////////gpio functions//////////////////////////////////////
void LCD_Alphanumeric_GPIO(uint8_t port, uint32_t pin, bool state){
	if(state == true){
		GPIO_SetValue(port,pin);
	}else{
		GPIO_ClearValue(port,pin);
	}
}


Status LCD_Alphanumeric_GPIO_Send(LCD_data_pack data){
	LCD_Alphanumeric_GPIO(LCD_RS_PORT,LCD_RS_PIN,data.RS);
	LCD_Alphanumeric_GPIO(LCD_RW_PORT,LCD_RW_PIN,data.RW);
	LCD_Alphanumeric_GPIO(LCD_E_PORT,LCD_E_PIN,1);
	LCD_Alphanumeric_Delay(5);
	LCD_Alphanumeric_GPIO(LCD_DB0_PORT,LCD_DB0_PIN,_BV(data.data,0));
	LCD_Alphanumeric_GPIO(LCD_DB1_PORT,LCD_DB1_PIN,_BV(data.data,1));
	LCD_Alphanumeric_GPIO(LCD_DB2_PORT,LCD_DB2_PIN,_BV(data.data,2));
	LCD_Alphanumeric_GPIO(LCD_DB3_PORT,LCD_DB3_PIN,_BV(data.data,3));
	LCD_Alphanumeric_GPIO(LCD_DB4_PORT,LCD_DB4_PIN,_BV(data.data,4));
	LCD_Alphanumeric_GPIO(LCD_DB5_PORT,LCD_DB5_PIN,_BV(data.data,5));
	LCD_Alphanumeric_GPIO(LCD_DB6_PORT,LCD_DB6_PIN,_BV(data.data,6));
	LCD_Alphanumeric_GPIO(LCD_DB7_PORT,LCD_DB7_PIN,_BV(data.data,7));
	LCD_Alphanumeric_Delay(5);
	LCD_Alphanumeric_GPIO(LCD_E_PORT,LCD_E_PIN,0);
	LCD_Alphanumeric_Delay(data.delay);
	return SUCCESS;
}

void LCD_Alphanumeric_GPIO_Init(void){

	GPIO_SetDir(LCD_DB0_PORT,LCD_DB0_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_DB1_PORT,LCD_DB1_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_DB2_PORT,LCD_DB2_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_DB3_PORT,LCD_DB3_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_DB4_PORT,LCD_DB4_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_DB5_PORT,LCD_DB5_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_DB6_PORT,LCD_DB6_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_DB7_PORT,LCD_DB7_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_RS_PORT,LCD_RS_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_RW_PORT,LCD_RW_PIN,DIR_OUTPUT);
	GPIO_SetDir(LCD_E_PORT,LCD_E_PIN,1);

	lcd_addr = GPIO_ADDR;

	LCD_settings.Data_bits = 8;
	LCD_settings.Data_lines = 2;
	LCD_settings.Fontsize = 11;
	process_start(&LCD_Alphanumeric_process,NULL);
	LCD_Alphanumeric_Delay(10);

	LCD_Alphanumeric_Clear_Display();
	return;
}
/*global functions */

void LCD_Alphanumeric_select(uint8_t addr){
	lcd_addr = addr;
	return;
}

/////////////////////////I2C functions//////////////////////////////////////
void LCD_Alphanumeric_I2C_IO(uint32_t pin, bool state){
	if(state == true){
		lcd_i2c_data |= (1<<pin);
	}else{
		lcd_i2c_data &= ~(1<<pin);
	}
}


Status LCD_Alphanumeric_I2C_Send(LCD_data_pack data){
	LCD_Alphanumeric_I2C_IO(LCD_I2C_RS_PIN,data.RS);
	LCD_Alphanumeric_I2C_IO(LCD_I2C_RW_PIN,data.RW);
	LCD_Alphanumeric_I2C_IO(LCD_I2C_E_PIN,1);
	I2C_8574_WriteOutput (lcd_addr,&lcd_i2c_data, 0x00);
	LCD_Alphanumeric_I2C_IO(LCD_I2C_DB0_PIN,_BV(data.data,4));
	LCD_Alphanumeric_I2C_IO(LCD_I2C_DB1_PIN,_BV(data.data,5));
	LCD_Alphanumeric_I2C_IO(LCD_I2C_DB2_PIN,_BV(data.data,6));
	LCD_Alphanumeric_I2C_IO(LCD_I2C_DB3_PIN,_BV(data.data,7));
	I2C_8574_WriteOutput (lcd_addr,&lcd_i2c_data, 0x00);
	LCD_Alphanumeric_I2C_IO(LCD_I2C_E_PIN,0);
	I2C_8574_WriteOutput (lcd_addr,&lcd_i2c_data, 0x00);
	LCD_Alphanumeric_I2C_IO(LCD_I2C_E_PIN,1);
	I2C_8574_WriteOutput (lcd_addr,&lcd_i2c_data, 0x00);
	LCD_Alphanumeric_I2C_IO(LCD_I2C_DB0_PIN,_BV(data.data,0));
	LCD_Alphanumeric_I2C_IO(LCD_I2C_DB1_PIN,_BV(data.data,1));
	LCD_Alphanumeric_I2C_IO(LCD_I2C_DB2_PIN,_BV(data.data,2));
	LCD_Alphanumeric_I2C_IO(LCD_I2C_DB3_PIN,_BV(data.data,3));
	I2C_8574_WriteOutput (lcd_addr,&lcd_i2c_data, 0x00);
	LCD_Alphanumeric_I2C_IO(LCD_I2C_E_PIN,0);
	I2C_8574_WriteOutput (lcd_addr,&lcd_i2c_data, 0x00);
	LCD_Alphanumeric_Delay(100);
	LCD_Alphanumeric_Delay(data.delay);
	return SUCCESS;
}

Status LCD_Alphanumeric_I2C_Init(uint8_t addr){
	Status i2c_stat;

	I2C_Dev_Init(LCD_MAX_SPEED_HZ);
	lcd_addr = addr;
	lcd_i2c_data = 0x00;
	i2c_stat = I2C_8574_WriteOutput (lcd_addr,&lcd_i2c_data, 0x00);
	if(i2c_stat == ERROR){return ERROR;}
	LCD_settings.Data_bits = 4;
	LCD_settings.Data_lines = 2;
	LCD_settings.Fontsize = 11;
	process_start(&LCD_Alphanumeric_process,NULL);
	LCD_Alphanumeric_Delay(10);
	LCD_Alphanumeric_Clear_Display();
	return SUCCESS;
}


Status LCD_Alphanumeric_Init(uint8_t addr){
	lcd_addr = addr;
	if(addr == GPIO_ADDR){
		LCD_Alphanumeric_GPIO_Init();
		return SUCCESS;
	}else{
		return LCD_Alphanumeric_I2C_Init(addr);
	}
}

uint8_t LCD_Alphanumeric_addr(void){
	return lcd_addr;
}
#endif
