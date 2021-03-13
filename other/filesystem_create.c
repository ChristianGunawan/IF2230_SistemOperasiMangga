// 13519214 - Simple binary file writer
#include <stdio.h>
#include <stdlib.h>

#define KERNEL_MAX 11

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage : fscreate <map path> <file path> <sector path>\n");
        exit(1);
    }
    FILE *map = fopen(argv[1], "wb");
    FILE *file = fopen(argv[2], "wb");
    FILE *sector = fopen(argv[3], "wb");

    for (int i = 0; i < 512; i++) {
        if (i < KERNEL_MAX)
            fwrite("\xFF", 1, 1, map);
        else
            fwrite("\x00", 1, 1, map);
        fwrite("\x00\x00", 2, 1, file);
        fwrite("\x00", 1, 1, sector);
    }

    fclose(map);
    fclose(file);
    fclose(sector);
    return 0;
}
