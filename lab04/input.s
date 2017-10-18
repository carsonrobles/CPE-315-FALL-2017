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
