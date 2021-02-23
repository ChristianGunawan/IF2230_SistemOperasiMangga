// 13519214 - Other function

#include "kernel.h"
#include "other.h"

void drawPixel(int x, int y, int color) {
    putInMemory(V_GRAPHIC, x+y*320, color);
}

void drawRectangle(int x, int y, int w, int h) {
    int a = x, b = y;

    while (a <= x + w) {
        drawPixel(a, y, 0xF);
        drawPixel(a, y+h, 0xF);
        a++;
    }
    while (b <= y + h) {
        drawPixel(x, b, 0xF);
        drawPixel(x + w, b, 0xF);
        b++;
    }
}

void bitDraw(int xs, int ys, int color, char *bitarray) {
    int x = xs, y = ys, i = 0;
    while (bitarray[i] != '\0') {
        switch (bitarray[i]) {
            case '0':
                x++;
                break;
            case '1':
                drawPixel(x, y, color);
                x++;
                break;
            case 'n':
                x = xs;
                y++;
                break;
        }
        i++;
    }
}

void clearScreen() {
    // Switch to real mode
    int i = 0;
    while (i < 4096) {
        charVideoMemoryWrite(2*i, ' ');
        charVideoMemoryWrite(1 + 2*i, 0x0);
        i++;
    }
}

void setCursorPos(int r, int c) {
    int temp = 0x100*r + c;
    interrupt(0x10, 0x0200, 0x1, 0, temp);
}

void charVideoMemoryWrite(int offset, char character) {
    putInMemory(V_MEM, V_OFFSET + offset, character);
}

void drawBootLogo() {
    // TODO : Use raw binary
    int i = 0, j = 0;
    char *logo = LOGO_STRING;

    interrupt(0x10, 0x0013, 0, 0, 0);
    // Change video mode to 0x13, Graphical 320x200

    bitDraw(LOGO_X_OFFSET + 10, LOGO_Y_OFFSET + 10, 0xE, logo);
    printString("\n\n\n\n\n  mangga");
    drawRectangle(LOGO_X_OFFSET + 4, LOGO_Y_OFFSET + 6, 54, 44);
    // Draw logo with offset and color preset


    drawRectangle(LOADING_X_OFFSET - 2, LOADING_Y_OFFSET - 1, 67, 8);
    drawRectangle(LOADING_X_OFFSET - 4, LOADING_Y_OFFSET - 1, 71, 8);
    // Loading bar boundary
    while (i < 4096) {
        // Delaying loop and loading bar
        if (mod(i,64) == 0) {
            // Loading bar
            drawPixel(LOADING_X_OFFSET + i / 64, LOADING_Y_OFFSET + 1, 0x09);
            drawPixel(LOADING_X_OFFSET + i / 64, LOADING_Y_OFFSET + 2, 0x09);
            drawPixel(LOADING_X_OFFSET + i / 64, LOADING_Y_OFFSET + 3, 0x09);
            drawPixel(LOADING_X_OFFSET + i / 64, LOADING_Y_OFFSET + 4, 0x09);
            drawPixel(LOADING_X_OFFSET + i / 64, LOADING_Y_OFFSET + 5, 0x09);
        }
        if (j > 1024) {
            j = 0;
            i++;
        }
        j++;
    }
}
