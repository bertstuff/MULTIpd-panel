#ifndef BMP_MONO_H
#define BMP_MONO_H
#include <Core/protocol/FAT/ff.h>
#include <stdint.h>

/*
 * @brief monochrome bmp to array
 * @param filename filename to open
 * @param _w return width of image
 * @param _h return height of image
 * @return pointer to array. free this pointer after use
 */
uint32_t read_bmp_mono(char *img,int row,char *filename,int* _w, int* _h, FRESULT * rc);

#endif /*BMP_MONO_H*/
