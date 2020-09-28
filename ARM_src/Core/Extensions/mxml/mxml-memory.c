#include "mxml-memory.h"
#include <Core/Memory/list.h>
#include <Core/Memory/cmem.h>

LIST(Memory_list);

struct mxml_memory{
	struct mxml_memory * next;
	void * data;
};

struct list {
  struct list *next;
};

struct mxml_memory * find_in_list(void *ptr){
	struct mxml_memory * cur_list = list_head(Memory_list);
	while(cur_list != NULL){
		if(cur_list->data == ptr){
			return cur_list;
		}
		cur_list = list_item_next(cur_list);
	}
	return NULL;
}

void *m_malloc(size_t size){
	void * ptr = cmem_malloc(size);
	struct mxml_memory * data = cmem_malloc(sizeof(struct mxml_memory));
	data->data = ptr;
	list_add(Memory_list,data);
	return ptr;
}

void *m_calloc(size_t num, size_t size){
	void * ptr = cmem_malloc(num * size);
	struct mxml_memory * data = cmem_malloc(sizeof(struct mxml_memory));
	memset(ptr,0,num * size);
	data->data = ptr;
	list_add(Memory_list,data);
	return ptr;
}

void *m_realloc(void *ptr, size_t size){
	if(ptr == NULL){
		return m_malloc(size);
	}
	struct mxml_memory * data = find_in_list(ptr);
	void * ptr_data = cmem_malloc(size);
	memmove(ptr_data,ptr,size);
	data->data = ptr_data;
	cmem_free(ptr);
	return ptr_data;
}

void m_free(void *ptr){
	struct mxml_memory * data = find_in_list(ptr);
	if(ptr == NULL){ printf("cant found ptr"); return;}
	list_remove(Memory_list,data);
	cmem_free(ptr);
	cmem_free(data);
	return;
}

void m_list_lenght(void){
	printf("list_lenght: %d \r\n", list_length(Memory_list));
}
