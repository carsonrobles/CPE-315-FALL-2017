#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mipssim.h"

extern int halt;

int loadmem(mipscontext *mips, char *fn) {
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
        n = fread((void *) &mips->mem[memp/4], 4, 1, fd); /* note div/4 to make word index */

        if (n)
            memp += 4;  /* Increment byte pointer by size of instr */
        else
            break;
    } while (memp < (MIPS_MEM_SIZE * sizeof (MIPS)));

    fclose(fd);

    mips->proglen = (unsigned int)memp;

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

fetched fetch(mipscontext *mc) {
    fetched f;

    fprintf(stderr, "PC: 0x%08x\n", mc->pc);
    f.bits = mc->mem[mc->pc / 4];
    f.pc   = mc->pc;
    mc->pc += 4;

    return f;
}

decoded decode(fetched *fetch_out, MIPS *regfile) {
    decoded instr;
    memset(&instr, 0, sizeof (decoded));

    instr.pc = fetch_out->pc;

    instr.data = fetch_out->bits;
    instr.op   = fetch_out->bits >> 26;

    if (invalidop(instr.op)) {
        instr.invalid = 1;
    } else if (instr.op == 0x0) {
        // R
        instr.type  = R_INSTR;
        instr.rs    = (fetch_out->bits >> 21) & 0x1f;
        instr.rs_val = regfile[instr.rs];
        instr.rt    = (fetch_out->bits >> 16) & 0x1f;
        instr.rt_val = regfile[instr.rt];
        instr.rd    = (fetch_out->bits >> 11) & 0x1f;
        instr.shamt = (fetch_out->bits >>  6) & 0x1f;
        instr.funct =  fetch_out->bits        & 0x3f;

        if (invalidfunct(instr.funct))
            instr.invalid = 1;
        else if (instr.funct == 0xc)
            instr.rs_val = regfile[2];
    } else if (instr.op == 0x2 || instr.op == 0x3) {
        // J
        instr.type    = J_INSTR;
        instr.wordind = fetch_out->bits & 0x3ffffff;
    } else {
        // I
        instr.type   = I_INSTR;
        instr.rs     = (fetch_out->bits >> 21) & 0x1f;
        instr.rs_val = regfile[(fetch_out->bits >> 21) & 0x1f];
        instr.rt     = (fetch_out->bits >> 16) & 0x1f;
        instr.rt_val = regfile[(fetch_out->bits >> 16) & 0x1f];
        instr.imm    = fetch_out->bits & 0xffff;
    }

    instruction_print(instr);

    return instr;
}

memmed memory_access(mipscontext *mips, executed *ex) {
    memmed mem_out;
    memset(&mem_out, 0, sizeof(memmed));

    mem_out.wb_mode = ex->wb_mode;
    mem_out.dest = ex->reg_dest;

    if (ex->jmp) {
        mips->pc = ex->pc_src;
        return mem_out;
    }

    switch (ex->access) {
        case READ:
            mem_out.data = mips->mem[ex->alu_out];
            break;
        case WRITE:
            mips->mem[ex->alu_out] = ex->write_data;
            break;
        case DONOT:
            mem_out.data = ex->alu_out;
            break;
        default:
            fprintf(stderr, "Invalid memory access mode\n");
            exit(EXIT_FAILURE);
            break;
    }

    return mem_out;
}

void writeback(MIPS *regfile, memmed *m) {
    switch(m->wb_mode) {
        case WRITE:
    printf("\n\nWRITEBACK MODE %d DATA %d TO DEST %d\n\n", m->wb_mode, m->data, m->dest);

            regfile[m->dest] = m->data;
            break;
        case DONOT:
            break;
        default:
            fprintf(stderr, "Invalid writeback mode\n");
            exit(EXIT_FAILURE);
            break;
    }
}

void instruction_print(decoded instr) {
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
}

//void mem_dump(MIPS *mem, unsigned int proglen) {
void mem_dump(mipscontext *mips) {
    int i;

    printf("=== PROGRAM MEMORY ===\n");

    for (i = 0; i < mips->proglen; i += 4) { /* i contains byte offset addresses */
        printf("Instruction@%08X : %08X\n", i, mips->mem[i / 4]);
    }
}

