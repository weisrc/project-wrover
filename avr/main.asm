;
; avr.asm
;
; Created: 2/2/2024 10:30:15 AM
; Author : 2136016
;

.nolist
.include "m8515def.inc"
.list

; equates used by term io
.equ FCPU_L = 1000000 ;used by termio rtn 
.equ BAUD = 4800    ;desired baud rate

.dseg

.cseg

reset: rjmp main
int0_echo: rjmp sonar_echo0
int1_echo: rjmp sonar_echo1
.org $06
timer1_overflow: rjmp sonar_update
.org $0d
int2_echo: rjmp sonar_echo2

.org $20

main:
    ldi R16, low(RAMEND) ; initiate the stack
	out SPL, R16
	ldi R16, high(RAMEND)
    out SPH, R16

	rcall lcd_init
	rcall serial_init
	rcall sonar_init
	rcall motor_init
	rcall timer_init

	sei

loop:

	ldi r16, 5
	rcall delay

	lds r16, SONAR_DELTA0H
	rcall serial_print_hex
	lds r16, SONAR_DELTA0L
	rcall serial_print_hex
	ldi r16, ' '
	rcall serial_write

	lds r16, SONAR_DELTA1H
	rcall serial_print_hex
	lds r16, SONAR_DELTA1L
	rcall serial_print_hex
	ldi r16, ' '
	rcall serial_write

	lds r16, SONAR_DELTA2H
	rcall serial_print_hex
	lds r16, SONAR_DELTA2L
	rcall serial_print_hex

	ldi r16, CR
	rcall serial_write

	rjmp loop




.include "utils.inc"
.include "sonar.inc"
.include "timer.inc"
.include "motor.inc"
.include "lcd.inc"
.include "serial.inc"
.exit