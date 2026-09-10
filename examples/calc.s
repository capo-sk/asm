.include "sys2061.i"

main:
	lda #$0f
	jsr zp_save
	lda #$0f
	jsr zp_restore
	rts

.include "zpsave.i"
