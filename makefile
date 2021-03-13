# 13519214 - Makefile
all: diskimage bootloader kernel insertfilesystem

clean:
	rm out/*

test: kernelgcc

cleantest: cleangcc



# Main recipes
diskimage:
	dd if=/dev/zero of=out/mangga.img bs=512 count=2880

bootloader:
	nasm src/asm/bootloader.asm -o out/bootloader;
	dd if=out/bootloader of=out/mangga.img bs=512 count=1 conv=notrunc

kernel:
	bcc -ansi -c -o out/kernel.o src/kernel.c
	bcc -ansi -c -o out/std.o src/std.c
	bcc -ansi -c -o out/screen.o src/screen.c
	bcc -ansi -c -o out/shell.o src/shell.c
	bcc -ansi -c -o out/output.o src/output.c
	nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	ld86 -o out/kernel -d out/*.o # Linking
	dd if=out/kernel of=out/mangga.img bs=512 conv=notrunc seek=1

createfilesystem:
	dd if=/dev/zero of=out/fs/map.img bs=512 count=1
	dd if=/dev/zero of=out/fs/files.img bs=512 count=2
	dd if=/dev/zero of=out/fs/sectors.img bs=512 count=1

insertfilesystem:
	dd if=out/fs/map.img of=out/mangga.img bs=512 count=1 seek=256 conv=notrunc
	dd if=out/fs/files.img of=out/mangga.img bs=512 count=2 seek=257 conv=notrunc
	dd if=out/fs/sectors.img of=out/mangga.img bs=512 count=1 seek=259 conv=notrunc




# Test recipes
kernelgcc:
	gcc -c -Wall -Wextra -o out/compiledgcc.o src/kernel.c

cleangcc:
	rm out/compiledgcc.o
