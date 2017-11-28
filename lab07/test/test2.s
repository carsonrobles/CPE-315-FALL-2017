
.text

main:
   ori   $2, $zero, 0x3
   nop
   nop
   nop
   nop
   nop
   nop
   addi  $1, $2, 0x9
   nop
   nop
   nop
   nop
   nop
   nop
   addi $v0, $zero, 0xa
   nop
   nop
   nop
   nop
   nop
   nop
   syscall

.end main
