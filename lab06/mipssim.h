#ifndef MIPSSIM_H
#define MIPSSIM_H

#include "mips_asm_header.h"

#define MIPS_MEM_SIZE     1024
#define MIPS_REGFILE_SIZE 32

#define R_INSTR 'R'
#define I_INSTR 'I'
#define J_INSTR 'J'

/* structure to represent a MIPS instruction */
typedef struct _instruction {
    char type;                  /* type of instruction: R, I, J */

    unsigned int  data;
    unsigned char invalid;

    unsigned char op;           /* (R, I, J) */

    unsigned char rs;           /* (R, I)    */
    unsigned char rt;           /* (R, I)    */

    unsigned char rd;           /* (R)       */
    unsigned char shamt;        /* (R)       */
    unsigned char funct;        /* (R)       */

    unsigned int imm;           /* (I)       */

    unsigned int wordind;       /* (J)       */
} instruction;

int loadmem(MIPS *mem, char *fn);

instruction decode(MIPS bits);

void instruction_print(instruction instr);
void mem_dump(MIPS *mem, unsigned int proglen);
void regfile_dump(MIPS *regfile);

#endif
