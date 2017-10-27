#include <stdio.h>

#include "decode.h"

instruction decode(MIPS bits) {
    instruction instr;

    instr.op = (bits | 0) >> 26;

    if (instr.op == 0x0) {
        // R
        instr.type  = R_INSTR;
        instr.rs    = (bits >> 21) & 0x1f;
        instr.rt    = (bits >> 16) & 0x1f;
        instr.rd    = (bits >> 11) & 0x1f;
        instr.shamt = (bits >>  6) & 0x1f;
        instr.funct =  bits        & 0x3f;
    } else if (instr.op == 0x2 || instr.op == 0x3) {
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
    printf("type = %c, opcode = 0x%02x\n", instr.type, instr.op);

    if (instr.type == R_INSTR) {
        printf("function = 0x%02x, rs = 0x%02x, rt = 0x%02x, rd = 0x%02x\n",\
                instr.funct, instr.rs, instr.rt, instr.rd);
        if (instr.funct == 0x0 || instr.funct == 0x2 || instr.funct == 0x3) {
            printf("\tname = ");
            if (instr.funct == 0x0) {
                printf("sll");
            } else if (instr.funct == 0x2) {
                printf("srl");
            } else {
                printf("sra");
            }
            printf(", shift = 0x%02x\n", instr.shamt);
        } else if (instr.funct == 0x4 || instr.funct == 0x6
                || instr.funct == 0x7) {
            printf("\tR[$%02x] being shifted by R[$%02x]\n",\
                    instr.rt, instr.rs);
        }
    } else if (instr.type == I_INSTR) {
        if (instr.op == 0x08 || instr.op == 0x09) {
            printf("R[$%02x] is being compared to R[$%02x]\n",\
                    instr.rs, instr.rt);
            printf("effective address = PC + 4 + %08x\n", instr.imm << 2);
        } else {
            printf("rs = $%02x, rt = $%02x\n, immediate = %04x",\
                    instr.rs, instr.rt, instr.imm);
        }
    } else {
        printf("\teffective address = 0x%08x\n", instr.wordind << 2);
    }

    printf("\n");
}
