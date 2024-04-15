; Author: Wei
; Speaker control utilities

.cseg

; init the speaker
speaker_init:
	push r16
		
	in r16, DDRB
	ori r16, 1 << PB0
	out DDRB, r16

	in r16, TCCR0
	ori r16, (1 << WGM01) | (1 << COM00) ; mode 2, ctc, toggle OC0
	out TCCR0, r16

	pop r16
	ret

; set speaker on
; r16: freq
speaker_on:
	push r16
	out OCR0, r16
	in r16, TCCR0
	ori r16, 2 << CS00 ; /8
	out TCCR0, r16
	pop r16
	ret

; turn the speaker off
speaker_off:
	push r16
	in r16, TCCR0
	andi r16, ~(2 << CS00)
	out TCCR0, r16
	pop r16
	ret