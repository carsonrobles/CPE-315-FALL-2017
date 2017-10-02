#include <stdio.h>
#include <stdint.h>

typedef struct _intfloat {
    int exponent;
    int fraction;
} INTFLOAT, *INTFLOAT_PTR;

typedef struct _extracted {
    int sign;
    int exponent;
    int fraction;
} EXTRACTED, *EXTRACTED_PTR;

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

void extract_float(INTFLOAT_PTR x, float f) {
    unsigned int vect = (unsigned int)*((unsigned int *)&f);
    unsigned int sign = vect >> 31;
    unsigned int exp  = (vect >> 23) & 0xff;
    unsigned int frac = vect & 0x7fffff;

    printf("%f %x: %u %u %u\n", f, vect, sign, exp, frac);
}

void normalize(INTFLOAT_PTR x) {
    unsigned int floatint = 0;
    unsigned int sign = x->fraction >> 31;
    unsigned int exp = x->exponent
    unsigned int fraction = 
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
    printf("2a. Test case: 0x40C80000\n");
    printf("2b. Test case: 0xc3000000\n");
    printf("2c. Test case: 0x3e000000\n");
    printf("2d. Test case: 0x3EAAAAAB\n");
    printf("=========================\n\n");
}

/* part3: prints out formatted output for part 3 */
void part3(void) {
    printf("=========Part 3==========\n");
    printf("3a. Test case: 0x40c80000\n");
    printf("  Float:\n");
    printf("3b. Test case: 0xC3000000\n");
    printf("  Float:\n");
    printf("3c. Test case: 0x3E000000\n");
    printf("  Float:\n");
    printf("3d. Test case: 0x3EAAAAAB\n");
    printf("  Float:\n");
    printf("=========================\n\n");
}

/* part4: prints out formatted output for part 4 */
void part4(void) {
    printf("=========Part 4==========\n");
    printf("4a. Exp = %d, Frac = 0x%x\n");
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

    extract_float(NULL, 123.277);

    return 0;
}
