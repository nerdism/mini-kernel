
[extern main]
global _start
_start:

[bits 32]

lidt [idt_descriptor]

call main
jmp $

%include "idt.asm"
