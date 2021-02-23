// ------- Kernel -------
// BCC notes
// - Variable declaration must be put on top of code
// - First defined function is starting function
// - Pointer declaration syntax is <type> *<varname>;

#include "kernel.h"
#include "std.h"
#include "other.h"

int main() {
    // Declaration
    char stringBuffer[1024];
    int temp;

    // Setup
    clear(stringBuffer, 1024);
    makeInterrupt21();

    // Initial screen
    clearScreen();
    // interrupt(0x10, 0x0003, 0, 0, 0);
    // setCursorPos(1,0);
    // printString("Hello World!");
    // setCursorPos(0,0);
    drawBootLogo();
    interrupt(0x10, 0x0003, 0, 0, 0);

    // Other
    setCursorPos(1,0);
    interrupt(0x21, 0x0, "Output :", 0, 0);
    while (1) {
        setCursorPos(0,0);
        interrupt(0x21, 0x1, stringBuffer, 0, 0);
        setCursorPos(0,0);
        interrupt(0x21, 0x0, "                        ", 0, 0);
        setCursorPos(2,0);
        interrupt(0x21, 0x0, "                        ", 0, 0);
        setCursorPos(2,0);
        interrupt(0x21, 0x0, stringBuffer, 0, 0);
    }
}

void handleInterrupt21(int AX, int BX, int CX, int DX){
    switch (AX) {
        case 0x0:
            printString(BX);
            break;
        case 0x1:
            readString(BX);
            break;
        default:
            printString("Invalid interrupt");
    }
}

void videoMemoryWrite(int offset, char character) {
    putInMemory(V_MEM, V_OFFSET + offset, character);
}

void printString(char *string) {
    // -- Direct video memory writing --
    // int i = 0;
    // while (string[i] != '\0') {
    //     videoMemoryWrite(2*i, string[i]);
    //     videoMemoryWrite(1 + 2*i, 0xD);
    //     i++;
    // }

    // -- Position based print --
    // Interrupt 0x10 for print 1 char
    // AX = Mode, BX = Color, CX = Print n times
    // TODO : Get cursor pos (?)
    // int i = 0, temp = 0, col = 0;
    // while (string[i] != '\0') {
    //     temp = 0x0900 | string[i];
    //     setCursorPos(1, col);
    //     interrupt(0x10, temp, 0x0007, 0x1, 0);
    //     col++;
    //     i++;
    // }
    // setCursorPos(0, 0);

    // -- TTY based print --
    // AL = Char, BH = Page, BL = Color
    int i = 0, temp = 0;
    while (string[i] != '\0') {
        temp = 0x0E00 | string[i];
        interrupt(0x10, temp, 0x0007, 0, 0);
        i++;
    }
}

void readString(char *string) {
    char c;
    int i = 0, col = 0, temp = 0;
    // Enabling Cursor
    interrupt(0x10, 0x0100, 0, 0x000F, 0);

    do {
        // Note : ASCII 0xD -> Carriage Return
        c = interrupt(0x16, 0x00, 0, 0, 0);
        switch (c) {
            case 0xD:
                break;
            case 0x8:
                if (i > 0)
                    i--;

                interrupt(0x10, 0x0920, 0, 0x1, 0);
                if (col > 0) {
                    col--;
                    setCursorPos(0, col);
                }
                // 0x20 -> Space
                break;
            default:
                string[i] = c;
                temp = 0x0900 | c;
                setCursorPos(0, col);
                interrupt(0x10, temp, 0x0007, 0x1, 0);
                i++;
                col++;
        }
    } while (c != 0xD);
    setCursorPos(0, 0);
    string[i] = '\0';
    // Disabling cursor
    interrupt(0x10, 0x0100, 0, 0x3F00, 0);
}

void clear(char *string, int length) {
    int i = 0;
    while (i < length) {
        string[i] = '\0';
        i++;
    }
}

void clearScreen() {
    // Switch to real mode
    int i = 0;
    while (i < 4096) {
        videoMemoryWrite(2*i, ' ');
        videoMemoryWrite(1 + 2*i, 0x0);
        i++;
    }
}

void setCursorPos(int r, int c) {
    int temp = 0x100*r + c;
    interrupt(0x10, 0x0200, 0x1, 0, temp);
}
