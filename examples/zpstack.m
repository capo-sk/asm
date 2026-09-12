;-------------------------------------
; data stack on zero page
; $ff holds stack pointer
; stack starts from $fe and grows backwards
;-------------------------------------

zps.sp = $ff

;-------------------------------------
; init the stack
; set the pointer to $ff
;-------------------------------------
; input:
;   none
; output:
;   none
;-------------------------------------
	macro	zps_init
	lda	#$ff
	sta	zps.sp
	endm

	macro	spx
	ldx	zps.sp
	endm

	macro	xsp
	stx	zps.sp
	endm

	macro	zps_pha
	dex
	sta	(0,x)
	endm

	macro	zps_phay
	dex
	sta	(0,x)
	dex
	sty	(0,x)
	endm

	macro	zps_phb addr
	lda	addr
	&zps_pha
	endm

	macro	zps_phw addr
	lda	addr+1
	&zps_pha
	lda	addr
	&zps_pha
	endm

	macro
;-------------------------------------
; push a byte onto the stack
;-------------------------------------
; input:
;   A = data byte
; output:
;   none
;-------------------------------------
	macro	phb
	dec	zps.sp
	sta	(zps.sp)
	endm

;-------------------------------------
; push a word onto the stack
;-------------------------------------
; input:
;   A = low byte
;   X = high byte
; output:
;   none
;-------------------------------------
	macro	phw
	dec	zps.sp
	stx	(zps.sp)
	dec	zps.sp
	sta	(zps.sp)
	endm

;-------------------------------------
; fetch a byte from the stack
;-------------------------------------
; input:
;   none
; output:
;   A = data byte
;-------------------------------------
	macro	plb
	lda	(zps.sp)
	inc	zps.sp
	endm

;-------------------------------------
; fetch a word from the stack
;-------------------------------------
; input:
;   none
; output:
;   A = low byte
;   X = high byte
;-------------------------------------
	macro	plw
	lda	(zps.sp)
	inc	zps.sp
	ldx	(zps.sp)
	inc	zps.sp
	endm

