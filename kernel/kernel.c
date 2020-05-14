#include "screen.h"
#include "printf.h"
#include "memory.h"


void main() {
    init_screen();    
    printf("[+] screen init\n");
    memory_init();

    // printf("free memory begins from 0x%h\n", (uint32_t)m);
    // printf("free memory begins from 0x%h\n", (uint32_t)m2);
}


