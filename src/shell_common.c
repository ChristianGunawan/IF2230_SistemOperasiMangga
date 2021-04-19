#include "kernel-header/config.h"
#include "std-header/std_fileio.h"

extern int interrupt(int number, int AX, int BX, int CX, int DX);

void getDirectoryTable(char *buffer) {
    // WARNING : Naive implementation
    interrupt(0x21, 0x0002, buffer, FILES_SECTOR, 0);
    interrupt(0x21, 0x0002, buffer + SECTOR_SIZE, FILES_SECTOR + 1, 0);
}

void shellReturn() {
    int AX = BIN_PARENT_FOLDER << 8;
    int ret_code;
    AX |= 0x06;
    interrupt(0x21, AX, "mash", 0x2000, &ret_code);
}

void getShellCache(char *buffer) {
    int returncode;
    read(buffer, "_mash_cache", &returncode, ROOT_PARENT_FOLDER);
}

void setShellCache(char *buffer) {
    // int returncode;
    // write(buffer, "_mash_cache", &returncode, ROOT_PARENT_FOLDER);
}
