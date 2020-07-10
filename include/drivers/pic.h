#ifndef PIC_H
#define PIC_H


#include "types.h"

void pic_init(int offset1, int offset2);
void pic_irq_enable(uint8_t irq);
void pic_irq_disable(uint8_t irq);
void pic_ack(uint8_t irq);

/*
 * Pic (programmable interrupt controller (intel 8259 ic)) is a device
 * which is responsible for handling hardware interrupts.
 * for actual implemention look at the pic.c file
 */

/**
 * How to program the pic ?
 * pic accepts two types of commands
 *
 * 1) ICW: Initialization command word 
 *    we have ICW1, ICW2, ICW3, ICW4
 *
 * 2) OCW: Operation command word
 *    we have OCW1, OCW2, OCW3
 *  
 *  in order to program the pic we must first
 *  initialize the chip with ICW commands
 *  after that we can send OCW commands any time
 *  remember that if we are initializing ICWs
 *  we must do that in order
 *  first ICW1 -> ICW2 -> ICW3 -> ICW4
 *  
 */

/**    ICW1 port (0x20 master) (0xA0 slave)
 *  -----------------------------------------
 *  | 0 | 0 | 0 | 1 | ltm | 0 | sngl | icw4 |
 *  -----------------------------------------
 *
 * ltm:  1 = level triggerd mode, 0 = edge triggerd
 * sngl: 1 = single pic , 0 = cascade (multiple pic)
 * icw4: 1 = using icw4 , 0 = not using icw4
 *
 * ICW1: Programs the basic operation of the 8259. 
 * To program this ICW for 8086 operation,
 * place 1 in bit IC 4. 00010000b 
 * PIC will look for ICW with internal bits. 
 * the 4th bit from left is the place,
 * PIC is going to look for finding which ICW should be programmed.
 * For using ICW4 set rightmost bit 1. 0x11
 */

/**  ICW2 port (0x21 master) (0xa1 slave)
 *  --------------------------------------
 *  | A4 | A3 | A2 | A1 | A0 | 0 | 0 | 0 |
 *  --------------------------------------
 *
 * A0-A4: interrupt vector address
 *
 * initializing ICW2 sets pic interrupt offset
 * pic has 8 interrupt request pins
 * if any interrupt came from a pin and
 * pic would want to send the cpu the pin number
 * it would first add the binary number in A0-A4
 * to that pin number then it will send the interrupt number
 * on the data bus
 *
 */


/**   ICW3 port (0x21 master)
 * ----------------------------------------
 * | S7 | S6 | S5 | S4 | S3 | S2 | S1 | S0 |
 * ----------------------------------------
 * S0-S7: each S intdicates a pic pin number whether
 *	  connected to a slave pic or not.
 *	  1 = connected
 *	  0 = not connected
 *
 *    ICW3 port (0xa1 slave)
 * -----------------------------------
 * | 0 | 0 | 0 | 0 | 0 | D2 | D1 | D0 |
 * ------------------------------------
 * D0-D2: number of pin which slave is connected to master in binary
 *
 *
 * ICW3 Only used when ICW 1 indicates that the system is operated in cascade mode. 
 * This ICW indicates where the slave is connected to the master.
 * (In our case this number is 0x04 for master and 0x02 for slave)
 */


/**     ICW4 port (0x21 master)  (0xa1 slave)
 *  ----------------------------------------------
 *  | 0 | 0 | 0 | SFNM | buf | m/s | AEOI | Arch |
 *  ----------------------------------------------
 *
 * SFNM: 1 = special fully nested mode, 0 = not sfnm
 * buf and m/s : 0 x non bufferd mode
 *		 1 0 bufferd mode slave
 *		 1 1 bufferd mode master
 * 
 * AEOI: 1 = automatic end of interrupt, 0 = normal eoi
 * ARCH: 1 = 8086/8088, 0 = 8085
 *
 * ICW4 Programmed for use with the 8086–Pentium 4 microprocessors, 
 * but is not programmed in a system that functions with 
 * the 8085 microprocessor. Rightmost bit is 1 for 8086 (16 bit mode).
 * In this ICW we set AOEI to zero, in order to use ICW2. 
 * ICW2 is only used when this bit is 0.
 */

/**   OCW1 port (0x21 master) (0xa1 slave)
 *
 *   this command sets the interrupt mask bits
 *   in the Interrupt mask register (imr)
 *
 *  -----------------------------------------
 *  | M7 | M6 | M5 | M4 | M3 | M2 | M1 | M0 |
 *  -----------------------------------------
 * 
 *  M0-M7: every M (bit) represent the interrrupt pin
 *	   1 = mask that interrupt pin (does not accept interrupt)
 *	   0 = unmask that interrupt pin
 *	
 */

/**   OCW2 port (0x20 master) (0xa0 slave)
 *
 *    with this command we send end of interrupt (eoi)
 *    
 *  -----------------------------------------
 *  | R | SL | EOI | 0 | 0 | L2 | L1 | L0 |
 *  -----------------------------------------
 * 
 *  L0-L2: interrupt number in in-service register (isr)
 *         if we are using specific eoi then we must set
 *         the interrupt pin number in L0-L2
 *
 *  
 *  R, Sl, EOI
 *  0, 0 , 1   = Non specifi eoi command 
 *  0, 1 , 1   = specific eoi command
 *  x, x , x   = to be documented
 *	
 */

/**   OCW3 port (0x20 master) (0xa0 slave)
 *
 *    we can read irr and isr register
 *    and do polling
 *    
 *  -------------------------------------
 *  | 0 | 0 | 0 | 0 | 1 | P | RIR | RIS |
 *  -------------------------------------
 *  
 *  P, RIR, RIS (poll, read IRr, read isr)
 *
 *  1,  0,  0  = do polling
 *		 after sending polling byte
 *		 if read from  pic
 *		 we get a byte with this format in binary
 *		 a0000bcd
 *		 a: if 1 then we have interrupt
 *		 bcd: number of the interrupt with highest prio
 *
 *
 *  0,  1,  0  = read IRR in next read
 *  0,  1,  1  = read ISR in next read
 *	
 */

/** 
 * we can read interrupt mask register with
 * input command from port (0x21 master) (0xA1 slave)
 */

/* 
 * What is ack specific byte (0x60)?
 *
 * When we are in nomral EOI we can set OCW2 which is determined how interrupt should end.
 * Nonspecific EOI byte is 0x20.
 * Specific EOI byte is 0x60.
 *
 * Nonspecific End-of-Interrupt—A command sent by the interrupt service procedure to signal the end of the interrupt. 
 * The 8259A automatically determines which interrupt level was active and resets the correct bit of the interrupt status register.
 * Resetting the status bit allows the interrupt to take action again or a lower priority interrupt to take effect.
 *
 * Specific End-of-Interrupt—A command that allows a specific interrupt request to be reset.
 * The exact position is determined with bits L 2 –L 0 of OCW 2. 0x60 + irq
 *
 * There are three status registers: In-service register (ISR), Interrupt Request Register (IRR), Interrupt Mask Register (IMR)
 * 
*/




#endif



