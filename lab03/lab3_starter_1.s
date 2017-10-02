# Lab 3, part 1

.data
test1:	.word	0x5555

.text


main:

# Here's your first MIPS instruction.  it's an "or word immediate" (ori)

#	[1a]  First, let's load a value into $t0  
	ori	$t0, $zero, 0x0001	# load 1 into reg $t0

#	[1b]  Now, let's try the same thing to register $t1 with a different value: 
	ori	$t1, $zero, 0x00F2

#	[1c]  Now, let's try adding two numbers. Like, in the registers $t0 and $t1 
	add	$t2, $t0, $t1		# What are the results ?

#	[1d]  Now, you write an instruction that adds an immediate value of 0x33 to $t2
#	And puts the result in $t3.  What's $t3 afterwards ? 
#    ! YOUR CODE GOES HERE !

#	[1e]  What if we want to load a 16-bit immediate value? 
	li	$t0, 0x5145	# 16 bits into low bits of $t0

#	[1f]  But what if we want to put a value into the high 16 bits of $t0? 
	lui	$t0, 0x3333		# what's the result?

#	[1g]  How can we load a full 32 bits into a destination register ? 
	lui	$t0, 0x4321		# High bits
	ori	$t0, $t0, 0x8765	# or'd with low bits
#	what's the result in $t0 ?

#	[1h]  What the heck is a "pseudoinstruction" ?

	li	$t0, 0x2345		# result in $t0 is ? 

#	[1i]  And, what if it is bigger than 32 bits?

	li 	$t0, 0x53468ACE		# This loads into $t0 - HOW ? 

#	[1j]  What are the actual instructions produced by the assembler to do this?

	nop				# This does nothing. What is it?  

 


