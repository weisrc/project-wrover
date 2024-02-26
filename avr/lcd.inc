

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

.equ LCD_CLEAR = $01

.equ LCD_ENTRY = $04
.equ LCD_ENTRY_INC = $02
.equ LCD_ENTRY_SHIFT = $01

.equ LCD_LF = $0a

.cseg

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

	rcall lcd_clear_command

	ldi r16, LCD_ENTRY | LCD_ENTRY_INC
	rcall lcd_command

	ldi r16, 1
	rcall delay

	pop r16
	ret


lcd_command:
	push r16
	out LCD_DB, r16
	in r16, LCD_CTL
	ori r16, LCD_EN
	andi r16, ~LCD_RS
	out LCD_CTL, r16
	nop
	nop
	nop
	andi r16, ~LCD_EN
	out LCD_CTL, r16
	rcall lcd_wait
	pop r16
	ret

lcd_write:
	push r16
	out LCD_DB, r16
	in r16, LCD_CTL
	ori r16, LCD_EN | LCD_RS
	out LCD_CTL, r16
	nop
	nop
	nop
	andi r16, ~LCD_EN
	out LCD_CTL, r16
	rcall lcd_wait
	pop r16
	ret

lcd_wait:
	push r16
	ldi r16, 15
lcd_wait_loop:
	dec r16
	breq lcd_wait_loop
	pop r16
	ret

lcd_clear_command:
	push r16
	ldi r16, LCD_CLEAR
	rcall lcd_command
lcd_clear_command_wait:
	ldi r16, 40
	rcall lcd_wait
	dec r16
	breq lcd_clear_command_wait
	pop r16
	ret


lcd_print:
	push r16
lcd_print_loop:
	lpm r16, Z+
	cpi r16, 0
	breq lcd_print_end
	cpi r16, LCD_LF
	breq lcd_print_lf
	rcall lcd_write
	rjmp lcd_print_loop
lcd_print_lf:
	rcall lcd_second
	rjmp lcd_print_loop
lcd_print_end:
	pop r16
	ret

lcd_second:
	push r16
	ldi r16, $80 + $40
	rcall lcd_command
	pop r16
	ret