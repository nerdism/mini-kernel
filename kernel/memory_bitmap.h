#ifndef MEMORY_BITMAP_H
#define MEMORY_BITMAP_H

#include "types.h"

void bitmap_memory_init(void *memory_start, uint32_t limit);

/**
 * allocate blocks of memory with given size 
 * returns a pointer to free memory area
 */
void *mem_alloc(uint32_t size);


/* free blocks of memory begin from mem_ptr */
void mem_free(void *mem_ptr, uint32_t size);


#endif
