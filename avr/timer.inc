.equ TIMER_TOP = $8000

timer_init:
	push r16

	in r16, TCCR1A
	ori r16, (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11) ; mode 14, fast pwm, ICR1 as TOP
	out TCCR1A, r16

	in r16, TCCR1B
	ori r16, (1 << CS10) | (1 << WGM13) | (1 << WGM12) ; mode 14: fast pwm, no prescaling
	out TCCR1B, r16

	ldi r16, high(TIMER_TOP)
	out ICR1H, r16 ; write high first, p. 105
	ldi r16, low(TIMER_TOP)
	out ICR1L, r16

	in r16, TIMSK ; enable overflow interrupt
	ori r16, 1 << TOIE1
	out TIMSK, r16

	pop r16
	ret
