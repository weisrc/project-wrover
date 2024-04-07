; Wei Heng Jiang
; serial.inc
; 
;

.equ FCPU = 1000000
.equ SERIAL_BAUD = 2400
.equ SERIAL_UBRR = (FCPU / 16 / SERIAL_BAUD) - 1

serial_init:
	push r16
	ldi r16, high(SERIAL_UBRR)
	out UBRRH, r16
	ldi r16, low(SERIAL_UBRR)
	out UBRRL, r16

	ldi r16, (1 << RXEN) | (1 << TXEN)
	out UCSRB, r16

	ldi r16, (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0) ; 8-bit
	out UCSRC, r16

	pop r16
	ret

serial_write:
	sbis UCSRA, UDRE
	rjmp serial_write
	out UDR, r16
	ret


serial_is_empty:
	push r16
	in r16, UCSRA
	andi r16, 1 << RXC
	pop r16
	ret


serial_read:
	in r16, UDR
	ret


