#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mips_asm_header.h"
#include "mipssim.h"

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

    mem_dump(mem, (unsigned int)proglen);
    regfile_dump(regfile);

    for (pc = 0; pc < proglen; pc += 4) {   /* i contains byte offset addresses */
        ir = decode(mem[pc / 4]);           // fetch and decode

        instruction_print(ir);
    }


    return 0;
}
