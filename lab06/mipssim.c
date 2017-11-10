#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mipssim.h"

int loadmem(MIPS *mem, char *fn) {
    MB_HDR mb_hdr;    /* Header area */
    FILE *fd;
    int n;
    int memp;
    int i;
    //char filename[] = "testcase1.mb"; /* This is the filename to be loaded */

    /* format the MIPS Binary header */

    fd = fopen(fn, "rb");
    if (fd == NULL) {
        printf("\nCouldn't load test case - quitting.\n");
        //exit(99);
        return (-1);
    }

    memp = 0;   /* This is the memory pointer, a byte offset */

    /* read the header and verify it. */
    fread((void *) &mb_hdr, sizeof(mb_hdr), 1, fd);

    if ((!strcmp(mb_hdr.signature, "~MB"))==0) {
        printf("\nThis isn't really a mips_asm binary file - quitting.\n");
        //exit(98);
        return (-2);
    }

    printf("\n%s Loaded ok, program size=%d bytes.\n\n", fn, mb_hdr.size);

    /* read the binary code a word at a time */

    do {
        n = fread((void *) &mem[memp/4], 4, 1, fd); /* note div/4 to make word index */

        if (n)
            memp += 4;  /* Increment byte pointer by size of instr */
        else
            break;
    } while (memp < (MIPS_MEM_SIZE * sizeof (MIPS)));

    fclose(fd);

    return (memp);
}

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
            funct != 0x2b && funct != 0x08 && funct != 0x09 && \
            funct != 0x0c);
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

