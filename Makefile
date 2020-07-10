
CC=gcc -m32
LD=ld -melf_i386 
CCOPTIONS= -Wall -ffreestanding -fno-pie -c -g -std=gnu99 
BOOT_FILES=$(wildcard boot/*.asm)
KASM=$(wildcard kernel/*.asm)
SRCS=$(wildcard kernel/*.c drivers/*.c)

OBJS=${SRCS:.c=.o}

os-image: boot_sect.bin kernel.bin
	@cat $^ > $@
	@echo "os-image 	has been built"

boot_sect.bin:	boot/boot_sect.asm $(BOOT_FILES)
	@nasm -I boot/ -f bin $< -o $@
	@echo "boot_sect.bin	has been built"


kernel.bin:  kernel_entry.o ${OBJS}
	@$(LD) --oformat binary -Ttext 0x1000 $^ -o $@
	@echo "kernel.bin 	has been built"

#kernel.o: kernel/kernel.c
#	@$(CC) $(CCOPTIONS) $< -o $@
#	@echo "kernel.o 	has been built"

%.o: %.c
	@$(CC) $(CCOPTIONS) -I include $< -o $@
	@echo "$@ 	has been built"

kernel_entry.o: kernel/kernel_entry.asm ${KASM}
	@nasm -I kernel/ -f elf $< -o $@
	@echo "kernel_entry.o 	has been built"


clean:
	@find . -name "*.o" -type f -delete
	@find . -name "*.bin" -type f -delete
	@rm os-image
	@echo "removed os-image *.o and *.bin files"
