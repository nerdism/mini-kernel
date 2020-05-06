#include "screen.h"
#include "printf.h"
#include "memory.h"


void main() {
    init_screen();    
    printf("[+] screen init\n");
    memory_init();
    /* printf("memory info count: %d", x); */
}


