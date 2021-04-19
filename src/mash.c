#include "std-header/std_fileio.h"
#include "std-header/std_stringio.h"
#include "basic-header/std_opr.h"

int main() {
    char buf[512];
    clear(buf, 64);
    interrupt(0x21, 0x0, "hore", 0, 1);
    gets(buf);
    print(buf, 0x2);
    while (1);
}
