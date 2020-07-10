#ifndef INTERRUPTS_H
#define INTERRUPTS_H

/*
IRQ Interrupts:
32	System Timer
33	Keyboard
34	Cascade
35	Serial Port 2
36	Serial Port 1
37	Parallel Port 2 or Sound Card
38	Floppy
39	Parallel Port 1
40	Real Time Clock
41	Network (or ATA 3)
42	Network/Sound/SCSI
43	SCSI Other (or ATA 2)
44	PS/2 Mouse
45	FPU
46	ATA 0
47	ATA 1
**/

/*
 * for system call intr number we can use any available number greater than 47, i.e 48
 * */

typedef void (*intr_handler_t) (int intr_num, int code); /* function pointer for each hardware and software interrupts.*/
void interrupt_handler(int intr_num, int code);
void intr_handler_register(int intr_num, intr_handler_t handler);
void intr_ack();
void intr_block();
void intr_disable(int intr_num);
void intr_enable(int intr_num);
void intr_init(); 
void intr_unblock();
void intr_wait();

#endif
