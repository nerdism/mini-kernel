
#include "low_level.h"

/** 
 *   read a byte from a port
 */
unsigned char inb(unsigned short port) {
    /* "=a" means put the data in to the al register */
    /* "d" means read the port from dx register */
    unsigned char result; 
    __asm__("in %%dx, %%al" : "=a" (result) : "d" (port));

    return result;
}

/**
 * write a byte to a port
 */
void outb(unsigned short port, unsigned char data) {
    __asm__("out %%al, %%dx" : :"a" (data) , "d" (port));
}


/**
 * read a word from port
 */
unsigned short inw(unsigned short port) {

    unsigned short result; 
    __asm__("in %%dx, %%ax" : "=a" (result) : "d" (port));
    
    return result;
}

/**
 * write a word to a port
 */
void outw(unsigned short port, unsigned short data) {
    __asm__("out %%ax, %%dx" : :"a" (data), "d" (port));
}


