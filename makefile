# 13519214 - Makefile
all: diskimage bootloader kernel createfilesystem insertfilesystem fileloader mash logoinsert

clean:
	# -- Cleaning output files --
	@rm out/fs/*
	@rm out/*
	@rm out/asm/*
	@rm out/shell/*

test: kernelgcc

cleantest: cleangcc

mash:
	if [ ! -d "out/shell" ]; then mkdir out/shell; fi
	@bcc -ansi -c -o out/shell/mash.o src/mash.c
	@bcc -ansi -c -o out/shell/std_stringio.o src/std_stringio.c
	@bcc -ansi -c -o out/shell/std_fileio.o src/std_fileio.c
	@bcc -ansi -c -o out/shell/shell_common.o src/shell_common.c
	@bcc -ansi -c -o out/shell/std_opr.o src/std_opr.c
	if [ ! -d "out/shell/asm" ]; then mkdir out/shell/asm; fi
	@nasm -f as86 src/asm/interrupt.asm -o out/shell/asm/interrupt.o
	@ld86 -o out/mash -d out/shell/*.o out/shell/asm/interrupt.o
	@cd out; ./loadFile mangga.img mash 0

insertls:
	if [ ! -d "out/shell/ls" ]; then mkdir out/shell/ls; fi
	@bcc -ansi -c -o out/shell/ls/ls.o src/ls.c
	@nasm -f as86 src/asm/interrupt.asm -o out/shell/asm/interrupt.o
	@ld86 -o out/ls -d out/shell/ls/*.o out/shell/std_fileio.o \
			out/shell/std_stringio.o out/shell/shell_common.o \
	 		out/shell/std_opr.o out/shell/asm/interrupt.o
	@cd out; ./loadFile mangga.img ls 0

logoinsert:
	@cp other/logo.hoho out/logo.hoho
	@cd out; ./loadFile mangga.img logo.hoho 255

# Main recipes
diskimage:
	# -- Initial mangga.img --
	if [ ! -d "out" ]; then mkdir out; fi
	@dd if=/dev/zero of=out/mangga.img bs=512 count=2880 status=noxfer

bootloader:
	# -- Bootloader insertion --
	@nasm src/asm/bootloader.asm -o out/bootloader;
	@dd if=out/bootloader of=out/mangga.img bs=512 count=1 conv=notrunc status=noxfer

kernel:
	# -- Source Compilation --
	@bcc -ansi -c -o out/kernel.o src/kernel.c
	@bcc -ansi -c -o out/std_stringio.o src/std_stringio.c
	@bcc -ansi -c -o out/std_fileio.o src/std_fileio.c
	@bcc -ansi -c -o out/screen.o src/screen.c
	@bcc -ansi -c -o out/output.o src/output.c
	@bcc -ansi -c -o out/std_opr.o src/std_opr.c
	@nasm -f as86 src/asm/kernel.asm -o out/kernel_asm.o
	if [ ! -d "out/asm" ]; then mkdir out/asm; fi
	@nasm -f as86 src/asm/interrupt.asm -o out/asm/interrupt.o
	ld86 -o out/kernel -d out/*.o out/asm/interrupt.o
	# ------------ Compiled kernel stat ------------
	# Max Kernel Size : 8192 bytes (16 sectors, 1 sector = 512 bytes)
	@stat --printf="Kernel Size : %s bytes\n" out/kernel
	# ----------------------------------------------
	@dd if=out/kernel of=out/mangga.img bs=512 conv=notrunc seek=1 status=noxfer

createfilesystem:
	if [ ! -d "out/fs" ]; then mkdir out/fs; fi
	@./other/fscreate out/fs/map.img out/fs/files.img out/fs/sectors.img

insertfilesystem:
	# -- Filesystem insertion --
	@dd if=out/fs/map.img of=out/mangga.img bs=512 count=1 seek=256 conv=notrunc status=noxfer
	@dd if=out/fs/files.img of=out/mangga.img bs=512 count=2 seek=257 conv=notrunc status=noxfer
	@dd if=out/fs/sectors.img of=out/mangga.img bs=512 count=1 seek=259 conv=notrunc status=noxfer

filesystemcreator:
	if [ ! -d "other" ]; then mkdir other; fi
	@gcc -Wall -Wextra -O3 -o other/fscreate other/filesystem_create.c
	chmod +x other/fscreate

fileloader:
	@gcc -Wall -Wextra -O3 -o out/loadFile other/fileloader.c


# Test recipes
kernelgcc:
	gcc -c -Wall -Wextra -o out/compiledgcc.o src/kernel.c

cleangcc:
	rm out/compiledgcc.o
