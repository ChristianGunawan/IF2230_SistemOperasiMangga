// 13519214 - Kernel header

// Configuration
#define V_MEM 0xB000
#define V_OFFSET 0x8000
#define V_GRAPHIC 0xA000

// Implemented in assembly
extern void putInMemory(int segment, int address, char character);
extern void makeInterrupt21();
extern int interrupt(int number, int AX, int BX, int CX, int DX);


void handleInterrupt21 (int AX, int BX, int CX, int DX);
// Interupt 21 Handler

void printString(char *string);
// System call for char print

void readString(char *string);
// System call for char input

void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
// Clearing string buffer

void videoMemoryWrite(int offset, char character);
// putInMemory() wrapper for video memory writing

void clearScreen();
// Wipe entire screen

void setCursorPos(int r, int c);
// Move cursor position
// Note : Row and Column start from 0
