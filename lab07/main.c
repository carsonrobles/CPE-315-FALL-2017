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

    MIPS fetched;
    decoded instr;

    memset(&instr, 0, sizeof (decoded));

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

    executed exe;

    memset(&exe, 0, sizeof (executed));

    unsigned int clocks = 0;

    /* USING RETZ'S EXAMPLE STRUCTURE... CHANGE LATER */
    for (halt = 0, clocks = 0; !halt; clocks++) {
        //exe = execute(&instr);
        decode(fetched, &instr, mips.regfile);
        fetched = fetch(&mips);
        getchar();
    }

    mipscontext_display(&mips);

    return 0;
}
