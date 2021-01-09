
			add $a0 , $zero, $zero, 0			# $a0 = 0
			add $a1, $zero, $zero, 0			# $a1 = 0
			
			beq $imm, $zero, $zero, READ_SECTOR
BUSY_WAIT:
			add $t0, $imm, $zero, 17			# $t0 = $diskstatus
			in $t0, $zero, $t0, 0				# $t0 = diskstatus
			bne $imm, $t0, $zero, BUSY_WAIT
			beq $ra, $zero, $zero, 0			# return
	
READ_SECTOR:	
			jal $imm, $zero, $zero, BUSY_WAIT	# check if disk is free
			add $t0, $zero, $imm, 15			# $t0 = &disksector
			out $a1, $zero, $t0, 0				# disksector = $a1
			add $t0, $t0, $imm, 1				# $t0 += 1 -> $t0 = &diskbuffer
			out $a0, $t0, $zero, 0				# diskbudffer = $a0
			sub $t0, $t0, imm, 2				# $t0 -= 2 -> $t0 = &diskcmd
			out $imm, $t0, $imm, 1				# diskcmd = 1
			
			add $a0, $a0, $imm, 128				# $a0 += 128
			add $a1, $a1, $imm, 1				# $a1 += 1
			sub $t0 , $imm, $a1, 4				# $t0 = 4 - $a1
			bne $imm, $t0, $zero, READ_SECTOR

WRITE_SECTOR:
			add $t0, $zero, $zero, 0			# $t0 = 0
			add $t1, $zero, $imm, 128			# $t1 = 128
			add $t2, $zero, $imm, 256			# $t2 = 256
			add $t3, $zero, $imm, 384			# $t3 = 384
			add $s0, $zero, $imm, 512			# $s0 = 512
			lw $a0, $zero, $t0, 0				# $a0 = mem[$t0]
			lw $a1, $zero, $t1, 0 				# $a1 = mem[$t1]
			xor $a0, $a0, $a1, 0				# $a0 = $a1 ^ $a0
			lw $a1, $zero, $t2, 0 				# $a1 = mem[$t2]
			xor $a0, $a0, $a1, 0				# $a0 = $a1 ^ $a0
			lw $a1, $zero, $t3, 0 				# $a1 = mem[$t3]
			xor $a0, $a0, $a1, 0				# $a0 = $a1 ^ $a0
			sw $a0, $zero, $s0, 0				# mem[$s0] = $a0
			add $t0, $t0, $zero, 1				# $t0 += 1
			add $t1, $t1, $imm, 1				# $t1 += 1
			add $t2, $t2, $imm, 1				# $t2 += 1
			add $t3, $t3, $imm, 1				# $t3 += 1
			add $s0, $s0, $imm, 1				# $s0 += 1
			add $a1, $zero, $imm, 127			# $a1 = 127
			blt $imm, $t0, $a1, WRITE_SECTOR 	# jump to WRITE_SECTOR if $t0 < 127
			
			jal $imm, $zero, $zero, BUSY_WAIT	# check if disk is free
			add $t0, $zero, $imm, 15			# $t0 = &disksector
			out $imm, $zero, $t0, 4				# disksector = 4
			add $t0, $t0, $imm, 1				# $t0 += 1 -> $t0 = &diskbuffer
			out $imm, $t0, $zero, 512			# diskbudffer = 512
			sub $t0, $t0, imm, 2				# $t0 -= 2 -> $t0 = &diskcmd
			out $imm, $t0, $imm, 2				# diskcmd = 2
			jal $imm, $zero, $zero, BUSY_WAIT	# wait for disk to finish
			halt $zero, $zero, $zero, 0			# halt
			
