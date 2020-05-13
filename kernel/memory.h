#ifndef MEMORY_H
#define MEMORY_H

void memory_init();

/**
 * allocate blocks of memory with given size 
 * returns a pointer to free memory area
 */
void *mem_alloc(uint32_t size);
#endif
