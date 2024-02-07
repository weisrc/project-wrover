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


; (r16) -> (r16)
nibble_to_hex:
	cpi r16, 10
	brlo nibble_to_hex_ret
	subi r16, -('A' - '0' - 1.0)
nibble_to_hex_ret:
	subi r16, -'0'
	ret

; (r16) -> (r16 msd, r17 lsd)
byte_to_hex:
	mov r17, r16
	lsr r16
	lsr r16
	lsr r16
	lsr r16
	rcall nibble_to_hex
	push r16
	mov r16, r17
	andi r16, $0f
	rcall nibble_to_hex
	mov r17, r16
	pop r16
	ret
