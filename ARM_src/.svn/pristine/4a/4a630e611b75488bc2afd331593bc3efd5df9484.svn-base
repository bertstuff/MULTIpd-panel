/*
 * spi_sd_lpc17xx.h
 *
 *  Created on: 09.07.2010
 *      Author: mthomas
 */

#ifndef SPI_SD_LPC17XX_H_
#define SPI_SD_LPC17XX_H_

#include "diskio.h"

#define CT_MMC				0x01
#define CT_SD1				0x02
#define CT_SD2				0x04
#define CT_SDC				(CT_SD1|CT_SD2)
#define CT_BLOCK			0x08


void MMC_disk_timerproc(void);

DSTATUS MMC_disk_initialize(void);
DSTATUS MMC_disk_status(void);
DRESULT MMC_disk_ioctl(BYTE ctrl, void *buff);
DRESULT MMC_disk_read(BYTE* buff, DWORD sector, BYTE count);
DRESULT MMC_disk_write(const BYTE* buff, DWORD sector, BYTE count);



#endif /* SPI_SD_LPC17XX_H_ */
