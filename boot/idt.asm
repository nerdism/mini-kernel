;----------- Macros ---------------------

; multi line instructions in one line except pop instruction.
; usage:
    ; multi push 1, push 2, push 3, ...

%macro multi 1-*
    %rep %0
        %1
    %rotate 1
    %endrep
%endmacro
;--------- End Macros --------------------

; ---------------- Interrupts ---------------------

; ----- x86 Exceptions -----
interrupt00: multi push 0, push 0,  jmp intr_handler
interrupt01: multi push 0, push 1,  jmp intr_handler
interrupt02: multi push 0, push 2,  jmp intr_handler
interrupt03: multi push 0, push 3,  jmp intr_handler
interrupt04: multi push 0, push 4,  jmp intr_handler
interrupt05: multi push 0, push 5,  jmp intr_handler
interrupt06: multi push 0, push 6,  jmp intr_handler
interrupt07: multi push 0, push 7,  jmp intr_handler
interrupt08: multi         push 8,  jmp intr_handler ; interrupt 8 already pushed its codes, so we need to push its number.
interrupt09: multi push 0, push 9,  jmp intr_handler
interrupt10: multi         push 10, jmp intr_handler ; interrupt 10 already pushed its codes, so we need to push its number
interrupt11: multi         push 11, jmp intr_handler ; interrupt 11 already pushed its codes, so we need to push its number 
interrupt12: multi         push 12, jmp intr_handler ; interrupt 12 already pushed its codes, so we need to push its number
interrupt13: multi         push 13, jmp intr_handler ; interrupt 13 already pushed its codes, so we need to push its number
interrupt14: multi         push 14, jmp intr_handler ; interrupt 14 already pushed its codes, so we need to push its number
interrupt15: multi push 0, push 15, jmp intr_handler
interrupt16: multi push 0, push 16, jmp intr_handler
interrupt17: multi         push 17, jmp intr_handler ; interrupt 17 already pushed its codes, so we need to push its number
interrupt18: multi push 0, push 18, jmp intr_handler
interrupt19: multi push 0, push 19, jmp intr_handler

;------- Resevered interrupts ---------
interrupt20: multi push 0, push 20, jmp intr_handler
interrupt21: multi push 0, push 21, jmp intr_handler
interrupt22: multi push 0, push 22, jmp intr_handler
interrupt23: multi push 0, push 23, jmp intr_handler
interrupt24: multi push 0, push 24, jmp intr_handler
interrupt25: multi push 0, push 25, jmp intr_handler
interrupt26: multi push 0, push 26, jmp intr_handler
interrupt27: multi push 0, push 27, jmp intr_handler
interrupt28: multi push 0, push 28, jmp intr_handler
interrupt29: multi push 0, push 29, jmp intr_handler
interrupt30: multi push 0, push 30, jmp intr_handler
interrupt31: multi push 0, push 31, jmp intr_handler

;---- PIC interrupts IRQs -----
interrupt32: multi push 0, push 31, jmp intr_handler
interrupt33: multi push 0, push 32, jmp intr_handler
interrupt34: multi push 0, push 34, jmp intr_handler
interrupt35: multi push 0, push 35, jmp intr_handler
interrupt36: multi push 0, push 36, jmp intr_handler
interrupt37: multi push 0, push 37, jmp intr_handler
interrupt38: multi push 0, push 38, jmp intr_handler
interrupt39: multi push 0, push 39, jmp intr_handler
interrupt40: multi push 0, push 40, jmp intr_handler
interrupt41: multi push 0, push 41, jmp intr_handler
interrupt42: multi push 0, push 42, jmp intr_handler
interrupt43: multi push 0, push 43, jmp intr_handler
interrupt44: multi push 0, push 44, jmp intr_handler
interrupt45: multi push 0, push 45, jmp intr_handler
interrupt46: multi push 0, push 46, jmp intr_handler
interrupt47: multi push 0, push 47, jmp intr_handler
;interrupt48: multi push 0, push 48, jmp syscall_handler ; used for handling the system calls TODO: complete this after writing syscall 

;------------------ End interrupts ------------------


intr_handler:
    pushad 
    push dword [esp+36] ; second interrupt_handler argument (i.e code) -> interrupt code
    push dword [esp+36] ; first interrupt_handler argument (i.e intr_num) -> interrupt number
    mov eax, 2 * 8 ; switch to data segment inorder to retrieve some data stored already in data segment defined in gdt
    mov ds, eax
    call interrupt_handler ; call interrupt handler function in c file (interrupt.c)
    popad
    iret


start_idt:
    dw interrupt00 -  
