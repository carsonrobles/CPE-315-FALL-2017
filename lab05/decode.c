#include <stdio.h>

typedef unsigned int MIPS, *MIPS_PTR;

void decode(MIPS instruction) {
    unsigned short opcode = (instruction | 0) >> 24;

    if (opcode == 0) {
        // R
    } else if (opcode == 2 || opcode == 3) {
        // J
    } else {
        // I
    }
}
