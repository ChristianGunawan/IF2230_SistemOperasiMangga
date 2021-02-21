// ------- Kernel -------
// BCC notes
// - Variable declaration must be put on top of code
// - First defined function is starting function
// - Pointer declaration syntax is <type> *<varname>;

// Configuration
#define V_MEM 0xB000
#define V_OFFSET 0x8000

#include "kernel.h"
#include "std.h"

int main() {
    // TODO : Extra, ASCII
    char *s;
    char stringBuffer[1024];

    clear(stringBuffer, 1024);
    readString(stringBuffer);
    printString(stringBuffer);

    while (1);
}

void handleInterrupt21(int AX, int BX, int CX, int DX){
    switch (AX) {
        // case 0x0:
        //     printString(BX);
        //     break;
        // case 0x1:
        //     // readString(BX);
        //     break;
        default:
            printString("Invalid interrupt");
    }
}

void videoMemoryWrite(int offset, char character) {
    putInMemory(V_MEM, V_OFFSET + offset, character);
}

void printString(char *string) {
    int i = 0;
    while (string[i] != '\0') {
        videoMemoryWrite(2*i, string[i]);
        videoMemoryWrite(1 + 2*i, 0xD); // DEBUG
        i++;
    }

    // interrupt(0x10,0x1301,0x010D,strlen(string),0);
}

void readString(char *string) {
    // Temporary read string
    char c = interrupt(0x16, 0x00, 0, 0, 0);
    int i = 1;
    string[0] = c;
    while (c != 0xD) {
        // ASCII 0xD -> Carriage Return
        c = interrupt(0x16, 0x00, 0, 0, 0);
        if (c != 0xD) {
            string[i] = c;
            i++;
        }
    }
    string[i] = '\0';
}

void clear(char *string, int length) {
    int i = 0;
    while (i < length) {
        string[i] = '\0';
        i++;
    }
}
