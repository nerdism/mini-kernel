#ifndef PIT_H
#define PIT_H

#include "types.h"

typedef struct {
    uint32_t second;
    uint32_t mili;
} clock_t;

/*
 * block the code execution until 
 * hundred of milisecond passes
 *  
 * @Param hmili waiting time in hundred of milisecond
 *	  ex.   clock_wait(3) means wait for 300 milisecond
 **/
void clock_wait(uint32_t hmili);


void pit_init();



/**
 * pit (programmable interval timer) 8253/8254
 * is a chip that can be used as a timer
 * we can use it in multitasking environment
 * for interrupting processes and switching them out
 *
 * also we use it as tool for measuring
 * how much time has passed
 * from two certain point 
 *
 * has 4 registers
 * three of them are data register for counters 
 * with port adresses (0x40, 0x41, 0x42)
 * and one is mode/command register. port (0x43)
 **/


/**
 *  mode/command register
 *  ----------------------------------------------
 *  | S1 | S0 | A1 | A0 | M2 | M1 | M0 | Bin/Bcd | 
 *  ----------------------------------------------
 *
 *  Bin/Bcd:
 *	0 = 16 bit binary mode
 *	1 = 4 digit bcd mode
 *  
 *  M0-M2:  (Operating Mode)
 *	0 0 0 = Mode 0 (interrupt on terminal count)
 *      0 0 1 = Mode 1 (hardware re-triggerable one-shot)
 *      0 1 0 = Mode 2 (rate generator)
 *      0 1 1 = Mode 3 (square wave generator)
 *      1 0 0 = Mode 4 (software triggered strobe)
 *      1 0 1 = Mode 5 (hardware triggered strobe)
 *      1 1 0 = Mode 2 (rate generator, same as 010b)
 *      1 1 1 = Mode 3 (square wave generator, same as 011b)
 *
 *  A0-A1:  Access mode
 *	0 0 = Latch count value command
 *      0 1 = Access mode: lobyte only
 *      1 0 = Access mode: hibyte only
 *	1 1 = Access mode: lobyte/hibyte	
 *	
 *
 *  S0-S1:  Select Channel/counter
 *	0 0 = counter 0
 *      0 1 = counter 1
 *      1 0 = counter 2
 *      1 1 = Read-back command (8254 only)
 **/

#endif

