	add $sp, $zero, $imm, 1			# set $sp = 1
	sll $sp, $sp, $imm, 11			# set $sp = 1 << 11 = 2048
	lw $a0, $zero, $imm, 0x100		# get n from address 0x100
	lw $a1, $zero, $imm, 0x101		# get k from address 0x101
	jal $imm, $zero, $zero, BINOM	# calc $v0 = BINOM(x)
	sw $v0, $zero, $imm, 0x102		# store BINOM(x) in 0x102
	halt $zero, $zero, $zero, 0		# halt
BINOM:
	add $sp, $sp, $imm, -4			# adjust stack for 3 items
	sw $s0, $sp, $imm, 3			# save $s0
	sw $ra, $sp, $imm, 2			# save return address
	sw $a1, $sp, $imm, 1			# save argument
	sw $a0, $sp, $imm, 0			# save argument
	beq $imm, $a1, $zero, L2		# jump to L2 if k == 0
	beq $imm, $a1, $a0, L2			# jump to L2 if k == n
L1:
	sub $a0, $a0, $imm, 1			# calculate n - 1
	jal $imm, $zero, $zero, BINOM	# calc $v0=BINOM(n-1, k)
	add $s0, $v0, $imm, 0			# $s0 = BINOM(n-1, k)
	lw $a0, $sp, $imm, 0			# restore $a0 = n
	lw $a1, $sp, $imm, 1			#restore $a1 = k
	sub $a0, $a0, $imm, 1			# calculate n - 1
	sub $a1, $a1, $imm, 1			# calculate k - 1
	jal $imm, $zero, $zero, BINOM	# calc $v0=BINOM(n-1, k-1)
	add $s0, $v0, $imm, 0			# $s0 = BINOM(n-1, k-1)
	add $v0, $v0, $s0, 0			# $v0 = BINOM(x-2) + BINOM(x-1)
	lw $a0, $sp, $imm, 0			# restore $a0
	lw $a1, $sp, $imm, 1			# restore $a1
	lw $ra, $sp, $imm, 2			# restore $ra
	lw $s0, $sp, $imm, 3			# restore $s0
L2:
	add $sp, $sp, $imm, 4			# pop 4 items from stack
	add $v0, $zero, $imm, 1			# $v0 = 1
	beq $ra, $zero, $zero, 0		# and return

