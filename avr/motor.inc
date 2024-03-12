.equ MOTOR0_PWM = PD5
.equ MOTOR1_PWM = PE2

.equ MOTOR0_REV = PD6
.equ MOTOR1_REV = PD7
.equ MOTOR_MODE = PORTD

.dseg

MOTOR_UPDATE_COUNTER: .byte 1
MOTOR0_TARGET: .byte 1
MOTOR1_TARGET: .byte 1
MOTOR0_CURRENT: .byte 1
MOTOR1_CURRENT: .byte 1

.cseg

motor_init:
	push r16

	sbi DDRD, MOTOR0_PWM
	sbi DDRE, MOTOR1_PWM

	sbi DDRD, MOTOR0_REV
	sbi DDRD, MOTOR1_REV

	clr r16
	sts MOTOR0_TARGET, r16
	sts MOTOR1_TARGET, r16
	sts MOTOR_UPDATE_COUNTER, r16
	rcall motor0_set
	rcall motor1_set

	pop r16
	ret



motor_update:
	push r16
	lds r16, MOTOR_UPDATE_COUNTER
	inc r16
	sts MOTOR_UPDATE_COUNTER, r16
	brne motor_update_end
	push r17
	lds r16, MOTOR0_CURRENT
	lds r17, MOTOR0_TARGET
	rcall offset_towards
	rcall motor0_set

	lds r16, MOTOR1_CURRENT
	lds r17, MOTOR1_TARGET
	rcall offset_towards
	rcall motor1_set

	pop r17
motor_update_end:
	pop r16
	ret

motor0_set:
	push r16
	sts MOTOR0_CURRENT, r16
	cpi r16, 0
	brge motor0_set_for
motor0_set_rev:
	sbi MOTOR_MODE, MOTOR0_REV
	neg r16
	rjmp motor0_set_end
motor0_set_for:
	cbi MOTOR_MODE, MOTOR0_REV
motor0_set_end:
	rcall double_s8_to_u8
	rcall motor0_speed
	pop r16
	ret


motor1_set:
	push r16
	sts MOTOR1_CURRENT, r16
	cpi r16, 0
	brge motor1_set_for
motor1_set_rev:
	sbi MOTOR_MODE, MOTOR1_REV
	neg r16
	rjmp motor1_set_end
motor1_set_for:
	cbi MOTOR_MODE, MOTOR1_REV
motor1_set_end:
	rcall double_s8_to_u8
	rcall motor1_speed
	pop r16
	ret


motor0_speed:
	push r16
	out OCR1AH, r16
	clr r16
	out OCR1AL, r16
	pop r16
	ret


motor1_speed:
	push r16
	out OCR1BH, r16
	clr r16
	out OCR1BL, r16
	pop r16
	ret