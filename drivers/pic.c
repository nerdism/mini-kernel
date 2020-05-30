#include "low_level.h"
#include "pic.h"
#include "types.h" 
#include "printf.h"

#define ICW1 0x11 // 0x01 -> ICW4 needed, 0x10 -> ICW3 needed 
#define ICW4 0x01 // 0x00 -> MCS 80/85 , 0x01 -> 80x86 mode
#define PIC_ACK 0x60 /* Specific EOI */

#define PIC_M_COMMAND 0x20 // PIC_M_COMMAND + 1 = PIC_M_DATA
#define PIC_M_DATA (PIC_M_COMMAND + 1) 

#define PIC_S_COMMAND 0xa0 // PIC_S_COMMAND + 1 = PIC_S_DATA
#define PIC_S_DATA (PIC_S_COMMAND + 1) // PIC_S_COMMAND + 1 = PIC_S_DATA


/* read content of the Interrupt Mask Register in Master pic */
static inline uint8_t read_master_imr() {
    return inb(PIC_M_DATA);
}

/* update content of the Interrupt Mask Register in Master pic */
static inline void update_master_imr(uint8_t data) {
    outb(PIC_M_DATA, data); /* OCW1 update mask register*/
}

/* read content of the Interrupt Mask Register in Slave pic */
static inline uint8_t read_slave_imr() {
    return inb(PIC_S_DATA);
}

/* update content of the Interrupt Mask Register in Slave pic */
static inline void update_slave_imr(uint8_t data) {
    outb(PIC_S_DATA, data); /* OCW1 update mask register*/
}

static inline uint8_t read_master_irr() {
    uint8_t command = 0x0A; /* OCW3 */
    outb(PIC_M_COMMAND, command);
    return inb(PIC_M_COMMAND);
}

static inline uint8_t read_slave_irr() {
    uint8_t command = 0x0A; /* OCW3 */
    outb(PIC_S_COMMAND, command);
    return inb(PIC_S_COMMAND);
}

static inline uint8_t read_master_isr() {
    uint8_t command = 0x0B; /* OCW3 */
    outb(PIC_M_COMMAND, command);
    return inb(PIC_M_COMMAND);
}

static inline uint8_t read_slave_isr() {
    uint8_t command = 0x0B; /* OCW3 */
    outb(PIC_S_COMMAND, command);
    return inb(PIC_S_COMMAND);
}

static inline void icw1_master() {
    outb(PIC_M_COMMAND, ICW1); 
}

static inline void icw1_slave() {
    outb(PIC_S_COMMAND, ICW1);
}

static inline void icw2_master(int offset1) {
    outb(PIC_M_DATA, offset1); /* ICW2 */
}

static inline void icw2_slave(int offset2) {
    outb(PIC_S_DATA, offset2);
}

static inline void icw3_master() {
    outb(PIC_M_DATA, 4); /* ICW3 */
}

static inline void icw3_slave() {
    outb(PIC_S_DATA, 2);
}

static inline void icw4_master() {
    outb(PIC_M_DATA, ICW4); 
}

static inline void icw4_slave() {
    outb(PIC_S_DATA, ICW4);
}



/*
 * Main goal here is to initialize the internal PIC registers.
 **/
void pic_init(int offset1, int offset2) {
    
    /* master config */
    icw1_master();
    icw2_master(offset1);
    icw3_master();
    icw4_master();

    /* update interrupt mask register */
    update_master_imr(~4); /* disable all irq lines except the slave line */


    /* slave config */
    icw1_slave();
    icw2_slave(offset2);
    icw3_slave();
    icw4_slave();

    /* update interrupt mask register */
    update_slave_imr(~0); /* disable all irq lines */

    printf("[+] pic ready\n");
}


/*
 * Enable prodecure:
 *  - irq < 8 master pic 
 *  - irq >= 8 slave pic  
 *  - get the mask value from master/slave 
 *  - AND the mask value with irq
 *  - store the mask again
 * */

void pic_irq_enable(uint8_t irq){

    uint8_t mask;
    if (irq < 8) {
       mask = inb(PIC_M_DATA);
       mask = mask & ~(1 << irq);
       update_master_imr(mask);
    }
    else {
        irq -= 8;
        mask = inb(PIC_S_DATA);
        mask = mask & ~(1 << irq);
	update_slave_imr(mask);
        pic_irq_enable(2); /* slave connects to pin 2 in master*/
    }
}

/*
 * Disable prodecure:
 *  - irq < 8 master pic 
 *  - irq >= 8 slave pic  
 *  - get the mask value from master/slave 
 *  - OR the mask value with irq
 *  - store the mask again
 * */

void pic_irq_disable(uint8_t irq){

    uint8_t mask;
    if (irq < 8) {
	mask = inb(PIC_M_DATA);
	mask = mask | (1 << irq);
	update_master_imr(mask);
    }
    else {
        irq -= 8;
        mask = inb(PIC_S_DATA);
        mask = mask | (1 << irq);
	update_slave_imr(mask);
    }
}

/*
 * 0x6x command is for acknowledge
 * 0x60 - 0x6F to D0 - D7 master/slave
 * */
void pic_ack(uint8_t irq){
    
    if (irq >= 8) {
        outb(PIC_M_COMMAND, PIC_ACK + (irq - 8));
        outb(PIC_S_COMMAND, PIC_ACK + 2);
    }
    else {
        outb(PIC_M_COMMAND, PIC_ACK + irq);
    }
}



