Main:
	move.b #$24,d0
	bclr.l #$02,d0
	or.b #$03,d0
	move.b #$F8,d1
	jmp Main
	and.b d0,d1
	not.w d1
	xor.w #$FF00
	move.b d1,$22
	bset.b #$05,$22
	bchg.b #$01,$22
