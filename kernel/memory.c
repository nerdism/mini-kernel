#include "printf.h"
#include "memory_layout.h"
#include "types.h"
#include "memory.h"
#include "memory_bitmap.h"
#include "kmalloc.h"

/**
 * for more explanation about this struct go to 
 * memory_test.asm in boot folder 
 */

struct Mem_Info {
    uint32_t basel;  /* 64 bit base address */
    uint32_t baseh; 

    uint32_t limitl; /* 64 bit length */
    uint32_t limith;

    uint32_t type;
    uint32_t acpi;
};

/* beginning of the biggest free memory area */
void *memory_start;


void memory_init() {

    /* memory test entries count */
    uint32_t c = *((uint32_t*)MEM_INFO_CNT);  

    /* address of beginning of a memory region with biggest free area*/
    /* it's a local defination of memory_start */
    void    *mem_begin ;

    /* size of the biggest memory area*/
    uint32_t max_len = 0;

    /* pointer to memory information obtained by bios int 0x15 (look at the memory_test.asm */
    struct Mem_Info *mem_ptr = (struct Mem_Info*)MEM_INFO;


    /* determine biggest free memory area
     * @Todo change this so that it controles
     *	other free memory regions
     **/
    for (int i = 0; i < c; i++) {
	if (mem_ptr[i].limitl > max_len) {
    	    mem_begin = (void*)mem_ptr[i].basel;	    	
	    max_len = mem_ptr[i].limitl;
	}
    }
    if (max_len < 2 * MEGA) {
	printf("[-] memory is less then 2 MB which is not functional\n");
	return;
    }

    /* uint8_t ismb = 0; */
    /* if (max_len > MEGA) ismb = 1; */
    /* printf("free memory begin: 0x%h, len: %u %s\n", */
		/* mem_begin, */
		/* ((ismb)	? max_len/MEGA	: max_len/KILO), */
		/* ((ismb) ? "MB"		: "KB")); */
    
    
    memory_start = mem_begin;

    kmalloc_init(memory_start, MEGA);
    bitmap_memory_init(memory_start+MEGA, max_len-MEGA); 
}



