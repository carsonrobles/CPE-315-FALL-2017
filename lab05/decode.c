#include <stdio.h>

#include "decode.h"

typedef unsigned int MIPS, *MIPS_PTR;

void decode(MIPS bits) {
    instruction instr;

    // clear instr
    memset(&instr, 0, sizeof(instruction));

    // get opcode from bits argument
    instr.op = (instruction | 0) >> 24;

    if (instr.op == 0) {
        // R
        instr.type  = R_INSTR;
        instr.rs    = (bits >> 21) & 0x1f;
        instr.rt    = (bits >> 16) & 0x1f;
        instr.rd    = (bits >> 11) & 0x1f;
        instr.shamt = (bits >>  6) & 0x1f;
        instr.funct =  bits        & 0x3f;
    } else if (instr.op == 2 || instr.op == 3) {
        // J
        instr.type    = J_INSTR;
        instr.wordind = bits & 0x3ffffff;
    } else {
        // I
    }
}
