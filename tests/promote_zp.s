label:
	jmp label	;-4C 00 00
	jmp .local	;-4C 06 00
.local:
	jmp (.local)	;-6C 06 00
