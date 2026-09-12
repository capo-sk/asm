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
zps_init:
	lda	#$ff
	sta	zps.sp
	rts

;-------------------------------------
; push a byte onto the stack
;-------------------------------------
; input:
;   A = data byte
; output:
;   none
;-------------------------------------
zps_phb:
	dec	zps.sp
	ldx	zps.sp
	sta	(0,x)
	rts

;-------------------------------------
; push a word onto the stack
;-------------------------------------
; input:
;   A = low byte
;   X = high byte
; output:
;   none
;-------------------------------------
zps_phw:
	dec	zps.sp
	stx	(zps.sp)
	dec	zps.sp
	sta	(zps.sp)
	rts

;-------------------------------------
; fetch a byte from the stack
;-------------------------------------
; input:
;   none
; output:
;   A = data byte
;-------------------------------------
zps_plb:
	lda	(zps.sp)
	inc	zps.sp
	rts

;-------------------------------------
; fetch a word from the stack
;-------------------------------------
; input:
;   none
; output:
;   A = low byte
;   X = high byte
;-------------------------------------
zps_plw:
	lda	(zps.sp)
	inc	zps.sp
	ldx	(zps.sp)
	inc	zps.sp
	rts

