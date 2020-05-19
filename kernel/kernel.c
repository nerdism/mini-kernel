#include "screen.h"
#include "printf.h"
#include "memory.h"
#include "interrupts.h"


void main() {
    init_screen();    
    printf("[+] screen init\n");
    memory_init();

    intr_init();
    
    // printf("free memory begins from 0x%h\n", (uint32_t)m);
    // printf("free memory begins from 0x%h\n", (uint32_t)m2);
}


