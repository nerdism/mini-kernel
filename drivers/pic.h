#ifndef PIC_H
#define PIC_H


#include "types.h"

void pic_init(int offset1, int offset2);
void pic_irq_enable(uint8_t irq);
void pic_irq_disable(uint8_t irq);
void pic_ack(uint8_t irq);



#endif
