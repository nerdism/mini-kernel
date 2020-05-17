#ifndef KMALLOC_H
#define KMALLOC_H

#include "types.h"

void kmalloc_init(void *memory_start, uint32_t limit);


/* Allocate the about of size memory 
 * returns the usable memory address */
void *kmalloc(uint32_t size);

/* free a kernel memory region */
void kfree(void *mem);

#endif
