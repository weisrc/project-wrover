; Author: Wei
; Ultrasonic sensor driver

.equ TRIGGER = PD4

.dseg

SONAR_START0L: .byte 1
SONAR_START0H: .byte 1
SONAR_END0L: .byte 1
SONAR_END0H: .byte 1
SONAR_DELTA0L: .byte 1
SONAR_DELTA0H: .byte 1

SONAR_START1L: .byte 1
SONAR_START1H: .byte 1
SONAR_END1L: .byte 1
SONAR_END1H: .byte 1
SONAR_DELTA1L: .byte 1
SONAR_DELTA1H: .byte 1

SONAR_START2L: .byte 1
SONAR_START2H: .byte 1
SONAR_END2L: .byte 1
SONAR_END2H: .byte 1
SONAR_DELTA2L: .byte 1
SONAR_DELTA2H: .byte 1

.cseg

; initialize the sonar driver
sonar_init:
	push r16

	in r16, GICR
	ori r16, (1 << INT0) | (1 << INT1) | (1 << INT2) ; init bus and int0 and int1
	out GICR, r16

	in r16, MCUCR
	ori r16, (1 << ISC00) | (1 << ISC10) ; any logical change for int0 and int1
	out MCUCR, r16

	sbi DDRD, TRIGGER

	pop r16
	ret

; update sonar, called when Timer 1 overflows, begin to measure distance
sonar_update:
	push r16
	in r16, SREG
	push r16

	rcall sonar_compute ; compute the delta time

	in r16, EMCUCR
	ori r16, 1 << ISC2
	out EMCUCR, r16 ; set ISC2 to rising edge

	sbi PORTD, TRIGGER ; trigger for 10us
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	cbi PORTD, TRIGGER

	pop r16
	out SREG, r16
	pop r16
	reti

; compute the delta time for each sonar
sonar_compute:
	push r16
	push r17

	lds r16, SONAR_END0L ; delta = end - start
	lds r17, SONAR_START0L
	sub r16, r17
	sts SONAR_DELTA0L, r16
	lds r16, SONAR_END0H
	lds r17, SONAR_START0H
	sbc r16, r17
	sts SONAR_DELTA0H, r16

	lds r16, SONAR_END1L
	lds r17, SONAR_START1L
	sub r16, r17
	sts SONAR_DELTA1L, r16
	lds r16, SONAR_END1H
	lds r17, SONAR_START1H
	sbc r16, r17
	sts SONAR_DELTA1H, r16

	lds r16, SONAR_END2L
	lds r17, SONAR_START2L
	sub r16, r17
	sts SONAR_DELTA2L, r16
	lds r16, SONAR_END2H
	lds r17, SONAR_START2H
	sbc r16, r17
	sts SONAR_DELTA2H, r16

	pop r17
	pop r16
	ret

; sonar0 echo interrupt
sonar_echo0:
	push r16
	in r16, SREG
	push r16
	sbis PIND, PD2
	rjmp sonar_echo0_low
sonar_echo0_high:
	in r16, TCNT1L
	sts SONAR_START0L, r16 ; set start time if echo goes high
	in r16, TCNT1H
	sts SONAR_START0H, r16
	rjmp sonar_echo0_end
sonar_echo0_low:
	in r16, TCNT1L
	sts SONAR_END0L, r16 ; set end time if echo goes low
	in r16, TCNT1H
	sts SONAR_END0H, r16
sonar_echo0_end:
	pop r16
	out SREG, r16
	pop r16
	reti

; sonar1 echo interrupt
sonar_echo1:
	push r16
	in r16, SREG
	push r16
	sbis PIND, PD3
	rjmp sonar_echo1_low
sonar_echo1_high:
	in r16, TCNT1L
	sts SONAR_START1L, r16 ; set start time if echo goes high
	in r16, TCNT1H
	sts SONAR_START1H, r16
	rjmp sonar_echo1_end
sonar_echo1_low:
	in r16, TCNT1L
	sts SONAR_END1L, r16 ; set end time if echo goes low
	in r16, TCNT1H
	sts SONAR_END1H, r16
sonar_echo1_end:
	pop r16
	out SREG, r16
	pop r16
	reti

; sonar2 echo interrupt
sonar_echo2:
	push r16
	in r16, SREG
	push r16
	sbis PINE, PE0
	rjmp sonar_echo2_low
sonar_echo2_high:
	in r16, TCNT1L
	sts SONAR_START2L, r16 ; set start time if echo goes high
	in r16, TCNT1H
	sts SONAR_START2H, r16

	in r16, EMCUCR
	andi r16, ~(1 << ISC2)
	out EMCUCR, r16 ; set ISC2 to falling edge

	rjmp sonar_echo2_end
sonar_echo2_low:
	in r16, TCNT1L
	sts SONAR_END2L, r16 ; set end time if echo goes low
	in r16, TCNT1H
	sts SONAR_END2H, r16
sonar_echo2_end:
	pop r16
	out SREG, r16
	pop r16
	reti