; Wei Heng Jiang
; serial.inc

.equ FCPU = 1000000
.equ SERIAL_BAUD = 2400
.equ SERIAL_UBRR = (FCPU / 16 / SERIAL_BAUD) - 1

.dseg

SERIAL_RX_HEAD: .byte 1
SERIAL_RX_TAIL: .byte 1
SERIAL_RX_BUFFER: .byte 128

.cseg

serial_init:
	push r16
	ldi r16, high(SERIAL_UBRR)
	out UBRRH, r16
	ldi r16, low(SERIAL_UBRR)
	out UBRRL, r16

	ldi r16, (1 << RXEN) | (1 << TXEN) | (1 << RXCIE)
	out UCSRB, r16

	ldi r16, (1 << URSEL) | (1 << UCSZ1) | (1 << UCSZ0) ; 8-bit
	out UCSRC, r16

    clr r16
    sts SERIAL_RX_HEAD, r16
    sts SERIAL_RX_TAIL, r16

	pop r16
	ret

serial_write:
	sbis UCSRA, UDRE
	rjmp serial_write
	out UDR, r16
	ret


serial_is_empty:
	push r16
	push r17
	lds r16, SERIAL_RX_TAIL
	lds r17, SERIAL_RX_HEAD
	cp r16, r17
	pop r17
	pop r16
	ret

serial_read:
    push XL
    push XH

    lds XL, SERIAL_RX_TAIL
    andi XL, 0x7F
    clr XH

    subi XL, low(-SERIAL_RX_BUFFER)
    sbci XH, high(-SERIAL_RX_BUFFER)

	lds r16, SERIAL_RX_TAIL
	inc r16
	sts SERIAL_RX_TAIL, r16

    ld r16, X

    pop XH
    pop XL
	ret


serial_rxc:
	push XL
	in XL, SREG
	push XL
	push XH
	push r16
	
	lds XL, SERIAL_RX_HEAD
	andi XL, 0x7F
	clr XH

	subi XL, low(-SERIAL_RX_BUFFER)
	sbci XH, high(-SERIAL_RX_BUFFER)

	lds r16, SERIAL_RX_HEAD
	inc r16
	sts SERIAL_RX_HEAD, r16

	in r16, UDR
	st X, r16

	pop r16
	pop XH
	pop XL
	out SREG, XL
	pop XL
	reti
