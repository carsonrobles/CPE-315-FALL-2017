# Lab 3, part 2

.data
table1:	.word	4, 342552, -3112 , 2157, 1054, -33543, 1233, -433433, 10101, 16384 

.text


main:

# [2a] Let's get the address of table1 into $t4
# What does this do?  (NOTE: It's a pseudo-instruction! see the assembler output).

	la	$t4, table1		# how does this work? 

# [2b]  Get a count into $t3
#  add your instruction that will initialize $t3
# Hint: how many words are in the list?

#  (add your code here ...)

#	Initialize the total (sum) value into a register (e.g., $t0) = 0
#	Add an instruction here that clears $t0

#  [2c] (add your code here ...)


loop:	lw	$t1, 0($t4)		# [2d]  what does this do ?


# [2e]  Now add the code needed to accumulate the sum, advance the $t4 pointer, and test the count 
#  (add your code here ...)

#	(advance your pointer to the list of entries, $t4. By how much? your instruction(s) here:

#	decrement the counter you are using in $t3. How?
#	exit the loop when the counter hits 0)
#	Finish the code that accumulates the total of entries in table1, putting
#	the result into $t0.


#   [2f]  (your code goes here)



#	[2g]  Now use the syscall capability.  See your green card.
# 	Print the value of $t0















 


