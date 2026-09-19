	.macro	basic_header linenum

* = 2049

.token_sys = 158

		.word .last_line, linenum
		.byte .token_sys, "2061", 0
.last_line:	.word 0
	.endm
