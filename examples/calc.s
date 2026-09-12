.include "sys2061.i"

main:
	ldy #$0f
	jsr zp_save

	ldy #$0f
	jsr zp_restore
	rts

.include "zpsave.i"
.include "zpstack.i"
