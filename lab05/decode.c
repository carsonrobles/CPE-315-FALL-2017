#include <stdio.h>

#include "decode.h"

typedef unsigned int MIPS, *MIPS_PTR;

void decode(MIPS bits) {
    instruction instr;

    instr.op = (bits | 0) >> 26;

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
        instr.type = I_INSTR;
        instr.rs = (bits & 0x3e00000) >> 21;
        instr.rt = (bits & 0x1f0000) >> 16;
        instr.imm = bits & 0xffff
    }
}
