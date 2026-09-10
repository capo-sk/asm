;-------------------------------------
; copy zero page content out of the way / copy it back in place
;-------------------------------------

;-------------------------------------
; backup the zero page content
; skip $00 and $01
;-------------------------------------
; input:
;   Y = destination page
; output:
;   none
; side effects:
;   A = ? ($ff)
;   X = unchanged
;   Y = 0
;   C = unchanged
;   N = clear
;   V = ?
;   Z = set
;   $fd-fe = dest address
; to the page pointed to by A
;-------------------------------------
zp_save:
.start = $00				;we could set to 2 if 6510
.pointer = $fd				;use $fd-fe as pointer
	lda	.pointer+1		;save content of pointer area on hardware stack
	pha
	lda	.pointer
	pha
	sty	.pointer+1		;hold destination address
	lda	#0
	sta	.pointer
	ldy	#.start			;start copying from here
.loop:	lda	0,y			;copy from .start to $ff
	sta	(.pointer),y
	iny
	bne	.loop
	ldy	#.pointer		;retrieve original content of pointer area and copy to destination
	pla
	sta	(.pointer),y
	iny
	pla
	sta	(.pointer),y
	rts

;-------------------------------------
; restore the zero page content
; skip $00-01, $fd-fe
;-------------------------------------
; input:
;   Y = source page
; output:
;   none
; side effects:
;   A = ?
;   X = ?
;   Y = .pointer
;   C = tbd
;   N = ?
;   V = ?
;   Z = ?
;   $fd-fe = dest address
;-------------------------------------
zp_restore:
.start = $02				;skip $00-01 because 6510
.pointer = $fd				;use $fd-fe as pointer
	sty	.pointer+1
	lda	#0
	sta	.pointer
	ldy	#.start
.loop:	cpy	.pointer		;copy all from .start to $ff except the two pointer bytes
	beq	.skip
	cpy	.pointer+1
	beq	.skip
	lda	(.pointer),y
	sta	0,y
.skip:	iny
	bne	.loop
	ldy	#.pointer		;copy the content of the pointer area
	lda	(.pointer),y
	ldx	(.pointer),y
	sta	.pointer
	stx	.pointer+1
	rts

