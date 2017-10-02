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
    x->fraction = (int)(frac << 7);
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

    if (!(exp | frac)) return 0;

    if (ifp->fraction < 0) {
        sign = 1;
        frac = ~frac + 1;
    }

    frac >>= 7;

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

/* normalize: normalizes the provided float int structure */
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

void arithrightshift(int *vect, int n) {
    char sign = *vect | (1 << 31);

    *vect &= 0xffbfffff;

    while (n-- > 0) {
        *vect >>= 1;

        if (sign) *vect |= (1 << 31);
    }
}

/* fadd: adds two single precision floating point values */
float fadd(float a, float b) {
    INTFLOAT a_str;
    INTFLOAT b_str;

    INTFLOAT_PTR shift = NULL;

    int dif;
    int i;

    extract_float(&a_str, a);
    extract_float(&b_str, b);

    /* find number with most negative exponent */
    if ((dif = a_str.exponent - b_str.exponent) < 0) {
        dif   = -1 * dif;
        shift = &a_str;
    } else {
        shift = &b_str;
    }

    /* align smaller number with the larger */
    for (i = 0; i < dif; i++) {
        arithrightshift(&shift->fraction, 1);

        shift->exponent += 1;
    }

    arithrightshift(&a_str.fraction, 1);
    arithrightshift(&b_str.fraction, 1);

    a_str.exponent += 1;
    b_str.exponent += 1;

    shift->fraction = a_str.fraction + b_str.fraction;

    if (shift->fraction == 0)
        shift->exponent = 0;

    normalize(shift);

    return (packfloat(shift));
}

void part5printwrap(const char *pref, unsigned int v1, unsigned int v2) {
    char cs1 = (v1 & (1 << 31)) ? 0 : '+';
    char cs2 = (v2 & (1 << 31)) ? 0 : '+';

    float f1  = (float)*((float *)&v1);
    float f2  = (float)*((float *)&v2);
    float sum = fadd(f1, f2);

    printf("%s 0x%08X and 0x%08X (%c%f and %c%f)\n", pref, v1, v2, cs1, f1, cs2, f2);
    printf("  Sum:0x%08X (%f Decimal Value)\n", (unsigned int)*((unsigned int *)&sum), sum);
}

/* part5: prints out formatted output for part 5 */
void part5(void) {
    float a = 0, b = 0;
    printf("=========Part 5==========\n");

    part5printwrap("5a.", 0xBF800000, 0x3F800000);
    part5printwrap("5b.", 0x3F800000, 0x3E800000);
    part5printwrap("5c.", 0x40800000, 0xBE000000);

    printf("=========================\n\n");
}

float fsub(float a, float b) {
    INTFLOAT a_str;
    INTFLOAT b_str;

    INTFLOAT_PTR shift = NULL;

    int dif;
    int i;

    extract_float(&a_str, a);
    extract_float(&b_str, b);

    /* find number with most negative exponent */
    if ((dif = a_str.exponent - b_str.exponent) < 0) {
        dif   = -1 * dif;
        shift = &a_str;
    } else {
        shift = &b_str;
    }

    /* align smaller number with the larger */
    for (i = 0; i < dif; i++) {
        arithrightshift(&shift->fraction, 1);

        shift->exponent += 1;
    }

    arithrightshift(&a_str.fraction, 1);
    arithrightshift(&b_str.fraction, 1);

    a_str.exponent += 1;
    b_str.exponent += 1;

    shift->fraction = a_str.fraction - b_str.fraction;

    if (shift->fraction == 0) {
        shift->exponent = 0;
    }

    normalize(shift);

    return (packfloat(shift));
}

void part6printwrap(const char *pref, unsigned int v1, unsigned int v2) {
    char cs1 = (v1 & (1 << 31)) ? 0 : '+';
    char cs2 = (v2 & (1 << 31)) ? 0 : '+';

    float f1  = (float)*((float *)&v1);
    float f2  = (float)*((float *)&v2);
    float sum = fsub(f1, f2);

    printf("%s 0x%08X and 0x%08X (%c%f - ", pref, v1, v2, cs1, f1);
    if (f2 >= 0) {
        printf("%c%f)", cs2, f2);
    } else {
        printf("(%c%f))", cs2, f2);
    }
    printf("  Sum:0x%08X (%f Decimal Value)\n", (unsigned int)*((unsigned int *)&sum), sum);
}

/* part6: prints out formatted output for part 6 */
void part6(void) {
    printf("=========Part 6==========\n");
    float a = 0, b = 0;
    part6printwrap("6a.", 0x40400000, 0x3f800000);
    part6printwrap("6b.", 0x40400000, 0xbf800000);
    part6printwrap("6c.", 0x40000000, 0x40000000);
    printf("=========================\n\n");
}

float fmul(float a, float b) {
    INTFLOAT a_str, b_str;
    INTFLOAT result;

    extract_float(&a_str, a);
    extract_float(&b_str, b);

    int a_32 = a_str.fraction >> 7;
    int b_32 = b_str.fraction >> 7;


    char sign = (a_32 >> 31) ^ (b_32 >> 31);

    if (a_32 >> 31)
        a_32 = ~a_32 + 1;

    if (b_32 >> 31)
        b_32 = ~b_32 + 1;

    long int res = (long)a_32 * (long)b_32;
    while (res && ((res >> 62) == 0 || (res >> 62) == 3))
        res <<= 1;

    result.fraction = (int)(res >> 32);
    result.exponent = a_str.exponent + b_str.exponent - 1;

    normalize(&result);

    if (sign) result.fraction = ~result.fraction + 1;

    return (packfloat(&result));
}

void part7printwrap(const char *pref, unsigned int v1, unsigned int v2) {
    float f1 = (float)*((float *)&v1);
    float f2 = (float)*((float *)&v2);

    float        prod_flt = fmul(f1, f2);
    unsigned int prod_int = (unsigned int)*((unsigned int *)&prod_flt);

    printf("%s 0x%08X and 0x%08X (%f x %f)\n", pref, v1, v2, f1, f2);
    printf(" Product:0x%08X (%f Decimal Value)\n", prod_int, prod_flt);
}

/* part7: prints out formatted output for part 7 */
void part7(void) {
    printf("=========Part 7==========\n");

    part7printwrap("7a.", 0x40200000, 0x40200000);
    part7printwrap("7b.", 0xc1700000, 0x45800000);

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
