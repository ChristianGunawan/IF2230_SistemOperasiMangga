// ------- Kernel -------
// BCC notes
// - Variable declaration must be put on top of code
// - First defined function is starting function
// - Pointer declaration syntax is <type> *<varname>;

#include "header/kernel.h"
#include "header/std.h"
#include "header/other.h"
#include "header/shell.h"

int main() {
    // Declaration
    char stringBuffer[1024];
    int temp;

    // Setup
    clear(stringBuffer, 1024);
    makeInterrupt21();

    // Initial screen
    clearScreen();
    // drawBootLogo();

    // Print & Read string loop
    interrupt(0x10, 0x0003, 0, 0, 0);
    shell();
    // setCursorPos(1,0);
    // interrupt(0x21, 0x0, "Output :", 0, 0);
    while (1) {
        // setCursorPos(0,0);
        // interrupt(0x21, 0x1, stringBuffer, 0, 0);
        // setCursorPos(0,0);
        // interrupt(0x21, 0x0, "                        ", 0, 0);
        // setCursorPos(2,0);
        // interrupt(0x21, 0x0, "                        ", 0, 0);
        // setCursorPos(2,0);
        // interrupt(0x21, 0x0, stringBuffer, 0, 0);
    }
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
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

void printString(char *string) {
    printColoredString(string, BIOS_GRAY);
}

void readString(char *string) {
    // TODO : Literally gets()
    // TODO : Extra, arrow keys control char
    char c;
    int i = 0, col = 0, temp = 0, savedCursorRow, savedCursorCol;
    enableKeyboardCursor();
    savedCursorRow = getCursorPos(1);
    savedCursorCol = getCursorPos(0);

    do {
        c = interrupt(0x16, 0x00, 0, 0, 0);
        switch (c) {
            case CHAR_INPUT_NEWLINE:
                break;
            case CHAR_BACKSPACE:
                if (i > 0)
                    i--;

                directCharPrint(CHAR_SPACE, BIOS_BLACK);
                if (col > 0) {
                    col--;
                    setCursorPos(savedCursorRow, col);
                }

                break;
            default:
                setCursorPos(savedCursorRow, col);
                directCharPrint(c, BIOS_LIGHT_BLUE);
                i++;
                col++;
        }
    } while (c != CHAR_INPUT_NEWLINE);
    string[i] = CHAR_NULL; // Terminating string
    disableKeyboardCursor();
    setCursorPos(savedCursorRow + 1,savedCursorCol);
}

void clear(char *string, int length) {
    int i = 0;
    while (i < length) {
        string[i] = CHAR_NULL;
        i++;
    }
}
