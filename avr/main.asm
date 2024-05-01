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

	rcall timer_init
	rcall motor_init

	rcall serial_init
	rcall sonar_init
	rcall speaker_init
	rcall player_init
	rcall tunes_init
	rcall handle_init

	ldi r16, TUNES_XP_STARTUP_HEAD
	rcall player_set

	ldi r16, 20 ; wait a bit for the lcd
	rcall delay
	rcall lcd_init ; initialize the submodules

	ldi ZH, high(BOOT_MSG << 1) ; print the boot message
	ldi ZL, low(BOOT_MSG << 1)
	; rcall lcd_print

	sei ; enable interrupts

loop:
	rcall handle ; handle incoming commands
	rcall serial_update ; only required for ack_serial
	rcall motor_update ; smooth motor update
	rcall player_update
	rcall tunes_motor_update
	rcall tunes_sonar_update
	rjmp loop


BOOT_MSG: .db "WRover AVR!", LF, "Waiting ESP...", 0

.include "utils.inc"
.include "sonar.inc"
.include "timer.inc"
.include "motor.inc"
.include "lcd.inc"
.include "speaker.inc"
.include "ack_serial.inc"
.include "handle.inc"
.include "dev_utils.inc"
.include "notes.inc"
.include "player.inc"
.include "tunes.inc"
.exit