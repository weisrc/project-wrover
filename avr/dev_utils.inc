
; (r16) -> (r16)
nibble_to_hex:
	cpi r16, 10
	brlo nibble_to_hex_ret
	subi r16, -('A' - '0' - 10)
nibble_to_hex_ret:
	subi r16, -'0'
	ret

; (r16) -> (r16 msd, r17 lsd)
byte_to_hex:
	mov r17, r16
	lsr r16
	lsr r16
	lsr r16
	lsr r16
	rcall nibble_to_hex
	push r16
	mov r16, r17
	andi r16, $0f
	rcall nibble_to_hex
	mov r17, r16
	pop r16
	ret


serial_print_hex:
	push r16
	push r17
	rcall byte_to_hex
	rcall serial_write
	mov r16, r17
	rcall serial_write
	pop r17
	pop r16
	ret

lcd_print_hex:
	push r16
	push r17
	rcall byte_to_hex
	rcall lcd_write
	mov r16, r17
	rcall lcd_write
	pop r17
	pop r16
	ret

print_sonars:
	push r16

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

	pop r16
	ret