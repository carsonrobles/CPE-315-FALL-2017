#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "mips_asm_header.h"
#include "mipssim.h"

#define MIPS_MEM_SIZE 1024

int main(void) {
    MIPS mem[MIPS_MEM_SIZE];

  int i;
  int loadlen = loadmem(mem, 1024);

/* ok, now dump out the instructions loaded: */

  for (i = 0; i<loadlen; i+=4) /* i contains byte offset addresses */
     {
     printf("Instruction@%08X : %08X\n", i, mem[i/4]);
     }
  printf("\n");

  for (i = 0; i<loadlen; i+=4) /* i contains byte offset addresses */
     {
     print_cmd(decode(mem[i/4]));
     }

  exit(0);
    return 0;
}
