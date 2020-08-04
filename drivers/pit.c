
/**
 * this file contains code for initializing pit
 * (programmable interval timer 8253/8254 chip)
 *
 * for explaining pit look at this pit.h
 **/

#include "kernel/low_level.h"
#include "printf.h"
#include "kernel/interrupts.h"
#include "drivers/pit.h"

/* Input frequency of every timer */
#define IF 1193182	    /* Hz */

/* Desired output frequencey of counter0 */
#define OF 10		    /* becuase f = 1/T
			     * and we want to have interrupt 
			     * every 100 ms or 0.1 s
			     * that makes f = 1/0.1 
			     * which is f = 10
			     */

/* 
 * --> fout = fin / counter_val
 * --> counter_val = fin / fout
 * --> counter_val = fin / fout / 2
 *
 * division by two because in mode 3 counter 
 * decreases by two value with every clock paulse
 **/

#define COUNTER0_VALUE (((unsigned)IF) / OF / 2)

/* port addresses */
#define COUNTER0    0x40    /* counter0 data reg R/W */
#define COUNTER1    0x41    /* counter1 data reg R/W */
#define COUNTER2    0x42    /* counter2 data reg R/W */
#define COMMAND_REG 0x43    /* mode/command  reg   W */

#define MODE3	    0x36    /* 00 11 011 0  
			     * square wave generator mode 
			     * and sending two byte continusly 
			     * to the data register of counter 0
			     **/

#define PIT_INTR_NUM	32  /* or pin zero of master pic */

uint32_t second = 0;
uint32_t mili	= 0;

static void pit_intr_handler(int intr_num, int code) {
    mili++;
    if (mili >= OF) {
	second++;
	mili = 0; 
    }
}

static clock_t clock_read() {
    clock_t t = {
	.second = second, 
	.mili   = mili
    };
    return t;
}


void pit_init() {
    
    /* initializing counter0 */
    outb(COMMAND_REG, MODE3);

    /* least significant byte of counter*/
    outb(COUNTER0, COUNTER0_VALUE & 0xff);

    /* most significant byte of counter */
    outb(COUNTER0, (COUNTER0_VALUE >> 8) & 0xff);

    intr_handler_register(PIT_INTR_NUM, pit_intr_handler);
    intr_enable(PIT_INTR_NUM);

    printf("[+] clock initialized\n");
}


/*
 * block the code execution until 
 * hundred of milisecond passes
 *  
 * @Param hmili waiting time in hundred of milisecond
 *	  ex.   clock_wait(3) means wait for 300 milisecond
 **/
void clock_wait(uint32_t hmili) {
    uint32_t desired, start, passed;
    desired = hmili * 100;
    clock_t s = clock_read();    
    start = s.second * 1000 + s.mili * 100;
    clock_t p;
    do {
	p = clock_read(); 
	passed = p.second * 1000 + p.mili * 100;
	/* printf("%u < %u\n", (passed - start) , desired); */
    } while ((passed - start) < desired);
}



