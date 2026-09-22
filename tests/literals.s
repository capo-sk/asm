* = $1234						;-34 12

label:
var = 100

; basics
	.byte	42, $42, "*B", %00101011, $992c		;-2a 42 2a 42 2b 2c
	.word	!42, 4242, $4241			;-2a 00 92 10 41 42
	.word	label, var				;-34 12 64 00
	.byte	label, <label, >label, <var, >var	;-34 34 12 64 00
	.word	^var					;-00 64
	.byte	^var, !var				;-00 64
	.byte	0-1, $78+16, $f8+16			;-ff 88 08
	.word	label-$34				;-00 12
