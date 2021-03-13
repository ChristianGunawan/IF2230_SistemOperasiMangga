// 13519214 - Kernel header

#include "config.h"


// --- Internal Filesystem Configuration ---
#define SECTOR_SIZE 512
// Kernel Maximum Size, if changed ensure dd commands in makefile and bootloader.asm set properly
#define KERNEL_SECTOR_SIZE 15
// Macro for filesystem syscalls
#define FILE_SECTOR_SIZE 0x10 // 16 sectors (8192 bytes) for 1 file entry
#define MAP_SECTOR 0x100
#define FILES_SECTOR 0x101
#define SECTORS_SECTOR 0x103

// Predefined values in map filesystem
#define EMPTY_MAP_ENTRY 0x00 // For empty entry
#define FILLED_MAP_ENTRY 0xFF // If sector are filled

// Flags in files filesystem
#define ROOT_PARENT_FOLDER 0xFF // Flag for "P" byte
#define EMPTY_FILES_ENTRY 0xFE // Flag for "S" byte
#define FOLDER_ENTRY 0xFF // Flag for "S" byte
#define PARENT_BYTE_OFFSET 0x0 // "P" byte, parent folder index
#define ENTRY_BYTE_OFFSET 0x1 // "S" byte, entry index at sectors filesystem

// Predefined values in sectors filesystem
#define EMPTY_SECTORS_ENTRY 0x00 // For empty entry

// INT 21H Handler
// TODO : Add


// Implemented in assembly
extern void putInMemory(int segment, int address, char character);
extern void makeInterrupt21();
extern int interrupt(int number, int AX, int BX, int CX, int DX);

void handleInterrupt21(int AX, int BX, int CX, int DX);
// Interupt 21H Handler

void printString(char *string);
// System call for string print

void readString(char *string);
// System call for string input

void clear(char *buffer, int length); // Fungsi untuk mengisi buffer dengan 0
// Clearing string buffer

void readSector(char *buffer, int sector);
// Reading file at sector and copy to buffer

void writeSector(char *buffer, int sector);
// Writing buffer at sector

void readFile(char *buffer, char *path, int *result, char parentIndex);
// Read file with relative path

void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
// Writing file with relative path
