#include <stdio.h>
#include <stdint.h>

/* umultiply: multiplies two 16-bit unsigned nubmers and returns a 32-bit unsigned number */
unsigned int umultiply(unsigned int a, unsigned int b) {
    unsigned int prod  = 0;
    unsigned int toadd = a;
    unsigned int mask  = 0x1;

    int i;

    for (i = 0; i < 16; i++) {
        if (b & mask)
            prod += toadd;

        toadd <<= 1;
        mask  <<= 1;
    }

    return prod;
}

/* part1: prints out formatted output for part 1 */
void part1(void) {
    printf("=========Part 1==========\n");
    printf("1a. a=0x0001, b=0x0001 c=0x%08x\n", umultiply(0x0001, 0x0001));
    printf("1b. a=0x0001, b=0xFFFF c=0x%08x\n", umultiply(0x0001, 0xffff));
    printf("1c. a=0x8000, b=0x0001 c=0x%08x\n", umultiply(0x8000, 0x0001));
    printf("1d. a=0x4000, b=0x4000 c=0x%08x\n", umultiply(0x4000, 0x4000));
    printf("1e. a=0x8000, b=0x8000 c=0x%08x\n", umultiply(0x8000, 0x8000));
    printf("=========================\n\n");
}

/* part2: prints out formatted output for part 2 */
void part2(void) {
    printf("=========Part 2==========\n");

    printf("=========================\n\n");
}

/* part3: prints out formatted output for part 3 */
void part3(void) {
    printf("=========Part 3==========\n");

    printf("=========================\n\n");
}

/* part4: prints out formatted output for part 4 */
void part4(void) {
    printf("=========Part 4==========\n");

    printf("=========================\n\n");
}

/* part5: prints out formatted output for part 5 */
void part5(void) {
    printf("=========Part 5==========\n");

    printf("=========================\n\n");
}

/* part6: prints out formatted output for part 6 */
void part6(void) {
    printf("=========Part 6==========\n");

    printf("=========================\n\n");
}

/* part7: prints out formatted output for part 7 */
void part7(void) {
    printf("=========Part 7==========\n"); 

    printf("=========================\n\n");
}

int main(void) {
    part1();
    part2();
    part3();
    part4();
    part5();
    part6();
    part7();

    return 0;
}
