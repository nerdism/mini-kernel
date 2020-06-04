
/**
 * this file contains code for initializing pit
 * (programmable interval timer 8253/8254 chip)
 */

#include "low_level.h"
#include "printf.h"
#include "interrupts.h"
#include "pit.h"

#define COUNTER0    0x40    /* counter0 data reg R/W */
#define COUNTER1    0x41    /* counter1 data reg R/W */
#define COUNTER2    0x42    /* counter2 data reg R/W */
#define COMMAND_REG 0x43    /* mode/command  reg   W */

#define MODE3	0x36 /* 00 11 011 0*/


void pit_init() {
    outb(COMMAND_REG, MODE3);
    outb(COUNTER0, 0xff);
    outb(COUNTER0, 0xff);

    intr_enable(32);
    printf("clock initialized\n");
}
