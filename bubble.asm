			
			add $a0, $zero, $imm, 1024		# $a0 = 1024
			add $t0, $zero, $zero, 0		# $t0 = 0
FIRST_LOOP:
			add $t0, $t0, $imm, 1			# $t0 += 1
			add $t1, $zero, $imm , 16		# $t1 = 16
			beq $imm, $t0, $t1, DONE	
			add $t1, $zero, $zero , 0		# $t1 = 0
SECOND_LOOP:
			sub $t2, $imm, $t0, 16			# $t2 = 16 - $t0
			beq $imm, $t2, $zero, FIRST_LOOP
			lw $t2, $a0, $t1, 0				# $t2 = arr[$a0 + $t1]
			add $t1, $zero, $imm , 1		# $t1 += 1
			lw $t3, $a0, $a0, 0				# $t3 = arr[$a0 + $t1]
			bge $imm, $t2, $t3, SECOND_LOOP # if $t2 >= $t3 continue
			sw $t2, $a0, $t1, 0				# mem[$a0 + $t1] = $t2
			sub $t1 ,$t1, $imm, 1			$ $t1 -= 1
			sw $t3, $a0, $t1, 0				# mem[$a0 + $t1] = $t3
			add $t1 ,$t1, $imm, 1			$ $t1 += 1
			
DONE:
			halt $zero, $zero, $zero, 0		# halt
