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
