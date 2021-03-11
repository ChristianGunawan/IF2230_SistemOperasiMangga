// TODO : Recheck kernel size

#include "header/shell.h"

void shell() {
    interrupt(0x21, 0x1, "hello", 0, 0);
}
