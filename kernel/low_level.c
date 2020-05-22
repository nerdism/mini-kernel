
#include "low_level.h"
#include "types.h"

/** 
 *   read a byte from a port
 */
uint8_t inb(uint16_t port) {
    /* "=a" means put the data in to the al register */
    /* "d" means read the port from dx register */
    uint8_t result; 
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

/**
 * write a byte to a port
 */
void outb(uint16_t port, uint8_t data) {
    __asm__("out %%al, %%dx" : :"a" (data) , "d" (port));
}


/**
 * read a word from port
 */
uint16_t inw(uint16_t port) {

    uint16_t result; 
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    
    return result;
}

/**
 * write a word to a port
 */
void outw(uint16_t port, uint16_t data) {
    __asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
}


