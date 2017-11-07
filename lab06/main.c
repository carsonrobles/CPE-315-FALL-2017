#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mips_asm_header.h"
#include "mipssim.h"

int main(void) {
    /* declare mips resources */
    MIPS mem[MIPS_MEM_SIZE];            // memory
    MIPS regfile[MIPS_REGFILE_SIZE];    // register file
    MIPS pc;                            // program counter

    /* initialize mips resources */
    memset(mem, 0, MIPS_MEM_SIZE);
    memset(regfile, 0, MIPS_REGFILE_SIZE);

    pc = 0;

    int proglen = loadmem(mem, 1024);

    /* ok, now dump out the instructions loaded: */

    for (pc = 0; pc < proglen; pc += 4) { /* i contains byte offset addresses */
        printf("Instruction@%08X : %08X\n", pc, mem[pc / 4]);
    }

    printf("\n");

    for (pc = 0; pc < proglen; pc += 4) { /* i contains byte offset addresses */
        instruction_print(decode(mem[pc / 4]));
    }

    return 0;
}