/*
typedef struct _executed {
    unsigned int ALU_res;
    unsigned int pc;
    unsigned int write_data;
} executed;
*/
executed execute(decoded *decode_out) {
    executed execute_out;

    memset(&execute_out, 0, sizeof (executed));

    execute_out.access     = DONOT;
    execute_out.write_data = decode_out->rs_val;
    execute_out.wb_mode    = DONOT;
    execute_out.wb_mode    = WRITE;

    int signext_imm = (decode_out->imm & (1 << 15)) ? 0xffff0000 | decode_out->imm : decode_out->imm;

    switch (decode_out->type) {
        case 'R':
            execute_out.reg_dest = decode_out->rd;

            switch (decode_out->funct) {
                case 0x0:
                    execute_out.alu_out = decode_out->rt_val << decode_out->shamt;
                    break;
                case 0x2:
                    execute_out.alu_out = decode_out->rt_val >> decode_out->shamt;
                    break;
                case 0x3:
                    execute_out.alu_out = (int) decode_out->rt_val >> decode_out->shamt;
                    break;
                case 0x4:
                    execute_out.alu_out = decode_out->rt_val << decode_out->rs_val;
                    break;
                case 0x6:
                    execute_out.alu_out = decode_out->rt_val >> decode_out->rs_val;
                    break;
                case 0x7:
                    execute_out.alu_out = (int) decode_out->rt_val >> decode_out->rs_val;
                    break;
                case 0x8:
                    execute_out.pc_src  = decode_out->rs_val;
                    execute_out.jmp     = 1;
                    break;
                case 0x9:
                    execute_out.pc_src   = decode_out->rs_val;
                    execute_out.jmp      = 1;
                    execute_out.alu_out  = decode_out->pc + 4;
                    execute_out.reg_dest = 31;
                    break;
                case 0x20:
                    execute_out.alu_out = (int) decode_out->rs_val + (int) decode_out->rt_val;
                    break;
                case 0x21:
                    execute_out.alu_out = decode_out->rs_val + decode_out->rt_val;
                    break;
                case 0x22:
                    execute_out.alu_out = (int) decode_out->rs_val - (int) decode_out->rt_val;
                    break;
                case 0x23:
                    execute_out.alu_out = decode_out->rs_val - decode_out->rt_val;
                    break;
                case 0x24:
                    execute_out.alu_out = decode_out->rs_val & decode_out->rt_val;
                    break;
                case 0x25:
                    execute_out.alu_out = decode_out->rs_val | decode_out->rt_val;
                    break;
                case 0x26:
                    execute_out.alu_out = decode_out->rs_val ^ decode_out->rt_val;
                    break;
                case 0x27:
                    execute_out.alu_out = ~(decode_out->rs_val | decode_out->rt_val);
                    break;
                case 0x2a:
                    if ((int) decode_out->rs_val < (int) decode_out->rt_val)
                        execute_out.alu_out = 1;
                    else
                        execute_out.alu_out = 0;
                    break;
                case 0x2b:
                    if (decode_out->rs_val < decode_out->rt_val)
                        execute_out.alu_out = 1;
                    else
                        execute_out.alu_out = 0;

                    break;
                case 0xc:
                    execute_out.wb_mode = DONOT;
                    /* check this $v0 is RS? */
                    //if (regfile[2] == 10) {
                    if (decode_out->rs_val == 10) {
                        halt = 1;
                        //return TERMINATE;
                    }

                    break;
                default:
                    execute_out.wb_mode = DONOT;
                    fprintf(stderr, "Invalid function\n");
                    memset(&execute_out, 0, sizeof (executed));

                    return (execute_out);
                    break;
            }
            break;
        case 'I':
            execute_out.reg_dest = decode_out->rt;

            switch (decode_out->op) {
                case 0x08:      // addi
                    execute_out.alu_out = (signed)decode_out->rs_val + signext_imm;
                break;
                case 0x09:      // addiu
                    execute_out.alu_out = decode_out->rs_val + decode_out->imm;
                break;
                case 0x0c:      // andi
                    execute_out.alu_out = decode_out->rs_val & decode_out->imm;
                break;
                case 0x0d:      // ori
                    execute_out.alu_out = decode_out->rs_val | decode_out->imm;
                break;
                case 0x0e:      // xori
                    execute_out.alu_out = decode_out->rs_val ^ decode_out->imm;
                break;
                case 0x0a:      // slti
                    execute_out.alu_out = ((signed)decode_out->rs_val < signext_imm) ? 1 : 0;
                break;
                case 0x0b:      // sltiu
                    execute_out.alu_out = (decode_out->rs_val < decode_out->imm) ? 1 : 0;
                break;
                case 0x04:      // beq
                    //*pc = (regfile[instr.rs] == regfile[instr.rt]) ? *pc + 4 + (instr.imm << 2): *pc;
                    execute_out.wb_mode = DONOT;
                    execute_out.pc_src  = (decode_out->rs_val == decode_out->rt_val) ? decode_out->pc + (signext_imm << 2): decode_out->pc;
                    execute_out.jmp     = 1;
                break;
                case 0x05:      // bne
                    //*pc = (regfile[instr.rs] != regfile[instr.rt]) ? *pc + 4 + (instr.imm << 2): *pc;
                    execute_out.wb_mode = DONOT;
                    execute_out.pc_src  = (decode_out->rs_val != decode_out->rt_val) ? decode_out->pc + (signext_imm << 2): decode_out->pc;
                    execute_out.jmp     = 1;
                break;
                case 0x20:      // lb
                    execute_out.alu_out = (decode_out->rs_val + signext_imm);
                    execute_out.access  = READ;

                    //mips->readcount++;
                break;
                case 0x24:      // lbu
                    execute_out.alu_out = (decode_out->rs_val + signext_imm);
                    execute_out.access  = READ;

                    //mips->readcount++;
                break;
                case 0x21:      // lh
                    execute_out.alu_out = (decode_out->rs_val + signext_imm);
                    execute_out.access  = READ;

                    //mips->readcount++;
                break;
                case 0x25:      // lhu
                    execute_out.alu_out = (decode_out->rs_val + signext_imm);
                    execute_out.access  = READ;

                    //mips->readcount++;
                break;
                case 0x0f:      // lui
                    execute_out.alu_out = decode_out->imm << 16;
                break;
                case 0x23:      // lw
                    execute_out.alu_out = decode_out->rs_val + signext_imm;
                    execute_out.access  = READ;

                    //mips->readcount++;
                break;
                case 0x28:      // sb
                    execute_out.alu_out = decode_out->rs_val + signext_imm;
                    execute_out.access  = WRITE;
                    execute_out.wb_mode = DONOT;

                    //mips->writecount++;
                break;
                case 0x29:      // sh
                    execute_out.alu_out = decode_out->rs_val + signext_imm;
                    execute_out.access  = WRITE;
                    execute_out.wb_mode = DONOT;

                    //mips->writecount++;
                break;
                case 0x2b:      // sw
                    execute_out.alu_out = decode_out->rs_val + signext_imm;
                    execute_out.access  = WRITE;
                    execute_out.wb_mode = DONOT;

                    //mips->writecount++;
                break;
                default:
                    execute_out.wb_mode = DONOT;
                    fprintf(stderr, "Invalid function\n");
                    memset(&execute_out, 0, sizeof (executed));
                    return (execute_out);
                    break;
                break;
            }
            break;
        case 'J':
            switch (decode_out->op) {
                case 0x2:
                    execute_out.pc_src  = decode_out->wordind << 2;
                    execute_out.jmp     = 1;
                    execute_out.wb_mode = DONOT;
                    break;
                case 0x3:
                    execute_out.pc_src   = decode_out->wordind << 2;
                    execute_out.alu_out  = decode_out->pc + 4;
                    execute_out.jmp      = 1;
                    execute_out.reg_dest = 31;
                    printf("JAL\n\n\n\n************* %x\n\n\n\n", execute_out.pc_src);
                    break;
                default:
                    execute_out.wb_mode = DONOT;
                    fprintf(stderr, "Invalid function\n");
                    memset(&execute_out, 0, sizeof (executed));
                    return (execute_out);
                    break;
            }
            break;
        default:
            fprintf(stderr, "Invalid type\n");
            memset(&execute_out, 0, sizeof (executed));
            return (execute_out);
            break;
    }

    printf("%c: dest:%x wb:%x alu:%x jmp:%d pc:%x\n", decode_out->type, execute_out.reg_dest, execute_out.wb_mode, execute_out.alu_out, execute_out.jmp, execute_out.pc_src);

    return (execute_out);
}

void mipscontext_display(mipscontext *mips) {
    printf("\n=== SUMMARY ===\n");

    printf("\nPC:               \t%u\n", mips->pc);
    printf("\nInstruction Count:\t%u\n", mips->instrcount);
    printf("Memory Reads:     \t%u\n", mips->readcount);
    printf("Memory Writes:    \t%u\n", mips->writecount);
    printf("Clock Cycles:     \t%u\n", mips->clkcount);

    int i;

    for (i = 0; i < MIPS_REGFILE_SIZE; i++) {
        if (i % 4 == 0)
            printf("\n");
        else
            printf("\t");

        printf("%d:\t0x%08x", i, mips->regfile[i]);
    }

    printf("\n\n");

    if (mips->pc < mips->proglen)
        instruction_print(mips->ir);

    printf("\n");
}

