// 13519214 - Standard function

#include "std-header/std_fileio.h"
#include "std-header/boolean.h"
#include "kernel-header/config.h"

// ---------------- File I/O ----------------
void write(char *buffer, char *path, int *returncode, char parentIndex) {
    int AX = parentIndex << 8;
    AX |= 0x05;
    interrupt(0x21, AX, buffer, path, returncode);
}

void read(char *buffer, char *path, int *returncode, char parentIndex) {
    int AX = parentIndex << 8;
    AX |= 0x04;
    interrupt(0x21, AX, buffer, path, returncode);
}

void directSectorWrite(char *buffer, int sector) {
    interrupt(0x21, 0x03, buffer, sector, 0);
}

// ---------------- Misc ----------------
void memcpy(char *dest, char *src, int bytes) {
    int i = 0;
    while (i < bytes) {
        dest[i] = src[i];
        i++;
    }
}
