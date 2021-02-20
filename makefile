# 13519214

all:
	kernelgcc

kernelgcc:
	gcc -c -Wall -Wextra -o out/compiledgcc.o src/kernel.c

cleangcc:
	rm out/compiledgcc.o
