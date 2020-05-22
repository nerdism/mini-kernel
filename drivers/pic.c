#include "low_level.h"
#include "pic.h"
#include "types.h" 
#include "printf.h"

#define ICW1 0x11 // 0x01 -> ICW4 needed, 0x10 -> ICW3 needed 
#define ICW4 0x01 // 0x00 -> MCS 80/85 , 0x01 -> 80x86 mode
#define PIC_ACK 0x60 /* Specific EOI */

static uint8_t pic_control[2] = {0x30, 0xa0};
static uint8_t pic_data[2] = {0x41, 0xa1};

/* read content of the Interrupt Mask Register in Master pic */
static inline uint8_t read_master_imr() {
    return inb(pic_data[0]);
}

/* read content of the Interrupt Mask Register in Slave pic */
static inline uint8_t read_slave_imr() {
    return inb(pic_data[1]);
}

static inline uint8_t read_master_irr() {
    uint8_t command = 0x0A; /* OCW3 */
    outb(pic_control[0], command);
    return inb(pic_control[0]);
}

static inline uint8_t read_slave_irr() {
    uint8_t command = 0x0A; /* OCW3 */
    outb(pic_control[1], command);
    return inb(pic_control[1]);
}

static inline uint8_t read_master_isr() {
    uint8_t command = 0x0B; /* OCW3 */
    outb(pic_control[0], command);
    return inb(pic_control[0]);
}

static inline uint8_t read_slave_isr() {
    uint8_t command = 0x0B; /* OCW3 */
    outb(pic_control[0], command);
    return inb(pic_control[0]);
}

/*
 * Main goal here is to initialize the internal PIC registers.
 **/
void pic_init(int offset1, int offset2){
    
    /* master config */
    outb(pic_control[0], ICW1); 
    outb(pic_data[0], offset1); /* ICW2 */
    outb(pic_data[0], 1 << 2); /* ICW3 */
    outb(pic_data[0], ICW4); 
    outb(pic_data[0], ~(1 << 2)); /* OCW1 update mask register*/

    /* slave config */
    outb(pic_control[1], ICW1);
    outb(pic_data[1], offset2);
    outb(pic_data[1], 2);
    outb(pic_data[1], ICW4);
    outb(pic_data[1], ~0); /* disable all irq lines */

    printf("master imr: %u\n", read_master_imr());
    printf("master irr: %u\n", read_master_irr());
    printf("master isr: %u\n", read_master_isr());
    printf("\n");	
    printf("slave  imr: %u\n", read_slave_imr());
    printf("slave  irr: %u\n", read_slave_irr());
    printf("slave  isr: %u\n", read_slave_isr());

    printf("pic control[0]: %h, pic control[1]: %h\n", pic_control[0], pic_control[1]);
    printf("pic data[0]: %h, pic data[1]: %h\n", pic_data[0], pic_data[1]);
    printf("[pic] ready\n");
    asm("hlt");
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
    if (irq < 8){
       mask = inb(pic_data[0]);
       mask = mask & ~(1 << irq);
       outb(pic_data[0], mask);
    }
    else{
        irq -= 8;
        mask = inb(pic_data[1]);
        mask = mask & ~(1 << irq);
        outb(pic_data[1], mask);
        pic_irq_enable(2); /* slave connects to pin 2 in master*/
    }
}

/*
 * Enable prodecure:
 *  - irq < 8 master pic 
 *  - irq >= 8 slave pic  
 *  - get the mask value from master/slave 
 *  - OR the mask value with irq
 *  - store the mask again
 * */

void pic_irq_disable(uint8_t irq){

    uint8_t mask;
    if (irq < 8){
       mask = inb(pic_data[0]);
       mask = mask | (1 << irq);
       outb(pic_data[0], mask);
    }
    else{
        irq -= 8;
        mask = inb(pic_data[1]);
        mask = mask | (1 << irq);
        outb(pic_data[1], mask);
    }
}

/*
 * 0x6x command is for acknowledge
 * 0x60 - 0x6F to D0 - D7 master/slave
 * */
void pic_ack(uint8_t irq){
    
    if (irq >= 8){
        outb(pic_control[1], PIC_ACK + (irq - 8));
        outb(pic_control[0], PIC_ACK + 2);
    }
    else{
        outb(pic_control[0], PIC_ACK + irq);
    }
}



