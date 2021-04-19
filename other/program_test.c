extern int interrupt(int number, int AX, int BX, int CX, int DX);

int main() {
    char buf[512];
    clear(buf, 64);
    interrupt(0x21, 0x0, "Test", 0, 1);
    gets(buf);
    print(buf, 0x2);
    while (1);
}
