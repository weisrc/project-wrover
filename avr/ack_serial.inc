; Author: Wei
; Acknowledged Serial

.equ FCPU = 1000000
.equ SERIAL_BAUD = 4800
.equ SERIAL_UBRR = (FCPU / 16 / SERIAL_BAUD) - 1
.equ SERIAL_ACK = 123
.equ SERIAL_TIMEOUT = 1000
.equ SERIAL_WRITING_START = $ffff - SERIAL_TIMEOUT

.dseg

SERIAL_RX_HEAD: .byte 1
SERIAL_RX_TAIL: .byte 1
SERIAL_RX_BUFFER: .byte 128

SERIAL_TX_HEAD: .byte 1
SERIAL_TX_TAIL: .byte 1
SERIAL_TX_BUFFER: .byte 128

SERIAL_WRITING_H: .byte 1 ; 0: not writing, else: writing, this is the write cooldown
SERIAL_WRITING_L: .byte 1 ; if this reaches 0, then ACK serial timeout, resend

.cseg

; initialize serial
serial_init:
	push r16
	ldi r16, high(SERIAL_UBRR) ; set baud rate
	out UBRRH, r16
	ldi r16, low(SERIAL_UBRR)
	out UBRRL, r16

	ldi r16, (1 << RXEN) | (1 << TXEN) | (1 << RXCIE) ; enable rx, tx, rx interrupt
	out UCSRB, r16

	ldi r16, (1 << URSEL) | (1 << UPM1) | (1 << UPM0) | (1 << UCSZ1) | (1 << UCSZ0) ; odd parity, 8-bit
	out UCSRC, r16

    clr r16 ; init heads and tails
    sts SERIAL_RX_HEAD, r16
    sts SERIAL_RX_TAIL, r16
	sts SERIAL_TX_HEAD, r16
	sts SERIAL_TX_TAIL, r16
	sts SERIAL_WRITING_H, r16 ; init writing to 0
	sts SERIAL_WRITING_L, r16

	pop r16
	ret

; write to serial
; r16: data
serial_write:
	cli
	push XL
	push XH

	lds XL, SERIAL_TX_HEAD
	andi XL, $7F
	clr XH

	subi XL, low(-SERIAL_TX_BUFFER) ; X = HEAD + SERIAL_TX_BUFFER
	sbci XH, high(-SERIAL_TX_BUFFER)

	st X, r16 ; store data

	lds XH, SERIAL_TX_HEAD ; increment head
	inc XH
	sts SERIAL_TX_HEAD, XH

	pop XH
	pop XL
	sei
	ret

; update serial
serial_update:
	cli
	sbis UCSRA, UDRE
	rjmp serial_update_ret ; data register busy
    push XL
    push XH

	lds XH, SERIAL_WRITING_H
	cpi XH, 0
	breq serial_update_write ; if not writing, write

	lds XH, SERIAL_WRITING_H ; increment writing cooldown, will overflow
	lds XL, SERIAL_WRITING_L
	adiw XH:XL, 1
	sts SERIAL_WRITING_H, XH
	sts SERIAL_WRITING_L, XL
	rjmp serial_update_end

serial_update_write:
    lds XL, SERIAL_TX_TAIL
	lds XH, SERIAL_TX_HEAD
	cp XL, XH ; if tail == head, buffer is empty
	breq serial_update_end ; buffer is empty
    
	andi XL, 0x7F ; clip to 128
    clr XH

    subi XL, low(-SERIAL_TX_BUFFER) ; X = TAIL + SERIAL_TX_BUFFER
    sbci XH, high(-SERIAL_TX_BUFFER)

	; do not increment tail, only when ACK is received

serial_update_write_start:
	sbis UCSRA, UDRE ; data register busy
	rjmp serial_update_write_start

    ld XH, X
	out UDR, XH
	ldi XH, high(SERIAL_WRITING_START) ; set writing cooldown
	sts SERIAL_WRITING_H, XH
	ldi XH, low(SERIAL_WRITING_START)
	sts SERIAL_WRITING_L, XH
	
serial_update_end:
    pop XH
    pop XL
serial_update_ret:
	sei
	ret


; check if serial is empty, no data to read
serial_is_empty:
	push r16
	push r17
	lds r16, SERIAL_RX_TAIL
	lds r17, SERIAL_RX_HEAD
	cp r16, r17
	pop r17
	pop r16
	ret

; read from serial
; r16: data output
serial_read:
    push XL
    push XH

    lds XL, SERIAL_RX_TAIL
    andi XL, 0x7F
    clr XH

    subi XL, low(-SERIAL_RX_BUFFER) ; X = TAIL + SERIAL_RX_BUFFER
    sbci XH, high(-SERIAL_RX_BUFFER)

	lds r16, SERIAL_RX_TAIL ; increment tail
	inc r16
	sts SERIAL_RX_TAIL, r16

    ld r16, X

    pop XH
    pop XL
	ret


; serial interrupt
serial_rxc:
	push XL
	in XL, SREG
	push XL
	push XH
	push r16

	in XL, UCSRA
	in r16, UDR
	lds XH, SERIAL_WRITING_H
	cpi XH, 0
	brne serial_rxc_feedback ; if writing, send ACK
	andi XL, (1 << FE) | (1 << PE) ; check for errors
	brne serial_rxc_err

serial_rxc_ok:

	lds XL, SERIAL_RX_HEAD
	andi XL, 0x7F
	clr XH
	subi XL, low(-SERIAL_RX_BUFFER) ; X = HEAD + SERIAL_RX_BUFFER
	sbci XH, high(-SERIAL_RX_BUFFER)

	st X, r16
	out UDR, r16 ; echo as ACK

	lds r16, SERIAL_RX_HEAD ; increment head
	inc r16
	sts SERIAL_RX_HEAD, r16
	rjmp serial_rxc_end

serial_rxc_err:
	rjmp serial_rxc_end ; don't do anything

serial_rxc_feedback:
	clr r16
	sts SERIAL_WRITING_H, r16 ; stop writing
	sts SERIAL_WRITING_L, r16
	lds r16, SERIAL_TX_TAIL ; increment tail
	inc r16
	sts SERIAL_TX_TAIL, r16

serial_rxc_end:
	pop r16
	pop XH
	pop XL
	out SREG, XL
	pop XL
	reti
