
[bits 32]

VIDEO_MEMORY   equ 0xb8000
WHITE_ON_BLACK equ 0x0f


printF_pm:
    pusha
    mov edx, VIDEO_MEMORY

printF_pm_loop:
    mov al, [ebx]
    mov ah, WHITE_ON_BLACK
    
    cmp al, 0 
    je printF_pm_loop_done

    mov [edx], ax
    add edx, 2 
    add ebx, 1
    jmp printF_pm_loop

printF_pm_loop_done:
    popa
    ret

