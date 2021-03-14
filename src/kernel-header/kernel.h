// 13519214 - Kernel header

#include "config.h"


// --- Internal Filesystem Configuration ---
// ~ Root directory is located at "/"
// ~ Empty sector in map filesystem is equal with EMPTY_MAP_ENTRY
// ~ Empty entry in files filesystem is flagged by S byte
//         equal EMPTY_FILES_ENTRY and P byte equal ROOT_PARENT_FOLDER
// ~ Empty entry in sectors filesystem if and only if entire 16 bytes
//         is equal with EMPTY_SECTORS_ENTRY

// Bytes count in 1 sector
#define SECTOR_SIZE 512

// Kernel Maximum Size, if changed ensure dd commands in makefile,
//                      bootloader.asm, and fscreator set properly
#define KERNEL_SECTOR_SIZE 31

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
#define PATHNAME_BYTE_OFFSET 0x2 // 14 bytes, filled with pathnames

// Predefined values in sectors filesystem
#define EMPTY_SECTORS_ENTRY 0x00 // For empty entry

// INT 21H Handler
// TODO : Add


// --- Kernel Behavior ---
// ~ Kernel will assume exists external procedure called shell()
//          and will spawning shell after kernel setup complete.

// Implemented in assembly
extern void putInMemory(int segment, int address, char character);
extern void makeInterrupt21();
extern int interrupt(int number, int AX, int BX, int CX, int DX);

// External procedure call
extern void shell();

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
// -- Error code list --
// -1 - File not found

void writeFile(char *buffer, char *path, int *sectors, char parentIndex);
// Writing file with relative path
// -- Error code list --
// 0 - Exit successfully
// -1 - File exists
// -2 - Not enough entry in files filesystem
// -3 - Not enough empty sectors
// -4 - Invalid folder
// -- writeFile() services --
// buffer is file entry to be written
// path is filename, only maximum 14 char
// sectors will be used as error code container
// parentIndex used as "P" byte value / parent index at files filesystem entry
// If buffer == NULL, creating folder instead file
// TODO : Add service list
