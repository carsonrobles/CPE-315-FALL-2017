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
	
	# fibonacci tests
	li	$a0, 0		# test case: 0
	jal	fibonacci
	move	$a0, $v0
	li	$v0, 1
	syscall
	
	li	$a0, 1		# test case 1
	jal	fibonacci
	move	$a0, $v0
	li	$v0, 1
	syscall

	li	$a0, 5		# test case 5
	jal	fibonacci
	move	$a0, $v0
	li	$v0, 1
	syscall

	li	$a0, 10		# test case 10
	jal	fibonacci
	move	$a0, $v0
	li	$v0, 1
	syscall

	li	$a0, 20		# test case 20
	jal	fibonacci
	move	$a0, $v0
	li	$v0, 1
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
# ------------------------------ #
fibonacci:
	ble	$a0, 1, base			# if (n = 0 or n = 1) { base case }
	bgt	$a0, 1, recurse			# else { recursive case }

	base:					# if (n = 0 or n = 1)
		addi	$v0, $a0, 0		#	return n
		jr	$ra

	recurse:
		addi	$sp, $sp, -12		# allocate space for stack
		sw	$ra, 0($sp)		# store return address on stack
		sw	$a0, 4($sp)		# store n on the stack
	
		addi	$a0, $a0, -1
		jal	fibonacci		# fibonacci(n - 1)
		sw	$v0, 8($sp)		# store return value on stack

		lw	$a0, 4($sp)		# restore relevant n

		addi	$a0, $a0, -2
		jal	fibonacci		# fibonacci(n - 2)

		lw	$t0, 8($sp)		
		add	$v0, $t0, $v0		# return fibonacci(n - 2) + fibonacci(n - 1)

		lw	$ra, 0($sp)		# restore relevant return address
		addi	$sp, $sp, 12		# free stack frame
		jr	$ra

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
