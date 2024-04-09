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
.org $09
usart_rxc: rjmp serial_rxc
.org $0d
int2_echo: rjmp sonar_echo2

.org $20

main:
    ldi R16, low(RAMEND) ; initiate the stack
	out SPL, R16
	ldi R16, high(RAMEND)
    out SPH, R16

	ldi r16, 20 ; wait a bit
	rcall delay

	rcall lcd_init ; initialize the submodules
	rcall serial_init
	rcall sonar_init
	rcall motor_init
	rcall timer0_init
	rcall timer1_init
	rcall handle_init

	ldi ZH, high(boot_msg << 1) ; print the boot message
	ldi ZL, low(boot_msg << 1)
	rcall lcd_print

	sei ; enable interrupts

loop:
	rcall handle ; handle incoming commands
	rcall motor_update ; smooth motor update
	rcall serial_update ; only required for ack_serial
	rjmp loop


boot_msg: .db "WRover AVR", LF, "Waiting ESP...", 0

.include "utils.inc"
.include "sonar.inc"
.include "timer.inc"
.include "motor.inc"
.include "lcd.inc"
; .include "serial.inc"
; .include "buffered_serial.inc"
.include "ack_serial.inc"
.include "handle.inc"
.include "dev_utils.inc"
.exit