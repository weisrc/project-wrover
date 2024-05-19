; Author: Wei
; LCD utility functions

.equ LCD_DB = PORTA
.equ LCD_CTL = PORTC

.equ LCD_DB_DD = DDRA
.equ LCD_CTL_DD = DDRC

.equ LCD_RS = 1 << PC1
.equ LCD_EN = 1 << PC0

.equ LCD_FN = $20
.equ LCD_FN_8BIT = $10
.equ LCD_FN_2LINE = $08
.equ LCD_FN_FONT = $04

.equ LCD_DIS = $08
.equ LCD_DIS_ON = $04
.equ LCD_DIS_CURSOR = $02
.equ LCD_DIS_BLINK = $01

.equ LCD_DIS_CLEAR = $01

.equ LCD_ENTRY = $04
.equ LCD_ENTRY_INC = $02
.equ LCD_ENTRY_SHIFT = $01

.equ LCD_LF = $0a

.cseg

; Initialize the LCD, follows the initialization sequence in the datasheet
lcd_init:
	push r16

	ldi r16, $ff
	out LCD_DB_DD, r16

	in r16, LCD_CTL_DD
	ori r16, LCD_RS | LCD_EN
	out LCD_CTL_DD, r16 

	ldi r16, 1
	rcall delay

	ldi r16, LCD_FN | LCD_FN_8BIT | LCD_FN_2LINE | LCD_FN_FONT
	rcall lcd_command
	rcall lcd_command

	ldi r16, LCD_DIS | LCD_DIS_ON | LCD_DIS_CURSOR | LCD_DIS_BLINK
	rcall lcd_command

	rcall lcd_clear

	ldi r16, LCD_ENTRY | LCD_ENTRY_INC
	rcall lcd_command

	ldi r16, 1
	rcall delay

	pop r16
	ret


; send a command to the LCD
; r16: command
lcd_command:
	push r16
	out LCD_DB, r16 ; output command to data bus
	in r16, LCD_CTL
	nop
	nop
	ori r16, LCD_EN ; set enable
	andi r16, ~LCD_RS ; enter command mode
	out LCD_CTL, r16 ; output to control port
	nop
	nop
	nop
	nop
	andi r16, ~LCD_EN ; clear enable
	out LCD_CTL, r16
	rcall lcd_wait
	pop r16
	ret

; send a character to the LCD
; r16: character
lcd_write:
	push r16
	out LCD_DB, r16 ; output character to data bus
	in r16, LCD_CTL
	ori r16, LCD_EN | LCD_RS ; set enable and RS
	out LCD_CTL, r16
	nop
	nop
	nop
	andi r16, ~LCD_EN ; clear enable
	out LCD_CTL, r16
	rcall lcd_wait
	pop r16
	ret

; wait for the LCD to process the action
lcd_wait:
	push r16
	ldi r16, 30
lcd_wait_loop:
	dec r16
	breq lcd_wait_loop
	pop r16
	ret

; clear the LCD
lcd_clear:
	push r16
	ldi r16, LCD_DIS_CLEAR
	rcall lcd_command
	ldi r16, 40
lcd_clear_wait:
	rcall lcd_wait
	dec r16
	brne lcd_clear_wait
	pop r16
	ret

; print a string to the LCD
; Z: string
lcd_print:
	push r16
lcd_print_loop:
	lpm r16, Z+
	cpi r16, 0
	breq lcd_print_end ; end of string
	cpi r16, LCD_LF ; line feed
	breq lcd_print_lf ; go to the second line
	rcall lcd_write ; write character
	rjmp lcd_print_loop
lcd_print_lf:
	rcall lcd_second
	rjmp lcd_print_loop
lcd_print_end:
	pop r16
	ret

; go to start
lcd_start:
	push r16
	ldi r16, $80
	rcall lcd_command
	pop r16
	ret


; go to the second line
lcd_second:
	push r16
	ldi r16, $80 + $40
	rcall lcd_command
	pop r16
	ret