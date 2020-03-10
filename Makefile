
CC=gcc -m32
LD=ld -melf_i386
CCOPTIONS= -ffreestanding -fno-pie -c

os-image: boot_sect.bin kernel.bin
	@cat $^ > $@
	@echo "os-image 	has been built"

boot_sect.bin:	boot/boot_sect.asm
	@nasm -I boot/ -f bin $^ -o $@
	@echo "boot_sect.bin	has been built"

kernel.bin: kernel.o kernel_entry.o	
	@$(LD) --oformat binary -Ttext 0x1000 $^ -o $@
	@echo "kernel.bin 	has been built"

kernel.o: kernel/kernel.c
	@$(CC) $(CCOPTIONS) $^ -o $@
	@echo "kernel.o 	has been built"

kernel_entry.o: kernel/kernel_entry.asm
	@nasm -f elf $^ -o $@
	@echo "kernel_entry.o 	has been built"


clean:
	@rm *.o *.bin os-image
	@echo "removed os-image *.o and *.bin files"
