.macro add op
	clc
	adc op
.endm

.macro addb op1 op2 dest
	lda op1
	&add op2
	sta dest
.endm

.macro addw op1 op2 dest
	&addb op1 op2 dest
	lda op1+1
	adc op2+1
	sta dest+1
.endm

* = $1000
&addw $fc $2533 $40

