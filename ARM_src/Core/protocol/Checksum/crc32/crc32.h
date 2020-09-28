#ifndef __CRC32_H__
#define __CRC32_H__
	#include <stdint.h>

	uint32_t update_crc32 (const void *buf, uint32_t len, uint32_t crc);

	uint32_t crc32 (const void *buf, uint32_t len);
#endif //__CRC32_H__
