
			add $t0, $zero, $zero, 352		# $t0 = 352
			lw $s0, $zero, $imm, 0x100		# $s0 = radius
			mul $s0, $s0, $s0, 0			# $s0 = radius^2
FIRST_LOOP:
			sub $t0, $t0, $imm, 1			# $t0 -= 1
			beq $imm, $zero, $t0, DONE	
			add $t1, $zero, $zero, 288		# $t1 = 288
SECOND_LOOP:
			add $a0, $zero, $imm, 175		# x axis of the center
			add $a1, $zero, $imm, 143		# y axis of the center
			sub $t1, $t1, $imm, 1			# $t1 -= 1
			beq $imm, $zero, $t1, FIRST_LOOP
			sub $a0, $a0, $t0, 0			# $a0 -= $t0
			mul $a0, $a0, $a0, 0			# $a0 = $a0 ^2
			sub $a1, $a1, $t1, 0			# $a1 -= $t1
			mul $a1, $a1, $a1, 0			# $a1 = $a1 ^2
			add $a1, $a1, $a0, 0			# $a1 = $a1 + $a0
			bgt $imm, $a1, $s0, SECOND_LOOP # jump to SECOND_LOOP if $a1 > radius^2
WRITE_PIXEL:
			out $t0, $zero, $imm, 19		# monitorx = $t0
			out $t1, $zero, $imm, 20		# monitory = $t0
			add $a1, $zero, $imm, 255		# $a1 = 255
			out $a1, $zero, $imm, 21		# monitordata = 255
			add $a1, $zero, $imm, 1			# $a1 = 1
			out $a1, $zero, $imm, 18		# monitorcmd = 1
			beq $imm, $zero, $zero, SECOND_LOOP
DONE:
			halt $zero, $zero, $zero, 0	# halt
