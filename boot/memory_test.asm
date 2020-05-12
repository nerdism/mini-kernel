
; here we measure the memory size with the help of int 0x15 ax = 0xe820
; it returns a list of memory regiens with format mentioned below
; for a detailed information search "osdev memory detection"
; or use this link https://wiki.osdev.org/Detecting_Memory_(x86)

; entry struct format and size
; uint64    base
; uint64    limit
; uint32    type
;   type 1 usable normal memory
;   type 2 reserved - unusable
;   type 3 ACPI reclaimable memory 
;   type 4 ACPI NVS memory
;   type 5  Area containing bad memory
; uint32    acpi extended attribute

ENTRY_SIZE equ 24

MEM_ENT_CNT equ 0x8000	; memory info entry counts number Adress
MEM_INFO equ 0x8004	; memory informations entries Adress
MAGIC_NUM equ 0x534D4150

memtest:
    pusha
    mov ax, 0
    mov es, ax		; set extra segment to 0
    mov di, MEM_INFO	; set the place which memory information stored
    mov dword [MEM_ENT_CNT], 0   ; set memory entries counter to zero
    xor ebx, ebx	; set zero
    xor eax, eax	; set zero
loop:
    mov ax, 0xe820	; set int 0x15 command number
    mov edx, MAGIC_NUM	; apparently in some bios this will trash so we reset it
    mov ecx, ENTRY_SIZE	; after the intrrupt this might change to 20	
			; so we reset it every time
    int 0x15
    cmp bx, 0		
    je memtest_done	; if bx is set to zero means end of list if not then
    jc memtest_done	; if carry bit is set also means end of list
    add dword [MEM_ENT_CNT], 1   ; add to counter for a new entry
    add di, ENTRY_SIZE ;
    jmp loop

memtest_fail:
    mov bx, FAILED_MSG 
    call printF
    jmp $

memtest_done:
    mov eax, dword [MEM_ENT_CNT]
    cmp eax, 0				; if number of entries is zero means memtest failed
    je memtest_fail
    popa
    ret

FAILED_MSG:
    db "Memory test failed", 10, 13, 0
