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

double_s8_to_u8:
	cpi r16, 128
	brlo double_s8_to_u8_skip
	ldi r16, 127
double_s8_to_u8_skip:
	lsl r16
	ret