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

int halt = 0;

static int prompt(void) {
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

    mipscontext mips;
    memset(&mips, 0, sizeof (mipscontext));

    fetched fetch_out;
    decoded instr;
    executed execute_out;
    memmed mem_out;

    memset(&fetch_out, 0, sizeof (fetched));
    memset(&instr, 0, sizeof (decoded));
    memset(&execute_out, 0, sizeof (executed));
    memset(&mem_out, 0, sizeof (memmed));

    /* check for invalid input file */
    if (loadmem(&mips, argv[1]) < 0)
        exit(EXIT_FAILURE);

    mem_dump(&mips);

    int mode = MODE_INV;

    /*for (mips.pc = 0; mips.pc < mips.proglen; mips.pc += 4) {
        mips.ir = decode(mips.mem[mips.pc / 4]);

        if (mode != MODE_RUN) {
            mipscontext_display(&mips);
            mode = prompt();
        }

        if (mode == MODE_QUIT || step(&mips) == TERMINATE)
            break;
    }*/

    unsigned int clocks = 0;

    /* USING RETZ'S EXAMPLE STRUCTURE... CHANGE LATER */
    for (halt = 0, clocks = 0; !halt; clocks++) {
    int i;

    for (i = 0; i < MIPS_REGFILE_SIZE; i++) {
        if (i % 4 == 0)
            printf("\n");
        else
            printf("\t");

        printf("%d:\t0x%08x", i, mips.regfile[i]);
    }






        writeback(mips.regfile, &mem_out);
        mem_out = memory_access(&mips, &execute_out);
        execute_out = execute(&instr);
        instr = decode(&fetch_out, mips.regfile);
        fetch_out = fetch(&mips);
        getchar();
    }

    mipscontext_display(&mips);

    return 0;
}
