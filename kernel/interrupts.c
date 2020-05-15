#include "interrupts.h"
#include "pic.h"
#include "printf.h"
#include "types.h"


uint32_t intr_count[48]; /* Record how many each intr occures */
uint8_t intr_spurious[48]; /* Record how many each intr occures */

/* Some exceptions will push a 32-bit "error code" on to the top of the stack, \
 * which provides additional information about the error. 
 * This value must be pulled from the stack before returning control back to \
 * the currently running program. (i.e. before calling IRET).
 */
const char *exceptions[] = {
    /* this order is important. */
	"division by zero",
	"debug exception",
	"nonmaskable interrupt",
	"breakpoint",
	"overflow",
	"bounds check",
	"invalid instruction",
	"coprocessor error",
	"double fault",
	"copressor overrun",
	"invalid task",
	"segment not present",
	"stack exception",
	"general protection fault",
	"page fault",
	"unknown",
    "coprocessor error"
};

/*
 * In protected mode, the IRQs 0 to 7 conflict with the CPU exception which are reserved by Intel up until 0x1F.
 * (It was an IBM design mistake.) 
 * Consequently it is difficult to tell the difference between an IRQ or an software error. 
 * It is thus recommended to change the PIC's offsets (also known as remapping the PIC) so that IRQs use non-reserved vectors. 
 * A common choice is to move them to the beginning of the available range (IRQs 0..0xF -> INT 0x20..0x2F). (0 .. 15 -> 32 .. 47)
 * For that, we need to set the master PIC's offset to 0x20(32) and the slave's to 0x28(40). 
 */

void intr_init() {
    
    pic_init(32, 40);

    for (int i = 32; i < 48; i++) {
        intr_disable(i);
        intr_ack(i);
    }

    for (int i = 0; i < 48; i++) {
       intr_count[i] = 0;
       intr_spurious[i] = 0;
    }
}

void intr_handler(int intr_num, int code) {
    printf("\ninterrupt: num %i\tcode %i\n", intr_num, code);
    intr_count[intr_num]++;

    if (intr_num > 31){
        intr_spurious[intr_num]++;
    }
}

