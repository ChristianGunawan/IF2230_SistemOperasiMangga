// 13519214 - Simple binary file writer
// v1.1 - Adding 0xFF to file system itself
// v1.2 - Splitting loop and adding flag system, check kernel.h config
#include <stdio.h>
#include <stdlib.h>

#define KERNEL_MAX 16

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage : fscreate <map path> <file path> <sector path>\n");
        exit(1);
    }
    FILE *map = fopen(argv[1], "wb");
    FILE *file = fopen(argv[2], "wb");
    FILE *sector = fopen(argv[3], "wb");

    for (int i = 0; i < 512; i++) {
        if (i < KERNEL_MAX || (i >= 0x100 && i <= 0x103))
            fwrite("\xFF", 1, 1, map);
        else
            fwrite("\x00", 1, 1, map);
    }

    for (int i = 0; i < 64; i++) {
        fwrite("\xFF\xFE\x00\x00\x00\x00\x00\x00", 8, 1, file); // Empty file flags and root parent folder
        fwrite("\x00\x00\x00\x00\x00\x00\x00\x00", 8, 1, file); // Empty filename
    }

    for (int i = 0; i < 512; i++) {
        fwrite("\x00", 1, 1, sector);
    }

    fclose(map);
    fclose(file);
    fclose(sector);
    return 0;
}
