#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mips_asm_header.h"
#include "mipssim.h"

#define MAX_INPUT_SIZE 5

#define MODE_STEP  1
#define MODE_RUN   2
#define MODE_QUIT  3
#define MODE_INV  -1

int prompt(void) {
    char inp[MAX_INPUT_SIZE];

    while (1) {
        printf(": ");
        scanf("%s", inp);

        if (!strncmp(inp, "step", MAX_INPUT_SIZE))
            return (MODE_STEP);
        else if (!strncmp(inp, "run", MAX_INPUT_SIZE))
            return (MODE_RUN);
        else if (!strncmp(inp, "quit", MAX_INPUT_SIZE))
            return (MODE_QUIT);
    }

    return (MODE_INV);
}

int main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s [program file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* declare mips resources */
    MIPS mem[MIPS_MEM_SIZE];            // memory
    MIPS regfile[MIPS_REGFILE_SIZE];    // register file
    MIPS pc;                            // program counter
    instruction ir;                     // instruction register

    /* initialize mips resources */
    memset(mem, 0, MIPS_MEM_SIZE);
    memset(regfile, 0, MIPS_REGFILE_SIZE);

    pc = 0;

    int proglen = loadmem(mem, argv[1]);

    /* check for invalid input file */
    if (proglen < 0)
        exit(EXIT_FAILURE);

    mem_dump(mem, (unsigned int)proglen);

    int mode = MODE_INV;

    for (pc = 0; pc < proglen; pc += 4) {   /* i contains byte offset addresses */
        if (mode != MODE_RUN)
            mode = prompt();

        if (mode == MODE_QUIT)
            break;

        ir = decode(mem[pc / 4]);           // fetch and decode

        instruction_print(ir);
        step(ir, &pc, regfile, mem);
    }

    regfile_dump(regfile);

    return 0;
}
