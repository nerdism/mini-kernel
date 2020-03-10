
disk_load:
	push dx

	mov ah, 0x02 ; bios read sector function
	mov al, dh   ; read dh sectors
	mov ch, 0x00 ; select cylinder 0
	mov dh, 0x00 ; select head 0
	mov cl, 0x02 ; start reading from the second sector

	int 0x13 ; bios hard driver intrupt

	jc disk_error	; jump if carry bit was one

	pop dx
	cmp dh, al
	jne disk_error
	ret



disk_error:
	mov bx, DISK_ERROR_MSG
	call printF
	jmp $        ; jump to the current command 

; variables 
DISK_ERROR_MSG:
	db "disk read error", 0
