	# TODO: write to $zero
			add $a0, $zero, $zero, 0				# $a0 = 0, for first loop
			add $s0 , $zero, $imm, 0x120			# $s0 = 0x120
FIRST_LOOP:
			add $v0, $zero, $zero, 0				# $v0 = 0
			srl $a1, $a0, $imm, 2					# $a1 = $a0 // 4
			add $t0, $a1, $imm, 0x100				# $t0 = mem[0x100 + $a1]
			add $a1, $zero, $imm, 3					# $a1 = 3
			and $a1, $a1, $a0, 0					# $a1 = $a0 & $a1
			add $t1, $a1, $imm, 0x110				# $t1 = &(second matrix)
			add $a1, $zero, $imm, 15				# $a1 = 4, for second loop
			beq $imm, $a1, $a0, DONE				# jump to DONE if $a0 = 15
			add $a0, $a0, $imm, 1					# $a0 += 1
			add $a1, $zero, $imm, 4					# $a1 = 4, for second loop
SECOND_LOOP:
			sub $a1, $a1, $imm, 1					# $a1 -= 1
			lw $t2, $zero, $t0, 0 					# $t2 = (first matrix)[i][j]
			lw $t3, $zero, $t1, 0 					# $t3 = (second matrix)[i][j]
			mul $t2, $t2, $t3, 0					# $t2 = $t2 * $t3
			add $v0, $v0, $t2, 0					# $v0 += $t2
			add $t0 ,$t0 $imm,1						# $t0 += 1
			add $t1 ,$t1 $imm,4						# $t1 += 4
			bne $imm, $a1, $zero, SECOND_LOOP  	
WRITE:			
			sw $v0, $a0, $s0, 0						# mem[$a0 + $s0] = $v0
			beq $imm, $zero, $zero, FIRST_LOOP
DONE:
			halt $zero, $zero, $zero, 0	# halt
	
