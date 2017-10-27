#ifndef DECODE_H
#define DECODE_H

#define R_INSTR 'R'
#define I_INSTR 'I'
#define J_INSTR 'J'

/* structure to represent a MIPS instruction */
typedef struct _instruction {
    char type;         /* type of instruction: R, I, J */

    unsigned char op;           /* (R, I, J) */

    unsigned char rs;           /* (R, I)    */
    unsigned char rt;           /* (R, I)    */

    unsigned char rd;           /* (R)       */
    unsigned char shamt;        /* (R)       */
    unsigned char funct;        /* (R)       */

    unsigned short imm;         /* (I)       */

    unsigned int wordind;       /* (J)       */
} instruction;

#endif
