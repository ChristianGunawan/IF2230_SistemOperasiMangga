// TODO : Recheck kernel size

#include "header/shell.h"
#include "header/kernel.h" // TODO : Split getCursorPos
#include "header/std.h"


void shell() {
    char stringBuffer[1024];
    stringBuffer[0] = '\0';
    printString("OK\n");
    while (1) {
        readString(stringBuffer);
        // interrupt(0x21, 0x0, "location x ", 0, 0);
        // stringBuffer[0] = mod(getCursorPos(), 10) + 0x30;
        // interrupt(0x21, 0x0, stringBuffer, 0, 0);
        //
        // interrupt(0x21, 0x0, "location y ", 0, 0);
        // stringBuffer[0] = mod(getCursorPos()/0x100, 10) + 0x30;
        // interrupt(0x21, 0x0, stringBuffer, 0, 0);
    }

}
