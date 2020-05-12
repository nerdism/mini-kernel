
#include "printf.h"
#include "memory_layout.h"
#include "string.h"
#include "types.h"

#define KILO 1024
#define MEGA (KILO * KILO)

#define BLOCK 4096

/* for more explination about this struct go to memory_test.asm in boot folder */
struct Mem_Info {
    uint32_t basel;  /* 64 bit base address */
    uint32_t baseh; 

    uint32_t limitl; /* 64 bit length */
    uint32_t limith;

    uint32_t type;
    uint32_t acpi;
};

uint8_t *bitmap;    /* where shall i place this in memory */

void memory_init() {

    /* memory test entries count */
    uint32_t c = *((uint32_t*)MEM_INFO_CNT);  
    uint32_t max_len = 0;
    void *mem_begin;
    /* printf("memory entries count: %d\n", c); */
    struct Mem_Info *mem_ptr = (struct Mem_Info*)MEM_INFO;

    /* determine biggest free memory area */
    for (int i = 0; i < c; i++) {

	/* printf("basel: 0x%h, " */
	/*        "limitl: %u, " */
	/* 	"type: %h, " */
	/* 	"acpi: %h\n", */
	/* 	mem_ptr[i].basel, */
	/* 	mem_ptr[i].limitl/KILO, */
	/* 	mem_ptr[i].type, mem_ptr[i].acpi); */
	
	if (mem_ptr[i].limitl > max_len) {
    	    mem_begin = (void*)mem_ptr[i].basel;	    	
	    max_len = mem_ptr[i].limitl;
	}
    }
    uint8_t ismb = 0;
    if (max_len > MEGA) ismb = 1;
    printf("free memory begin: 0x%h, len: %u %s\n",
		mem_begin,
		((ismb)? max_len/MEGA : max_len/KILO),
		((ismb)? "MB": "KB"));
    
    
    uint32_t bitmap_size = (max_len/BLOCK); /* [+] we discard the remaining bytes of the last block */

    printf("bitmap size: %u bits or %u bytes", bitmap_size, bitmap_size/8);
    
    // memset(bitmap);
    
}



