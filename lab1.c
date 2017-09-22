#include <stdio.h>

void part1() {
   printf("Part 1: Data Types and Their Sizes\n");
   printf("==================================\n");
   printf("Byte Size: %d\n", sizeof(char));
   printf("Short Int Size: %d\n", sizeof(short));
   printf("Integer Size: %d\n", sizeof(int));
   printf("Long Integer Size: %d\n", sizeof(long));
   printf("==================================\n\n\n");
}

void part2() {
   unsigned char bytestring[] = {
      0x41, 0x33, 0x54, 0x80, 0xff, 0x99, 0x01, 0x78,
      0x55, 0x20, 0xfe, 0xee, 0x00, 0x00, 0x00, 0x00
   };
   int i = 0, *int_ptr = (int *) bytestring;
   short *short_ptr = (short *) bytestring;
   long *long_ptr = (long *) bytestring;

   printf("Part 2: Byte Ordering\n");
   printf("=====================\n");
   printf("2a. ");
   for (i = 0; i < 16; i++) {
      printf("0x%x ", bytestring[i]);
   }
   printf("\n\n");

   printf("2b. ");
   for (i = 0; i < 4; i++) {
      printf("0x%hx ", *short_ptr);
      short_ptr++;
   }
   printf("\n\n");

   printf("2c. ");
   for (i = 0; i < 2; i++) {
      printf("0x%x ", *int_ptr);
      int_ptr++;
   } printf("\n\n");

   printf("2d. 0x%lx\n\n", *long_ptr);

   printf("2e. ");
   for (i = 0; i < 16; i++) {
      printf("0x%x ", bytestring[i]);
   }
   printf("\n\n");

   printf("2f. 0x4133 0x5480 0xff99 0x0178\n\n");

   printf("2g. 0x41335480 0xff990178\n\n");

   printf("2h. 0x41335480ff990178\n");
   printf("=====================\n\n\n");
}

void part3() {
   printf("Part 3: Unisgned Addition of Small Fields\n");
   printf("=========================================\n");
   printf("3a. 0x20 + 0x35 = 0x%x\n\n", 0x20 + 0x35);
   printf("3b. 0x80 + 0x7f = 0x%x\n\n", 0x80 + 0x7f);
   printf("3c. 0x80 + 0xff = 0x%x\n\n", (unsigned char) (0x80 + 0xff));
   printf("3d. 0xff + 0x01 = 0x%x\n", (unsigned char) (0xff + 0x01));
   printf("=========================================\n\n\n");
}

char detect_carry(unsigned char op1, unsigned char op2,\
   unsigned char carry_in) {
   unsigned char sum = op1 + op2 + carry_in;

   if (sum < op1) {
      return 1;
   }
   return 0;
}

void part4() {
   printf("Part 4: Detecting a Carry Condition\n");
   printf("===================================\n");
   printf("4a. 0x20 + 0x35 Carry: %d\n\n", detect_carry(0x20, 0x35, 0));
   printf("4b. 0x80 + 0x7f Carry: %d\n\n", detect_carry(0x80, 0x7f, 0));
   printf("4c. 0x80 + 0xff Carry: %d\n\n", detect_carry(0x80, 0xff, 0));
   printf("4d. 0xff + 0x01 Carry: %d\n", detect_carry(0xff, 0x01, 0));
   printf("===================================\n\n\n");
}

char detect_overflow(signed char op1, signed char op2) {
   signed char sum = op1 + op2;

   if ((op1 < 0 && op2 < 0 && sum > 0) || (op1 > 0 && op2 > 0 && sum < 0)) {
      return 1;
   }
   return 0;
}

void part5() {
   printf("Part 5: Signed Addition and Overflow\n");
   printf("====================================\n");
   printf("5a. 0x20 + 0x35 Overflow: %d\n\n", detect_overflow(0x20, 0x35));
   printf("5b. 0x80 + 0x7f Overflow: %d\n\n", detect_overflow(0x80, 0x7f));
   printf("5c. 0x80 + 0xff Overflow: %d\n\n", detect_overflow(0x80, 0xff));
   printf("5d. 0xff + 0x01 Overflow: %d\n", detect_overflow(0xff, 0x01));
   printf("====================================\n\n\n");
}

unsigned int arbitrary_byte_add(unsigned char *result, unsigned char *a1,\
   unsigned char *a2, int size, unsigned int carry_in) {
   int i = 0;
   unsigned char carry = carry_in;

   for (i = size - 1; i >= 0; i--) {
      result[i] = carry + a1[i] + a2[i];
      carry = detect_carry(a1[i], a2[i], carry);
   }

   return carry;
}

void part6() {
   unsigned char a1a[] = { 0x44, 0x00, 0x00, 0x00, 0x01 };
   unsigned char a2a[] = { 0x30, 0xff, 0xff, 0x00, 0x00 };
   unsigned char result_a[5];
   unsigned char a1b[] = { 0xff, 0xff, 0xff, 0xff, 0xff };
   unsigned char a2b[] = { 0x00, 0x00, 0x00, 0x00, 0x01 };
   unsigned char result_b[5];
   int i = 0, carry = 0;

   printf("Part 6: Performing Extended Field Arithmetic\n");
   printf("============================================\n");
   printf("6a.\n");
   printf("\t");
   for (i = 0; i < 5; i++) {
      printf("0x%-2x ", a1a[i]);
   }
   printf("\n");
   printf("+\t");
   for (i = 0; i < 5; i++) {
      printf("0x%-2x ", a2a[i]);
   }
   printf("\n");
   printf("=\t");
   carry = arbitrary_byte_add(result_a, a1a, a2a, 5, 0);
   for (i = 0; i < 5; i++) {
      printf("0x%-2x ", result_a[i]);
   }
   printf("\n");
   printf("Carry Out = %d\n\n", carry);
   carry = 0;
   printf("6b.\n");
   printf("\t");
   for (i = 0; i < 5; i++) {
      printf("0x%-2x ", a1b[i]);
   }
   printf("\n");
   printf("+\t");
   for (i = 0; i < 5; i++) {
      printf("0x%-2x ", a2b[i]);
   }
   printf("\n");
   printf("=\t");
   carry = arbitrary_byte_add(result_b, a1b, a2b, 5, 0);
   for (i = 0; i < 5; i++) {
      printf("0x%-2x ", result_b[i]);
   }
   printf("\n");
   printf("Carry Out = %d\n", carry);
   printf("============================================\n");
}

void main() {
   int i = 0, *int_ptr;
   short int *si;
   long *long_ptr;
   unsigned char bytestring[] = {
      0x41, 0x33, 0x54, 0x80, 0xff, 0x99, 0x01, 0x78,
      0x55, 0x20, 0xfe, 0xee, 0x00, 0x00, 0x00, 0x00
   };

   part1();

   part2();

   part3();

   part4();

   part5();

   part6();
}
