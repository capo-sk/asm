.include "start.m"
.include "kernal.i"

	&basic_header 42

; main
* = 2061
start:
	jmp main		;4C1B08

hello:	.byte "HELLO WORLD",0	;48454C4C4F20574F524C4400

main:
	ldx #0			;A200
loop:	lda hello,x		;BD0F08
	beq finish		;F006
	jsr kprint		;20D2FF
	inx			;E8
	bne loop		;D0F5
finish:	rts			;60

* = 4000
extra:	asl a			;0A
	and #1			;2901
	lda $40			;A540
	adc $50,x		;7550
	ldx $60,y		;8660
	ldy $1000		;AC0010
	ora $2000,x		;1D0020
	sbc $3000,y		;F90030
	sec			;38
	bcs extra		;B0EB
	cmp ($70,x)		;C170
	eor ($80),y		;5180
	jmp ($75)		;6C7500

	.macro addw add1 add2 sum
	clc
	lda add1
	adc add2
	sta sum
	lda add1+1
	adc add2+1
	sta sum+1
	.endm

	.macro clrpage page
	lda #0
	tax
..loop:
	sta ^page,x
	inx
	bne ..loop
	.endm

	&addw $f0 $f2 $f4
	&clrpage $50
	&clrpage $51

