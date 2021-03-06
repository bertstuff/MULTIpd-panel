#include "BMP_mono.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>


uint32_t read_bmp_mono(char *img,int row,char *filename,int* _w, int* _h, FRESULT * rc)
{
	FIL file;
	UINT br;

	unsigned char head[54];


     *rc = f_open( &file, filename, FA_OPEN_EXISTING | FA_READ);
     if(*rc != FR_OK)
     {
       return 0;
     }

    // BMP header is 54 bytes
    *rc = f_read (&file,head,54,&br);

    int w = head[18] + ( ((int)head[19]) << 8) + ( ((int)head[20]) << 16) + ( ((int)head[21]) << 24);
    int h = head[22] + ( ((int)head[23]) << 8) + ( ((int)head[24]) << 16) + ( ((int)head[25]) << 24);

    // lines are aligned on 4-byte boundary
    int lineSize = (w / 32) * 4;
       if( w % 32 )
          lineSize += 4;

    // skip the header
    // skip palette - two rgb quads, 8 bytes
    uint32_t bmp_row = (h - 1) - row;
    *rc = f_lseek (&file, 54 + 8 + (lineSize * bmp_row));

    // BMP header is 54 bytes
    *rc = f_read (&file,img,lineSize,&br);


    uint32_t i;
    for(i=0; i<lineSize; i++){
    		img[i] = ~img[i];
    }

    *_w = w; *_h = h;
    return br;
}
