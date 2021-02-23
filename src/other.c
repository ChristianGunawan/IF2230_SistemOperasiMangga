// 13519214 - Other function

#include "kernel.h"

// TODO : Cleanup
void drawRectangle(int x, int y, int w, int h) {
    int a = x, b = y;

    while (a < 320) {
        b = y;
        while (b < 200) {
            if ((a < x + w) && (b < y + h))
                putInMemory(V_GRAPHIC, a+b*320, 0x07);
            b++;
        }
        a++;
    }

}

void drawPixel(int x, int y) {
    putInMemory(V_GRAPHIC, x+y*320, 0x07);
    // interrupt(0x10, 0x0C07, 0, x, y);
}

void drawBootLogo() {
    int a = 0, b = 0, j = 0, k = 0, temp = 0;
    interrupt(0x10, 0x0013, 0, 0, 0);
    // Draw Logo
    // 320x200
    // drawRectangle(50, 100, 20, 20);
    // while (a < 320) {
    // // putInMemory(0xA000, 0, 0x07);
    // // putInMemory(0xA000, 640, 0x07);
    //
    //     b = 0;
    //     while (b < 200) {
    //         if (mod(b, 16) == 0)
    //             putInMemory(V_GRAPHIC, a+b*320, 0x07);
    //         // putInMemory(V_GRAPHIC, a+b*320, 0x07);
            // interrupt(0x10, 0x0C07, 0, 5, 6);
    //         b++;
    //     }
    //     a++;
    // }


    drawPixel(15,0);
    drawPixel(16,0);
    drawPixel(18,0);
    drawPixel(19,0);
    drawPixel(20,0);
    drawPixel(11,1);
    drawPixel(13,1);
    drawPixel(14,1);
    drawPixel(15,1);
    drawPixel(16,1);
    drawPixel(17,1);
    drawPixel(21,1);
    drawPixel(4,2);
    drawPixel(6,2);
    drawPixel(7,2);
    drawPixel(8,2);
    drawPixel(9,2);
    drawPixel(11,2);
    drawPixel(12,2);
    drawPixel(13,2);
    drawPixel(14,2);
    drawPixel(16,2);
    drawPixel(17,2);
    drawPixel(18,2);
    drawPixel(19,2);
    drawPixel(20,2);
    drawPixel(21,2);
    drawPixel(22,2);
    drawPixel(26,2);
    drawPixel(3,3);
    drawPixel(4,3);
    drawPixel(5,3);
    drawPixel(6,3);
    drawPixel(7,3);
    drawPixel(8,3);
    drawPixel(9,3);
    drawPixel(11,3);
    drawPixel(15,3);
    drawPixel(16,3);
    drawPixel(17,3);
    drawPixel(18,3);
    drawPixel(20,3);
    drawPixel(21,3);
    drawPixel(23,3);
    drawPixel(26,3);
    drawPixel(27,3);
    drawPixel(3,4);
    drawPixel(4,4);
    drawPixel(5,4);
    drawPixel(7,4);
    drawPixel(8,4);
    drawPixel(9,4);
    drawPixel(10,4);
    drawPixel(12,4);
    drawPixel(14,4);
    drawPixel(15,4);
    drawPixel(16,4);
    drawPixel(17,4);
    drawPixel(18,4);
    drawPixel(19,4);
    drawPixel(20,4);
    drawPixel(25,4);
    drawPixel(26,4);
    drawPixel(28,4);
    drawPixel(2,5);
    drawPixel(3,5);
    drawPixel(4,5);
    drawPixel(5,5);
    drawPixel(6,5);
    drawPixel(7,5);
    drawPixel(8,5);
    drawPixel(9,5);
    drawPixel(10,5);
    drawPixel(13,5);
    drawPixel(14,5);
    drawPixel(16,5);
    drawPixel(18,5);
    drawPixel(26,5);
    drawPixel(2,6);
    drawPixel(3,6);
    drawPixel(4,6);
    drawPixel(5,6);
    drawPixel(6,6);
    drawPixel(7,6);
    drawPixel(10,6);
    drawPixel(11,6);
    drawPixel(12,6);
    drawPixel(13,6);
    drawPixel(20,6);
    drawPixel(24,6);
    drawPixel(25,6);
    drawPixel(29,6);
    drawPixel(3,7);
    drawPixel(4,7);
    drawPixel(7,7);
    drawPixel(10,7);
    drawPixel(12,7);
    drawPixel(15,7);
    drawPixel(16,7);
    drawPixel(19,7);
    drawPixel(4,8);
    drawPixel(5,8);
    drawPixel(6,8);
    drawPixel(7,8);
    drawPixel(9,8);
    drawPixel(10,8);
    drawPixel(12,8);
    drawPixel(14,8);
    drawPixel(19,8);
    drawPixel(30,8);
    drawPixel(3,9);
    drawPixel(4,9);
    drawPixel(5,9);
    drawPixel(6,9);
    drawPixel(9,9);
    drawPixel(11,9);
    drawPixel(13,9);
    drawPixel(14,9);
    drawPixel(16,9);
    drawPixel(18,9);
    drawPixel(21,9);
    drawPixel(24,9);
    drawPixel(25,9);
    drawPixel(4,10);
    drawPixel(5,10);
    drawPixel(7,10);
    drawPixel(8,10);
    drawPixel(9,10);
    drawPixel(12,10);
    drawPixel(15,10);
    drawPixel(19,10);
    drawPixel(21,10);
    drawPixel(23,10);
    drawPixel(24,10);
    drawPixel(31,10);
    drawPixel(4,11);
    drawPixel(5,11);
    drawPixel(6,11);
    drawPixel(8,11);
    drawPixel(10,11);
    drawPixel(11,11);
    drawPixel(12,11);
    drawPixel(14,11);
    drawPixel(25,11);
    drawPixel(6,12);
    drawPixel(7,12);
    drawPixel(8,12);
    drawPixel(9,12);
    drawPixel(10,12);
    drawPixel(11,12);
    drawPixel(12,12);
    drawPixel(13,12);
    drawPixel(14,12);
    drawPixel(16,12);
    drawPixel(18,12);
    drawPixel(21,12);
    drawPixel(26,12);
    drawPixel(6,13);
    drawPixel(7,13);
    drawPixel(9,13);
    drawPixel(11,13);
    drawPixel(12,13);
    drawPixel(13,13);
    drawPixel(14,13);
    drawPixel(15,13);
    drawPixel(17,13);
    drawPixel(18,13);
    drawPixel(19,13);
    drawPixel(21,13);
    drawPixel(23,13);
    drawPixel(29,13);
    drawPixel(30,13);
    drawPixel(6,14);
    drawPixel(7,14);
    drawPixel(8,14);
    drawPixel(10,14);
    drawPixel(11,14);
    drawPixel(12,14);
    drawPixel(16,14);
    drawPixel(17,14);
    drawPixel(20,14);
    drawPixel(21,14);
    drawPixel(22,14);
    drawPixel(27,14);
    drawPixel(7,15);
    drawPixel(8,15);
    drawPixel(9,15);
    drawPixel(10,15);
    drawPixel(12,15);
    drawPixel(13,15);
    drawPixel(14,15);
    drawPixel(16,15);
    drawPixel(18,15);
    drawPixel(19,15);
    drawPixel(21,15);
    drawPixel(24,15);
    drawPixel(27,15);
    drawPixel(8,16);
    drawPixel(9,16);
    drawPixel(11,16);
    drawPixel(12,16);
    drawPixel(13,16);
    drawPixel(15,16);
    drawPixel(17,16);
    drawPixel(18,16);
    drawPixel(20,16);
    drawPixel(21,16);
    drawPixel(25,16);
    drawPixel(10,17);
    drawPixel(11,17);
    drawPixel(12,17);
    drawPixel(15,17);
    drawPixel(17,17);
    drawPixel(18,17);
    drawPixel(19,17);
    drawPixel(21,17);
    drawPixel(22,17);
    drawPixel(23,17);
    drawPixel(24,17);
    drawPixel(14,18);
    drawPixel(16,18);
    drawPixel(18,18);

    // while (a < 30) {
    //     drawPixel(a, 35);
    //     a++;
    // }
    printString("\n\n\nmangga");

    // TODO : Extra, Load bar
    while (k < 4096) {
        if (mod(k,128) == 0) {
            // drawRectangle(X_OFFSET, Y_OFFSET, k/128, 5);
            drawPixel(X_OFFSET + k / 128, Y_OFFSET + 1);
            drawPixel(X_OFFSET + k / 128, Y_OFFSET + 2);
            drawPixel(X_OFFSET + k / 128, Y_OFFSET + 3);
            drawPixel(X_OFFSET + k / 128, Y_OFFSET + 4);
            drawPixel(X_OFFSET + k / 128, Y_OFFSET + 5);

        }
        if (j > 1024) {
            j = 0;
            k++;
        }
        j++;
    }
    interrupt(0x10, 0x0003, 0, 0, 0);
}
