
#include "printf.h"
#include "memory_layout.h"
#include "types.h"

struct Mem_Info {
    uint32_t basel;
    uint32_t baseh;
    uint32_t limitl;
    uint32_t limith;
    uint32_t type;
    uint32_t acpi;
};

void memory_init() {
    /* memory test entries count */

    uint32_t c = *((uint32_t*)MEM_INFO_CNT);  
    printf("memory entries count: %d\n", c);
    struct Mem_Info *mem_ptr = (struct Mem_Info*)MEM_INFO;
    for (int i = 0; i < c; i++) {
	printf("basel: 0x%h, "
	       "limitl: 0x%h, "
		"type: %h, "
		"acpi: %h\n",
		mem_ptr[i].basel,
		mem_ptr[i].limitl,
		mem_ptr[i].type, mem_ptr[i].acpi);
    }
}


