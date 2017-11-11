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

    /* check for invalid input file */
    if (loadmem(&mips, argv[1]) < 0)
        exit(EXIT_FAILURE);

    mem_dump(&mips);

    int mode = MODE_INV;

    for (mips.pc = 0; mips.pc < mips.proglen; mips.pc += 4) {
        printf("PC: %d\n", mips.pc);
        mips.ir = decode(mips.mem[mips.pc / 4]);

        if (mode != MODE_RUN) {
            mipscontext_display(&mips);
            mode = prompt();
        }

        if (mode == MODE_QUIT)
            break;

        step(&mips);
    }

    mipscontext_display(&mips);

    return 0;
}
