// 13519214 - Other function

#include "kernel.h"

void drawBootLogo() {
    int a = 0, b = 0, j = 0, k = 0, temp = 0;
    interrupt(0x10, 0x0013, 0, 0, 0);
    // Draw Logo
    // 320x200
    // while (a < 199) {
    // // putInMemory(0xA000, 0, 0x07);
    // // putInMemory(0xA000, 640, 0x07);
    //
    //     b = 0;
    //     while (b < 3) {
    //         if (b == 0 || b == 2)
    //         // putInMemory(V_GRAPHIC, a+b*320, 0x07);
    //         putInMemory(V_GRAPHIC, a*320+b, 0x07);
    //         // interrupt(0x10, 0x0C07, 0, a, b);
    //         b++;
    //     }
    //     a++;
    // }






    // #include "im2lazy.c"

    // TODO : Extra, Load
    // while (k < 4096) {
    // //     if (mod(k, 32) == 0) {
    // //         // temp = 0x0C00 | mod(k, 0x10);
    // //         temp = 0x0C0E;
    // //         interrupt(0x10, temp, 0, mod(k, 32), 0);
    // //         interrupt(0x10, temp, 0, mod(k, 32), 1);
    // //         interrupt(0x10, temp, 0, mod(k, 32), 2);
    // //         interrupt(0x10, temp, 0, mod(k, 32), 3);
    // //         interrupt(0x10, temp, 0, mod(k, 32), 4);
    // //     }
    //     if (j > 256) {
    //         j = 0;
    //         k++;
    //     }
    //     j++;
    // }
    // interrupt(0x10, 0x0003, 0, 0, 0);
    // TODO : Extra, ASCII / Graphical
}
