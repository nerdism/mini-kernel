#include "screen.h"
#include "printf.h"
#include "memory.h"
#include "interrupts.h"
#include "pit.h"
#include "ata.h"


void main() {
    init_screen();    
    printf("[+] screen init\n");
    memory_init();
    intr_init();
    pit_init();
    /* ata_init(); */

    
    // printf("free memory begins from 0x%h\n", (uint32_t)m);
    // printf("free memory begins from 0x%h\n", (uint32_t)m2);
}


