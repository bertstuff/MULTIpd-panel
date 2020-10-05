#ifndef _MXML_MEMORY_
#define _MXML_MEMORY_

	#include <stdio.h>
	#include <stdint.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include <string.h>

	void *m_calloc(size_t num, size_t size);
	void *m_realloc(void *ptr, size_t size);
	void m_free(void *ptr);
	void *m_malloc(size_t size);
	void m_list_lenght(void);

#endif
