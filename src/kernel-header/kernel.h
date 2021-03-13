// 13519214 - Kernel header

#include "config.h"

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

// readFile();
// writeFile();
// loadFile();
