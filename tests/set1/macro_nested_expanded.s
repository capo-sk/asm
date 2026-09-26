* = $1000
	lda $fc
	clc
	adc $2533
	sta $40
	lda $fc+1
	adc $2533+1
	sta $40+1
