#ifndef PIC_H
#define PIC_H


#include "types.h"

void pic_init(int offset1, int offset2);
void pic_irq_enable(uint8_t irq);
void pic_irq_disable(uint8_t irq);
void pic_ack(uint8_t irq);

/* ICW: Initialization command words 
 * OCW: Operation command words
 */

/*
 * ICW1: Programs the basic operation of the 8259. To program this ICW for 8086 operation, place 1 in bit IC 4. 00010000b 
 * PIC will look for ICW with internal bits. the 4th bit from left is the place PIC is going to look for finding which ICW should be programmed.
 * For using ICW4 set rightmost bit 1. 0x11
 * OCW1: Used to set and read the interrupt mask register.
 * ICW2: Selects the vector number used with the interrupt request inputs. (offset in code, i.e; ICW2 + irq
 * ICW3: Only used when ICW 1 indicates that the system is operated in cascade mode. \
 * This ICW indicates where the slave is connected to the master. (In our case this number is 0x04 for master and 0x02 for slave)
 * ICW4: Programmed for use with the 8086–Pentium 4 microprocessors, but is not \
 * programmed in a system that functions with the 8085 microprocessor. Rightmost bit is 1 for 8086 (16 bit mode).
 * In this ICW we set AOEI to zero, in order to use ICW2. ICW2 is only used when this bit is 0.
 *
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



