	move.w #$0123,d0
	move.b d0,d1
	move.w d1,d2
	move.l #$FFFF0000,d3
	move.w d0,d3
	move.l d3,d0
	move.w d2,d0
