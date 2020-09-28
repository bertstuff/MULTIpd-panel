#include <stdint.h>
#include <stdbool.h>

#ifndef __FLASH_MEM_H__
#define __FLASH_MEM_H__

/*
 * @brief load flash settings to ram. if no settings are found default settings will be loaded.
 * @Ram_mem pointer to settings structure (first element in structure must be uint32_t MemCheck; )
 * @Def_Ram_mem pointer to a structure with the default values ( same struct type as Ram_mem)
 * @size size of Ram_mem structure
 */
void Flash_mem_init(void * Ram_mem,void * Def_Ram_mem, uint32_t size);

/*
 * @brief save ram to flash
 */
bool Flash_mem_save(void);

/*
 * @brief enable save ram to flash on powerfail (only enable when you have a stable power supply)
 */
void Enable_BOD(void);

#endif //__FLASH_MEM_H__
