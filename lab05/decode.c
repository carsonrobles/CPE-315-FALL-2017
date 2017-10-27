#include <stdio.h>

#include "decode.h"

typedef unsigned int MIPS, *MIPS_PTR;

instruction decode(MIPS bits) {
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
        instr.rs = (bits >> 21) & 0x1f;
        instr.rt = (bits >> 16) & 0x1f;
        instr.imm = bits & 0xffff;
    }

    return instr;
}

void print_cmd(instruction instr) {
    printf("type = %c,\t", instr.type);
    printf("opcode = 0x%02x\n", instr.op);
    if (instr.type == R_INSTR) {
        printf("\tfunction = 0x%02x, rs = 0x%02x, rt = 0x%02x, rd = 0x%02x\n",\
                instr.funct);
    }
}
