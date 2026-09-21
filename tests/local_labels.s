* = $1000

	.macro loopme count1 count2
		ldx count1
..loopX:	ldy count2
..loopY:	dey
		bne ..loopY
		dex
		bne ..loopX
	.endm

big:
	ldx #100
.loop:	dex
	bne .loop
	rts

small:
	ldy #10
.loop:	dey
	bne .loop
	rts

verybig:
	&loopme 100 101
	&loopme 200 250
	rts
