# 13519214 - Makefile
all: diskimage bootloader kernel

clean:
	rm out/*

test: kernelgcc

cleantest: cleangcc



# Main recipes
diskimage:
	dd if=/dev/zero of=out/mangga.img bs=512 count=2880

bootloader:
	nasm src/bootloader.asm -o out/bootloader;
	dd if=out/bootloader of=out/mangga.img bs=512 count=1 conv=notrunc

kernel:
	bcc -ansi -c -o out/kernel.o src/kernel.c
	bcc -ansi -c -o out/std.o src/std.c
	nasm -f as86 src/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/*.o 
	dd if=out/kernel of=out/mangga.img bs=512 conv=notrunc seek=1



# Test recipes
kernelgcc:
	gcc -c -Wall -Wextra -o out/compiledgcc.o src/kernel.c

cleangcc:
	rm out/compiledgcc.o
