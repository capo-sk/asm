	.macro name arg1 arg2
	nop
	&name $1 $2
	.endm

	&name $3 $4
