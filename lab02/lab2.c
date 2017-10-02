#include <stdio.h>
#include <math.h>

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

void extract_float(INTFLOAT_PTR x, float f) {
    unsigned int vect = (unsigned int)*((unsigned int *)&f);
    unsigned int sign = vect >> 31;
    unsigned int exp  = ((vect >> 23) & 0xff);
    unsigned int frac = (vect & 0x7fffff);

    // remove bias
    exp  -= 127;

    // set hidden 1 and correct exponent for this
    frac |= (1 << 23);
    exp  += 1;

    if (sign)
        frac = ~frac + 1;

    x->exponent = (int)exp;
    x->fraction = (int)frac;
}

void part2printwrap(const char *pref, unsigned int v) {
    float f = (float)*((float *)&v);

    INTFLOAT intfloat;

    extract_float(&intfloat, f);

    printf("%s Test case: 0x%x\n", pref, v);

    printf("  Float: %f\n", f);
    printf("  Exponent: %d\n", intfloat.exponent);
    printf("  Fraction: 0x%08x\n", intfloat.fraction);
}

/* part2: prints out formatted output for part 2 */
void part2(void) {
    printf("=========Part 2==========\n");

    part2printwrap("2a.", 0x40C80000);
    part2printwrap("2b.", 0xc3000000);
    part2printwrap("2c.", 0x3e000000);
    part2printwrap("2d.", 0x3eaaaaab);

    printf("=========================\n\n");
}

float packfloat(INTFLOAT_PTR ifp) {
    unsigned int vect = 0;

    unsigned int sign = 0;
    unsigned int exp  = (unsigned int)*((unsigned int *)&ifp->exponent);
    unsigned int frac = (unsigned int)*((unsigned int *)&ifp->fraction);

    if (ifp->fraction < 0) {
        sign = 1;
        frac = ~frac + 1;
    }

    // get sign bit in position
    sign <<= 31;

    // clear the hidden 1
    frac &= ~(1 << 23);

    // remove bias and correct for no hidden 1
    exp  += 127 - 1;

    // position exponent
    exp <<= 23;

    // piece the float together
    vect = sign | exp | frac;

    return (float)*((float *)&vect);
}

void part3printwrap(const char *pref, unsigned int v) {
    float f = (float)*((float *)&v);

    INTFLOAT intfloat;

    extract_float(&intfloat, f);

    printf("%s Test case: 0x%08x\n", pref, v);
    printf("  Float: %f\n", packfloat(&intfloat));
}

/* part3: prints out formatted output for part 3 */
void part3(void) {
    printf("=========Part 3==========\n");

    part3printwrap("3a.", 0x40c80000);
    part3printwrap("3b.", 0xC3000000);
    part3printwrap("3c.", 0x3E000000);
    part3printwrap("3d.", 0x3EAAAAAB);

    printf("=========================\n\n");
}

void normalize(INTFLOAT_PTR x) {
    unsigned int cond = 0, sign = 0;

    if (x->fraction == 0) {
        return;
    }

    sign = x->fraction >> 31;

    while ((cond = x->fraction & 0xc0000000) == 0xc0000000 ||\
            cond == 0x00000000) {
        x->fraction <<= 1;
        x->exponent--;
    }

    if (sign) {
        x->fraction |= 0x80000000;
    }
}

/* part4: prints out formatted output for part 4 */
void part4(void) {
    INTFLOAT intfloat;

    printf("=========Part 4==========\n");
    intfloat.exponent = 0x00000001;
    intfloat.fraction = 0x40000000;
    normalize(&intfloat);
    printf("4a. Exp = %3d, Frac = 0x%-7x\n", intfloat.exponent,\
            intfloat.fraction);
    intfloat.exponent = 0x00000000;
    intfloat.fraction = 0x00200000;
    normalize(&intfloat);
    printf("4b. Exp = %3d, Frac = 0x%-7x\n", intfloat.exponent,\
            intfloat.fraction);
    intfloat.exponent = 0x0000000c;
    intfloat.fraction = 0xffc00000;
    normalize(&intfloat);
    printf("4c. Exp = %3d, Frac = 0x%-7x\n", intfloat.exponent,\
            intfloat.fraction);
    intfloat.exponent = 0xfffffff8;
    intfloat.fraction = 0x02000000;
    normalize(&intfloat);
    printf("4d. Exp = %3d, Frac = 0x%-7x\n", intfloat.exponent,\
            intfloat.fraction);
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