void instruction_print(instruction instr) {
    if (instr.invalid) {
        printf("invalid instruction: 0x%08x\n", instr.data);

        return;
    }

    printf("type = %c, opcode = 0x%02x\n", instr.type, instr.op);

    if (instr.type == R_INSTR) {
        printf("\tfunction = $%02x, rs = $%02x, rt = $%02x, rd = $%02x\n",\
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
        // TODO: check this line: if (instr.op == 0x08 || instr.op == 0x09) {
        if (instr.op == 0x04 || instr.op == 0x05) {
            printf("R[$%02x] is being compared to R[$%02x]\n",\
                    instr.rs, instr.rt);
            printf("\teffective address = PC + 4 + %08x\n", instr.imm << 2);
        } else {
            printf("\trs = $%02x, rt = $%02x, immediate = %04x\n",\
                    instr.rs, instr.rt, instr.imm);
        }
    } else {
        printf("\teffective address = 0x%08x\n", instr.wordind << 2);
    }

    printf("\n");
}

int step(instruction instr, MIPS *pc, MIPS *regfile, MIPS *mem) {
    MIPS *entry = NULL;

    int signext_imm = (instr.imm & (1 << 15)) ? 0xffff0000 | instr.imm : instr.imm;

    switch (instr.type) {
        case 'R':
            switch (instr.funct) {
                case 0x0:
                    regfile[instr.rd] = regfile[instr.rt] << instr.shamt;
                    break;
                case 0x2:
                    regfile[instr.rd] = regfile[instr.rt] >> instr.shamt;
                    break;
                case 0x3:
                    regfile[instr.rd] = (int) regfile[instr.rt] >> instr.shamt;
                    break;
                case 0x4:
                    regfile[instr.rd] = regfile[instr.rt] << regfile[instr.rs];
                    break;
                case 0x6:
                    regfile[instr.rd] = regfile[instr.rt] >> regfile[instr.rs];
                    break;
                case 0x7:
                    regfile[instr.rd] = (int) regfile[instr.rt]\
                            >> regfile[instr.rs];
                    break;
                case 0x8:
                    *pc = regfile[instr.rs];
                    break;
                case 0x9:
                    *pc = regfile[instr.rs];
                    regfile[31] = *pc + 4;
                    break;
                case 0x20:
                    regfile[instr.rd] = (int) regfile[instr.rs]\
                            + (int) regfile[instr.rt];
                    break;
                case 0x21:
                    regfile[instr.rd] = regfile[instr.rs] + regfile[instr.rt];
                    break;
                case 0x22:
                    regfile[instr.rd] = (int) regfile[instr.rs]\
                            - (int) regfile[instr.rt];
                    break;
                case 0x23:
                    regfile[instr.rd] = regfile[instr.rs] - regfile[instr.rt];
                    break;
                case 0x24:
                    regfile[instr.rd] = regfile[instr.rs] & regfile[instr.rt];
                    break;
                case 0x25:
                    regfile[instr.rd] = regfile[instr.rs] | regfile[instr.rt];
                    break;
                case 0x26:
                    regfile[instr.rd] = regfile[instr.rs] ^ regfile[instr.rt];
                    break;
                case 0x27:
                    regfile[instr.rd] = ~(regfile[instr.rs]\
                            | regfile[instr.rt]);
                    break;
                case 0x2a:
                    if ((int) regfile[instr.rs] < (int) regfile[instr.rt])
                        regfile[instr.rd] = 1;
                    else
                        regfile[instr.rd] = 0;
                    break;
                case 0x2b:
                    if (regfile[instr.rs] < regfile[instr.rt])
                        regfile[instr.rd] = 1;
                    else
                        regfile[instr.rd] = 0;
                    break;
                case 0xc:
                    if (regfile[2] == 0x10) {
                        return TERMINATE;
                    }
                    break;
                default:
                    fprintf(stderr, "Invalid function\n");
                    return 2;
                    break;
            }
            break;
        case 'I':
            switch (instr.op) {
                case 0x08:      // addi
                    regfile[instr.rt] = (signed)regfile[instr.rs] + signext_imm;
                break;
                case 0x09:      // addiu
                    regfile[instr.rt] = regfile[instr.rs] + instr.imm;
                break;
                case 0x0c:      // andi
                    regfile[instr.rt] = regfile[instr.rs] & instr.imm;
                break;
                case 0x0d:      // ori
                    regfile[instr.rt] = regfile[instr.rs] | instr.imm;
                break;
                case 0x0e:      // xori
                    regfile[instr.rt] = regfile[instr.rs] ^ instr.imm;
                break;
                case 0x0a:      // slti
                    regfile[instr.rt] = ((signed)regfile[instr.rs] < signext_imm) ? 1 : 0;
                break;
                case 0x0b:      // sltiu
                    regfile[instr.rt] = (regfile[instr.rs] < instr.imm) ? 1 : 0;
                break;
                case 0x04:      // beq
                    *pc = (regfile[instr.rs] == regfile[instr.rt]) ? *pc + 4 + (instr.imm << 2): *pc;
                break;
                case 0x05:      // bne
                    *pc = (regfile[instr.rs] != regfile[instr.rt]) ? *pc + 4 + (instr.imm << 2): *pc;
                break;
                case 0x20:      // lb
                    regfile[instr.rt] = mem[instr.rs + signext_imm] & 0xff;

                    // sign extend
                    if (regfile[instr.rt] & 0x80) regfile[instr.rt] |= 0xffffff00;
                break;
                case 0x24:      // lbu
                    regfile[instr.rt] = mem[instr.rs + signext_imm] & 0xff;
                break;
                case 0x21:      // lh
                    regfile[instr.rt] = mem[instr.rs + signext_imm] & 0xffff;

                    // sign extend
                    if (regfile[instr.rt] & 0x8000) regfile[instr.rt] |= 0xffff0000;
                break;
                case 0x25:      // lhu
                    regfile[instr.rt] = mem[instr.rs + signext_imm] & 0xffff;
                break;
                case 0x0f:      // lui
                    regfile[instr.rt] = instr.imm << 16;
                break;
                case 0x23:      // lw
                    regfile[instr.rt] = mem[instr.rs + signext_imm];
                break;
                case 0x28:      // sb
                    entry = &mem[regfile[instr.rs] + signext_imm];

                    *entry &= 0xffffff00;
                    *entry |= (regfile[instr.rt] & 0xff);
                break;
                case 0x29:      // sh
                    entry = &mem[regfile[instr.rs] + signext_imm];

                    *entry &= 0xffff0000;
                    *entry |= (regfile[instr.rt] & 0xffff);
                break;
                case 0x2b:      // sw
                    mem[regfile[instr.rs] + signext_imm] = regfile[instr.rt];
                break;
                default:
                    fprintf(stderr, "Invalid function\n");
                    return 2;
                    break;
                break;
            }
            break;
        case 'J':
            switch (instr.op) {
                case 0x2:
                    *pc = instr.wordind << 2;
                    break;
                case 0x3:
                    regfile[31] = *pc + 4;
                    *pc = instr.wordind << 2;
                    break;
                default:
                    fprintf(stderr, "Invalid function\n");
                    return 2;
                    break;
            }
            break;
        default:
            fprintf(stderr, "Invalid type\n");
            return 1;
            break;
    }

    /* set $zero back to 0 in case it was altered */
    regfile[0] = 0;

    return 0;
}

void mem_dump(MIPS *mem, unsigned int proglen) {
    int i;

    printf("------------------------------\n");
    printf("PROGRAM MEMORY\n\n");

    for (i = 0; i < proglen; i += 4) { /* i contains byte offset addresses */
        printf("Instruction@%08X : %08X\n", i, mem[i / 4]);
    }

    printf("------------------------------\n");
}

void regfile_dump(MIPS *regfile) {
    int i;

    printf("------------------------------\n");
    printf("REGISTER FILE\n\n");

    for (i = 0; i < MIPS_REGFILE_SIZE; i++) {
        printf("%d:\t0x%08x\n", i, *(regfile + i));
    }

    printf("------------------------------\n");
}
