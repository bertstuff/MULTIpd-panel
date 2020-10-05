
#include <cr_section_macros.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CMEM_SIZE 0x4000
#define MEMORY_MASK 0xFFFFFFFE
#define USED_MASK 0x01

//memory allocation for momory system
__BSS(RAM2) static char memory[CMEM_SIZE];
//array with begin pointers of eache block. first bit is used to memorize is the block is used
void * mem_block[500];
//variable for debugging. maximum allocated blocks may not exceed the size of mem_block
uint16_t max_block = 0;


void cmem_init(void){
	uint16_t i;
	uint8_t block = 1;
	uint32_t memsize = CMEM_SIZE;
	uint32_t memplace = 0;

	//init memory in large blocks of a power 2
	mem_block[0] = (void *)((int)(&memory[0]) & MEMORY_MASK);
	for(i=0; i < 31; i++){
		if(memsize & (1 << i)){
			memplace += (1 << i);
			mem_block[block] = (void *)((int)(&memory[memplace]) & MEMORY_MASK);
			block++;
		}
	}
}

bool cmem_split_block(uint16_t blk_sel, uint16_t blk_size){
	uint16_t used_block = blk_sel;

	//check how many blocks are defined in the mem_block array
	while(mem_block[used_block]){used_block++;}
	//check if there is space in the mem_block array for another block
	if(used_block > sizeof(mem_block)-2){
		printf("CMEM: no block available!\r\n");
		return false;
	}

	//save used blocks for debugging propose
	if(max_block < used_block){
		max_block = used_block;
	}

	//insert a new block in the mem_block array
	memmove(&mem_block[blk_sel + 2],&mem_block[blk_sel+1],(used_block-blk_sel)* sizeof(void *));
	if(blk_size % 2){
		printf("wrong block size\r\n");
	}
	mem_block[blk_sel + 1] = mem_block[blk_sel] + (blk_size/2);
	return true;
}

void cmem_merge_block(uint16_t blk_sel){
	uint16_t used_block = blk_sel;
	//check how many blocks are defined in the mem_block array
	while(mem_block[used_block]){used_block++;}
	//remove selected block in mem_block array
	memmove(&mem_block[blk_sel+1],&mem_block[blk_sel+2],((used_block-blk_sel)-1)* sizeof(void *));
	return;
}

void * cmem_malloc(size_t size){
	uint32_t new_block_size = 0;
	uint8_t i;

	//calc needed block size thats a power of 2
	for(i=31; i >= 0; i--){
		if(size & (1 << i)){
			if(new_block_size == 0){
				new_block_size = (1 << i);
			}else{
				new_block_size = (new_block_size << 1);
				break;
			}
		}
	}
	if(new_block_size <= 1){new_block_size = 2;}//minimum block size is two. (first bit is used for the 'memory used' bit )

	//loop to find free memory space
	uint16_t blk_sel = 0;
	uint16_t blk_size = 0;
	while(1){
		//last block pointer has value NULL. return fail when reached end of memory
		if((mem_block[blk_sel] == 0)||(mem_block[blk_sel+1] == 0)){

			printf("CMEM: no memory available!\r\n");
			return NULL;
		}
		//calculate size of selected block. (Difference between pointer to current block and next block )
		blk_size = (((int)mem_block[blk_sel+1] & MEMORY_MASK)-((int)mem_block[blk_sel] & MEMORY_MASK));
		if(blk_size == new_block_size){
			//check if memory is in use
			if((int)(mem_block[blk_sel]) & USED_MASK){
				blk_sel++; //block is in use. increase block selection and loop again
			}else{
				mem_block[blk_sel]++; //block is perfect. set used bit
				return  (void *)(((int)mem_block[blk_sel] & MEMORY_MASK)+((int)memory & USED_MASK)); //return memory place minus the used bit
			}
		}else if(blk_size < new_block_size){
			blk_sel++;//block is to small. increase block selection and loop again
		}else{
			//block is to big
			if((int)(mem_block[blk_sel]) & USED_MASK){
				blk_sel++; //block is in use. increase block selection and loop again
			}else{
				//block is not in use but is to big. let's split it up in two.
				if(cmem_split_block(blk_sel,blk_size) == false){
					return NULL;
				}
			}
		}
	}
}

void cmem_free(void * ptr){
	uint16_t blk_sel = 0;
	uint16_t blk_size = 0;
	uint32_t offset = 0;
	while(mem_block[blk_sel] != NULL){
		//find memory block in mem_block array
		if((((int)mem_block[blk_sel] & MEMORY_MASK)+((int)memory & USED_MASK)) == (int)ptr){
			//disable used bit
			if(!((int)(mem_block[blk_sel]) & USED_MASK)){
				printf("CMEM: Pointer already free %x\r\n",ptr);
				return;
			}
			mem_block[blk_sel]--;
			while(1){
				//calc the offset of the selected block from the beginning of the memory
				offset = (((int)mem_block[blk_sel] & MEMORY_MASK)-((int)mem_block[0] & MEMORY_MASK));
				//calc the size of the selected block
				blk_size = (((int)mem_block[blk_sel+1] & MEMORY_MASK)-((int)mem_block[blk_sel] & MEMORY_MASK));
				//check if the block place is dividable with a block one size bigger (power 2). To determine if we have to merge the block before of after the selected block
				if((offset%(blk_size<<1))==0){
					//if the block after the selected block is equal in size and not used merge the blocks
					if(blk_size == (((int)mem_block[blk_sel+2] & MEMORY_MASK)-((int)mem_block[blk_sel+1] & MEMORY_MASK))){
						if(!((int)mem_block[blk_sel+1] & USED_MASK)){
							cmem_merge_block(blk_sel);
						}else{
							break;
						}
					}else{
						break;
					}
				}else{
					//if the block before the selected block is equal in size and not used merge blocks
					if(blk_size == (((int)mem_block[blk_sel] & MEMORY_MASK)-((int)mem_block[blk_sel-1] & MEMORY_MASK))){
						if(!((int)mem_block[blk_sel-1] & USED_MASK)){
							cmem_merge_block(blk_sel-1);
							blk_sel--;
						}else{
							break;
						}
					}else{
						break;
					}
				}

			}
			return;
		}
		blk_sel++;
	}
	printf("CMEM: cant find pointer to free %x\r\n",ptr);
}

void cmem_print(void){
	uint16_t blk_sel = 0;
	uint16_t blk_size = 0;
	while(mem_block[blk_sel+1] != NULL){
		blk_size = (((int)mem_block[blk_sel+1] & MEMORY_MASK)-((int)mem_block[blk_sel] & MEMORY_MASK));
		printf("BLK%d:%d %d\r\n",blk_sel,blk_size,(int)mem_block[blk_sel] & USED_MASK);
		blk_sel++;
	}
	printf("MAXBLK: %d\r\n",max_block);
}
