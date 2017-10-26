#include <stdio.h>

typedef unsigned int MIPS, *MIPS_PTR;

void decode(MIPS bits) {
    instruction instr;

    instr.op = (bits | 0) >> 26;
    if (opcode == 0) {
        // R
    } else if (opcode == 2 || opcode == 3) {
        // J
    } else {            /* I */
        instr.type = I_INSTR;
        instr.rs = (bits & 0x3e00000) >> 21;
        instr.rt = (bits & 0x1f0000) >> 16;
        instr.imm = bits & 0xffff
    }
}
