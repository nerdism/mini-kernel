[bits 16]

switch_to_pm:

    cli	; clear intrrupt 

    lgdt [gdt_descriptor]   ; load global descriptor table

    mov eax, cr0	    ; tell control register that we are in
    or eax, 0x1		    ; protected mode
    mov cr0, eax

    jmp CODE_SEG:init_pm    ; long jump (basicly clear the instration queue from the 16 bit operations)

[bits 32]
init_pm:

    mov ax, DATA_SEG
    mov ds, ax
    mov es, ax
    mov ss, ax
    mov gs, ax
    mov fs, ax

    mov ebp, 0x90000
    mov esp, ebp

    call BEGIN_PM


