#include <stdio.h>
#include <string.h>

#include "decode.h"

static int invalidop(unsigned char op) {
   return (op != 0x08 && op != 0x09 && op != 0x0c && \
           op != 0x0d && op != 0x0e && op != 0x0a && \
           op != 0x0b && op != 0x04 && op != 0x05 && \
           op != 0x02 && op != 0x03 && op != 0x20 && \
           op != 0x24 && op != 0x21 && op != 0x25 && \
           op != 0x0f && op != 0x23 && op != 0x28 && \
           op != 0x29 && op != 0x2b && op != 0x00);
}

static int invalidfunct(unsigned char funct) {
    return (funct != 0x20 && funct != 0x21 && funct != 0x22 && \
            funct != 0x23 && funct != 0x24 && funct != 0x27 && \
            funct != 0x25 && funct != 0x26 && funct != 0x00 && \
            funct != 0x02 && funct != 0x03 && funct != 0x04 && \
            funct != 0x06 && funct != 0x07 && funct != 0x2a && \
            funct != 0x2b && funct != 0x08 && funct != 0x09);
}

instruction decode(MIPS bits) {
    instruction instr;

    memset(&instr, 0, sizeof (instruction));

    instr.data = bits;
    instr.op   = (bits | 0) >> 26;

    if (invalidop(instr.op)) {
        instr.invalid = 1;
    } else if (instr.op == 0x0) {
        // R
        instr.type  = R_INSTR;
        instr.rs    = (bits >> 21) & 0x1f;
        instr.rt    = (bits >> 16) & 0x1f;
        instr.rd    = (bits >> 11) & 0x1f;
        instr.shamt = (bits >>  6) & 0x1f;
        instr.funct =  bits        & 0x3f;

        if (invalidfunct(instr.funct))
            instr.invalid = 1;
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
    if (instr.invalid) {
        printf("invalid instruction: 0x%08x\n", instr.data);

        return;
    }

    printf("type = %c, opcode = 0x%02x\n", instr.type, instr.op);

    if (instr.type == R_INSTR) {
        printf("  function = 0x%02x, rs = 0x%02x, rt = 0x%02x, rd = 0x%02x\n",\
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
            printf("\tR[$%02x] being shifted by R[$%02x]\n", instr.rt, instr.rs);
        }
    } else if (instr.type == I_INSTR) {
        printf("");
    } else {
        printf("  effective address = 0x%08x\n", instr.wordind << 2);
    }
}
