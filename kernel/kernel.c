#include "kernel/interrupts.h"
#include "kernel/memory.h"
#include "drivers/screen.h"
#include "drivers/keyboard.h"
#include "kernel/kshell.h"
#include "drivers/pit.h"
#include "drivers/ata.h"
#include "printf.h"


void main() {
    init_screen();    
    memory_init();
    intr_init();
    pit_init();
    ata_init();
    keyboard_init();
    
    
    kshell_run();
    // printf("free memory begins from 0x%h\n", (uint32_t)m);
    // printf("free memory begins from 0x%h\n", (uint32_t)m2);
}


