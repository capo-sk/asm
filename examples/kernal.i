kprint = $ffd2
CHROUT = $ffd2

.macro kchrout chr
	lda	chr
	jsr	CHROUT
.endm
