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
tmpspc:	.space	1

	.text
main:	and	$a1, $a1, $zero		# clear $a1
	la	$a1, hexstr
	
	

# ------------------------------ #
# bintohex
#
# $a0: 32-bit binary value to conver to hexadecimal
# $a1: address to begin writing null terminated hex string
# ------------------------------ #
bintohex:
	# init counter to decimal 10 and changeable address
	addi	$t0, $zero, 0xa 
	add	$t1, $a1, $zero
	
	# iterate over $a0, 4 bits at a time
	
	# compute ASCII value
	# store at address and update address
	
	
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
