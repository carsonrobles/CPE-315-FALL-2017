#ifndef MIPSSIM_H
#define MIPSSIM_H

#include "mips_asm_header.h"

#define MIPS_MEM_SIZE     1024
#define MIPS_REGFILE_SIZE 32

#define R_INSTR 'R'
#define I_INSTR 'I'
#define J_INSTR 'J'

#define TERMINATE 69

#define READ  1
#define WRITE 2
#define DONOT 0

/* structure to represent a MIPS instruction */
typedef struct _decoded {
    char type;                  /* type of instruction: R, I, J */

    unsigned int  data;
    unsigned char invalid;

    unsigned char op;           /* (R, I, J) */

    unsigned char rs;           /* (R, I)    */
    unsigned int rs_val;
    unsigned char rt;           /* (R, I)    */
    unsigned int rt_val;

    unsigned char rd;           /* (R)       */
    unsigned char shamt;        /* (R)       */
    unsigned char funct;        /* (R)       */

    unsigned int imm;           /* (I)       */

    unsigned int wordind;       /* (J)       */

    unsigned int pc;
} decoded;

typedef struct _executed {
    unsigned char jmp;          /* non zero if PC is being set to pc_src */
    unsigned int  pc_src;

    unsigned int  alu_out;
    unsigned int  write_data;
    unsigned char reg_dest;    // rt for imm
    unsigned char access;       // memory access mode

    unsigned char wb_mode;
} executed;

typedef struct _memmed {
    unsigned char wb_mode;
    unsigned char dest;
    unsigned int data;
} memmed;

typedef struct _fetched {
    MIPS bits;
    MIPS pc;
} fetched;

/* structure to represent MIPS runtime context */
typedef struct _mipscontext {
    unsigned int memoffset;
    unsigned int instrcount;
    unsigned int readcount;
    unsigned int writecount;
    unsigned int clkcount;
    unsigned int proglen;

    MIPS mem[MIPS_MEM_SIZE];            // memory
    MIPS regfile[MIPS_REGFILE_SIZE];    // register file
    MIPS pc;                            // program counter
    decoded ir;                     // instruction register
} mipscontext;

int loadmem(mipscontext *mips, char *fn);

fetched fetch(mipscontext *mc);
decoded decode(fetched *fetch_out, MIPS *regfile);
executed execute(decoded *decode_in);
memmed memory_access(mipscontext *mips, executed *ex);
void writeback(MIPS *regfile, memmed *m);

int step(mipscontext *mips);


void instruction_print(decoded instr);
void mem_dump(mipscontext *mips);
void regfile_dump(mipscontext *mips);
void mipscontext_display(mipscontext *mips);

#endif
