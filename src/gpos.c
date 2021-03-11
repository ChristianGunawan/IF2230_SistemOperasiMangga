extern int interrupt(int number, int AX, int BX, int CX, int DX);

int main() {
    getCursorPos();
    return 0;
}

void handleInterrupt21(int AX, int BX, int CX, int DX) {
    
}

int getCursorPos() {
    int DX;
    interrupt(0x10, 0x0300, 0x0000, 0, 0);
    DX = 3;
    return DX;
}
