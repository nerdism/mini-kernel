
[bits 32]
[extern main]

lidt [idt_descriptor]

call main
jmp $

%include "idt.asm"
