; Author: Wei
; Miscelaneous utility functions

.equ DELAY_50 = 12340

; delay subroutine
delay:
	push r16
delay_head:
	ldi r25, high(DELAY_50)
	ldi r24, low(DELAY_50)
delay_loop:
	sbiw r25:r24, 1
	brne delay_loop
	dec r16
	brne delay_head
	pop r16
	ret

; double signed 8-bit to unsigned 8-bit
; r16: signed 8-bit -> ~ 2 * r16 unsigned 8-bit
double_s8_to_u8:
	cpi r16, 128
	brlo double_s8_to_u8_skip
	ldi r16, 127
double_s8_to_u8_skip:
	lsl r16
	ret

; offset towards
; r16 will offset towards r17 by 1
offset_towards:
	push r17
	cp r16, r17
	breq offset_towards_end
	brlt offset_towards_inc
offset_towards_dec:
	dec r16
	rjmp offset_towards_end
offset_towards_inc:
	inc r16
offset_towards_end:
	pop r17
	ret