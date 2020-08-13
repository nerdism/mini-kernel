
[org 0x7c00]

KERNEL_LOAD_ADDRESS equ 0x1000
mov [BOOT_DRIVE], dl ; boot device number is in dl (system put it there)

mov bp, 0x9000	; stack base address
mov sp, bp	; initialize stack pointer

mov bx, REAL_MODE_MSG
call printF

call load_kernel    ; load kernel code

call memtest	    ; measure the memory

call switch_to_pm   ; switch to protected mode


%include "memory_test.asm"
%include "print_func.asm"
%include "disk_load.asm"
%include "gdt.asm"
%include "print_func_pm.asm"
%include "pm_switch.asm"

[bits 16]
load_kernel:
    mov bx, KERNEL_LOAD_MSG
    call printF

    mov bx, KERNEL_LOAD_ADDRESS

    ; @TODO: change below sector number to a variable
    ; that containes size of kernel in sectors
    mov dh, 50	; read this much sector after boot sector
    mov dl, [BOOT_DRIVE]
    call disk_load
    ret

[bits 32]
BEGIN_PM:
    mov ebx, PM_MODE_MSG
    call printF_pm
    
    call KERNEL_LOAD_ADDRESS

    jmp $


BOOT_DRIVE	db 0
KERNEL_LOAD_MSG db "loading kernel into memory" , 10, 13, 0
REAL_MODE_MSG   db "started in 16 bit real mode", 10, 13, 0
PM_MODE_MSG     db "switched to Protected mode" , 10, 13, 0

times 510 -($ - $$) db 0 ; add zeros 

dw 0xaa55 ; last two special bytes of boot sector 0xaa 0x55 (little endian)


