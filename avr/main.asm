;
; avr.asm
;
; Created: 2/2/2024 10:30:15 AM
; Author : 2136016
;

.nolist
.include "m8515def.inc"
.list

.equ LF = $0a
.equ CR = $0d

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

	ldi r16, 10
	rcall delay

	rcall lcd_init
	rcall serial_init
	rcall sonar_init
	rcall motor_init
	rcall timer_init
	rcall handle_init

	ldi ZH, high(boot_msg << 1)
	ldi ZL, low(boot_msg << 1)
	rcall lcd_print

	sei
loop:
	rcall handle
	;rcall print_sonars
	rcall motor_update
	rjmp loop


boot_msg: .db "WRover AVR", LF, "Waiting ESP...", 0

.include "utils.inc"
.include "sonar.inc"
.include "timer.inc"
.include "motor.inc"
.include "lcd.inc"
.include "serial.inc"
.include "handle.inc"
.include "dev_utils.inc"
.exit