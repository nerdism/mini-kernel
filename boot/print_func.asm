

printF:
	mov ah, 0x0e ; bios video card print font 
	mov al, [bx] ; put bx pointer content to al
	int 0x10     ; bios video card intrrupt
	add bx, 1    ; 
	cmp al, 0    ; if al was zero which means end of the string
	jne printF   ; jump to the begininng of the code
	ret          ; retrieve from the function



printHex:
	mov cl, 0x00 ; make counter
	mov ah, 0x0e ; bios video card print font 
	mov al, '0'
	int 0x10      ; biod video card intrrupt
	mov al, 'X'
	int 0x10
_printHex:
	mov bl, dh		
	shr bl, 4	; shif 4 bit to the right
	cmp bl, 10	; if bl content is less than 10 means 
        jl _print_num		; it's a number 
	add bl, 87
	jmp _printHexCont

_print_num:
	add bl, 48

_printHexCont:
	mov al, bl
	int 0x10
	mov bl, dh
	and bl, 0x0f
	cmp bl, 10
	jl _print_num2
	add bl, 87
	jmp _printHexCont2

_print_num2:
	add bl, 48


_printHexCont2:
	mov al, bl
	int 0x10
	mov dh, dl
	add cl, 1
	cmp cl, 2	; if counter is 2 which means we have printed a word
	jne _printHex	; then retrieve if not then jump to begininng and print the other byte
	ret
