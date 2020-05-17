#include "low_level.h"
#include "pic.h"
#include "types.h" 
#include "printf.h"

#define ICW1 0x11 // 0x01 -> ICW4 needed, 0x10 -> ICW3 needed 
#define ICW4 0x01 // 0x00 -> MCS 80/85 , 0x01 -> 80x86 mode
#define PIC_ACK 0x60

uint8_t pic_control[2] = {0x20, 0xA0};
uint8_t pic_data[2] = {0x21, 0xA1};


void pic_init(int offset1, int offset2){
    
    /* master config */
    outb(ICW1, pic_control[0]); 
    outb(offset1, pic_data[0]); /* ICW2 */
    outb(0x04, pic_data[0]);
    outb(ICW4, pic_data[0]);
    outb(~(0x04), pic_data[0]); /* update mask register*/

    /* slave config */
    outb(ICW1, pic_control[1]);
    outb(offset2, pic_data[1]);
    outb(0x02, pic_data[1]);
    outb(ICW4, pic_data[1]);
    outb(~0, pic_data[1]); /* disable all irq lines */

    printf("[pic] ready\n");
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
       outb(mask, pic_data[0]);
    }
    else{
        mask = inb(pic_data[1]);
        mask = mask & ~(1 << irq);
        outb(mask, pic_data[1]);
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
       outb(mask, pic_data[0]);
    }
    else{
        mask = inb(pic_data[1]);
        mask = mask | (1 << irq);
        outb(mask, pic_data[1]);
        pic_irq_enable(2); /* slave connects to pin 2 in master*/
    }
}

/*
 * 0x6x command is for acknowledge
 * 0x60 - 0x6F to D0 - D7 master/slave
 * */
void pic_ack(uint8_t irq){
    
    if (irq >= 8){
        outb(PIC_ACK + (irq - 8), pic_control[1]);
        outb(PIC_ACK + 2, pic_control[0]);
    }
    else{
        outb(PIC_ACK + irq, pic_control[0]);
    }
}