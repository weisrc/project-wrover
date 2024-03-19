
.equ MODE_NONE = 0
.equ MODE_SONAR0 = 1
.equ MODE_SONAR1 = 2
.equ MODE_SONAR2 = 3
.equ MODE_MOTOR0 = 4
.equ MODE_MOTOR1 = 5
.equ MODE_WRITE = 6
.equ MODE_COMMAND = 7
.equ MODE_CLEAR = 8
.equ MODE_LAST = 9

.dseg

MODE: .byte 1

.cseg

handle_init:
	push r16
	clr r16
	sts MODE, r16
	pop r16
	ret


handle:
	push r16
	push r17

	rcall serial_is_empty
	breq handle_end
	rcall serial_read

	lds r17, MODE
	cpi r17, MODE_NONE
	breq handle_none
	cpi r17, MODE_MOTOR0
	breq handle_motor0
	cpi r17, MODE_MOTOR1
	breq handle_motor1
	cpi r17, MODE_WRITE
	breq handle_write
	cpi r17, MODE_COMMAND
	breq handle_command

	rjmp handle_done
	
handle_none:
	cpi r16, MODE_LAST
	brsh handle_none_end
	sts MODE, r16
	rcall handle_single
handle_none_end:
	rjmp handle_end

handle_motor0:
	sts MOTOR0_TARGET, r16
	rjmp handle_done

handle_motor1:
	sts MOTOR1_TARGET, r16
	rjmp handle_done

handle_write:
	rcall lcd_write
	rjmp handle_done

handle_command:
	rcall lcd_command
	rjmp handle_done

handle_done:
	ldi r17, MODE_NONE
	sts MODE, r17

handle_end:
	pop r17
	pop r16
	ret



handle_single:
	push r16
	lds r16, MODE
	cpi r16, MODE_SONAR0
	breq handle_single_sonar0
	cpi r16, MODE_SONAR1
	breq handle_single_sonar1
	cpi r16, MODE_SONAR2
	breq handle_single_sonar2
	cpi r16, MODE_CLEAR
	breq handle_single_clear

	rjmp handle_single_end

handle_single_sonar0:
	lds r16, SONAR_DELTA0H
	rcall serial_write
	lds r16, SONAR_DELTA0L
	rcall serial_write
	rjmp handle_single_done

handle_single_sonar1:
	lds r16, SONAR_DELTA1H
	rcall serial_write
	lds r16, SONAR_DELTA1L
	rcall serial_write
	rjmp handle_single_done

handle_single_sonar2:
	lds r16, SONAR_DELTA2H
	rcall serial_write
	lds r16, SONAR_DELTA2L
	rcall serial_write
	rjmp handle_single_done

handle_single_clear:
	rcall lcd_clear
	rjmp handle_single_done

handle_single_done:
	ldi r16, MODE_NONE
	sts MODE, r16
handle_single_end:
	pop r16
	ret