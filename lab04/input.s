# ------------------------------ #
# lab 4
# ------------------------------ #

# ------------------------------ #
# example push
#   addi    $sp, $sp, -4
#   sw      $ra, 0($sp)
#
# example pull
#   lw      $ra, 0($sp)
#   addi    $sp, $sp, 4
# ------------------------------ #

	.data
hexstr:	.space	10
addstr: .space 	8

	.text
main:	and	$a1, $a1, $zero		# clear $a1
	la	$a1, hexstr
	li	$a0, 0x12345678
	
	li 	$s0, 0x13
	li	$s1, 0xfff000ff
	li      $s2, 0x10101010
	
	jal	bintohex
	
	li	$v0, 4
	la	$a0, hexstr
	
	#syscall
	
	li	$a0, 0x10000222
	li	$a1, 0xffffffff
	li	$a2, 0x10020013
	li	$a3, 0x11111111
	
	jal 	double_add
	
	add 	$s0, $v0, $zero
	add	$s1, $v1, $zero
	
	la 	$a1, hexstr
	add	$a0, $s0, $zero
	jal	bintohex
	
	li	$v0, 4
	la	$a0, hexstr
	
	syscall
	
	la 	$a1, hexstr
	add	$a0, $s1, $zero
	jal	bintohex
	
	li	$v0, 4
	la	$a0, hexstr
	
	syscall
	
donot:	nop
	b 	donot

# ------------------------------ #
# bintohex
#
# $a0: 32-bit binary value to convert to hexadecimal
# $a1: address to begin writing null terminated hex string
# ------------------------------ #
bintohex:
	# push used saved registers on stack
	addi 	$sp, $sp, -4
	sw	$s0, 0($sp)
	
	addi 	$sp, $sp, -4
	sw	$s1, 0($sp)
	
	addi 	$sp, $sp, -4
	sw	$s2, 0($sp)
	
	addi 	$sp, $sp, -4
	sw	$s3, 0($sp)

	# init counter to decimal 10 and changeable address
	addi	$s0, $zero, 0xa		# counter = 10
	addi	$s1, $a1,   0x8		# address = last address
	addi 	$s2, $zero, 0x0		# current nybble
	addi	$s3, $a0,   0x0		# vector to convert
	
	# write null character
	sb 	$zero, 0($s1)
	
	# iterate over $a0, 4 bits at a time
bhloop:	addi	$s0, $s0, -1
	addi	$s1, $s1, -1
	
	and 	$s2, $s3, 0xf
	srl	$s3, $s3, 0x4
	
	addi	$s2, $s2, 0x30
	
	sb	$s2, 0($s1)
	
	bnez	$s0, bhloop
	
	# compute ASCII value
	# store at address and update address

	# pop used saved registers
	lw	$s3, 0($sp)
	addi	$sp, $sp, 4
	
	lw	$s2, 0($sp)
	addi	$sp, $sp, 4

	lw	$s1, 0($sp)
	addi	$sp, $sp, 4
	
	lw	$s0, 0($sp)
	addi	$sp, $sp, 4
	
	# return from subroutine
	jr	$ra

# ------------------------------ #
# fibonacci
#
# $a0: 32-bit binary value to conver to hexadecimal
# $a1: address to begin writing null terminated hex string
# ------------------------------ #
fibonacci:
	addi	$sp, $sp, -12			# increment stack pointer
	sw	$ra, 0($sp)			# push return address

	beq	$a0, $zero, return0		# if (n = 0) return 0
	li	$t0, 1
	beq	$a0, $t0, return1		# if (n = 1) return 1

	addi	$a0, $a0, -1
	jal	fibonacci			# fibonacci(n - 1)
	
	addi	$a0, $a0, -1
	jal	fibonacci			# fibonacci(n - 2)

	addition:
		

	# TODO: return0
	return0:
		li	$v0, 0
	# TODO: return1
	return1:
		li	$v0, 1

# ------------------------------ #
# double_add
#
# $a0: high 32-bits of first number
# $a1: low 32-bits of first number
# $a2: high 32-btis of second number
# $a3: low 32-bits of second number
# ------------------------------ #
double_add:
	# add low words
	addu	$v1, $a1, $a3
	
	# if overflow add 1 to $v0
	sltu	$t0, $v1, $a1
	
	# add high words
	add	$v0, $a0, $a2
	add	$v0, $v0, $t0
	
	jr 	$ra
	
# ------------------------------ #
# double_sra
#
# $a0: high 32-bits of number
# $a1: low 32-bits of number
# $a2: number of bits to shift
# ------------------------------ #
double_sra:
	add	$t0, $zero, $a2
	and	$t1, $zero, $zero
	
s_loop:	sll	$t1, $t1, 1
	or	$t1, $t1, 0x1
	sub	$t0, $t0, 1
	
	bnez 	$t0, s_loop
	
	and	$t1, $t1, $a0
	
	li	$t0, 32
	sub	$t0, $t0, $a2
	
	sllv	$t1, $t1, $t0
	
	sra	$v0, $a0, $a2
	srl	$v1, $a1, $a2
	or	$v1, $v1, $t1
	
	jr 	$ra
	