// 13519214 - Kernel header

// Configuration
#define V_MEM 0xB000
#define V_OFFSET 0x8000
#define V_GRAPHIC 0xA000
#define CHAR_INPUT_NEWLINE CHAR_CARRIAGE_RETURN

// ASCII Charset macro
#define CHAR_LINEFEED 0xA
#define CHAR_CARRIAGE_RETURN 0xD
#define CHAR_BACKSPACE 0x8
#define CHAR_NULL 0x0
#define CHAR_SPACE 0x20

// BIOS Color Enum
// Note : Probably bcc doesn't have enum keyword
#define BIOS_BLACK 0x0
#define BIOS_BLUE 0x1
#define BIOS_GREEN 0x2
#define BIOS_CYAN 0x3
#define BIOS_RED 0x4
#define BIOS_MAGENTA 0x5
#define BIOS_BROWN 0x6
#define BIOS_GRAY 0x7

#define BIOS_DARK_GRAY 0x8
#define BIOS_LIGHT_BLUE 0x9
#define BIOS_LIGHT_GREEN 0xA
#define BIOS_LIGHT_CYAN 0xB
#define BIOS_LIGHT_RED 0xC
#define BIOS_LIGHT_MAGENTA 0xD
#define BIOS_YELLOW 0xE
#define BIOS_WHITE 0xF




// Implemented in assembly
extern void putInMemory(int segment, int address, char character);
extern void makeInterrupt21();
extern int interrupt(int number, int AX, int BX, int CX, int DX);
extern int getRawCursorPos(); // TODO : Split to another asm file

void handleInterrupt21 (int AX, int BX, int CX, int DX);
// Interupt 21H Handler

void printString(char *string);
// System call for char print

void readString(char *string);
// System call for char input

void clear(char *buffer, int length); //Fungsi untuk mengisi buffer dengan 0
// Clearing string buffer

void readSector(char *buffer, int sector);
// Reading file at sector and copy to buffer

void writeSector(char *buffer, int sector);
// Writing buffer at sector

// readFile();
// writeFile();
// loadFile();
