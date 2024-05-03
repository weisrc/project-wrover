; Author: Wei
; File containing some tunes

TUNES_START:
TUNES_METRO: .db    NOTE_FAS | NOTE_W, \
                    NOTE_SI | NOTE_W, \
                    NOTE_FAHS | NOTE_W, NOTE_FAHS | NOTE_Q, 0

TUNES_XP_STARTUP: .db   NOTE_REHS | NOTE_W, \
                        NOTE_RES | NOTE_H, \
                        NOTE_LAS | NOTE_H, \
                        NOTE_SOS | NOTE_W, \
                        NOTE_REHS | NOTE_H, \
                        NOTE_LAS | NOTE_W, 0

TUNES_WARNING0: .db NOTE_REH | NOTE_W, NOTE_DOHS | NOTE_W, \
                    NOTE_DOH | NOTE_W, NOTE_SI | NOTE_Q, \
                    NOTE_DOH | NOTE_Q, NOTE_SI | NOTE_Q, \
                    NOTE_DOH | NOTE_Q, NOTE_SI | NOTE_H, 0

TUNES_WARNING1: .db NOTE_SI | NOTE_W, NOTE_DOH | NOTE_W, \
                    NOTE_DOHS | NOTE_W, NOTE_REH | NOTE_W, 0

TUNES_WARNING2: .db NOTE_RE | NOTE_Q, NOTE_FAS | NOTE_Q, \
                    NOTE_SI | NOTE_Q, NOTE_REH | NOTE_Q, \
                    NOTE_DOHS | NOTE_Q, NOTE_SI | NOTE_Q, \
                    NOTE_LAS | NOTE_Q, NOTE_SI | NOTE_Q, \
                    NOTE_SILENCE | NOTE_W, \
                    NOTE_SI | NOTE_H, NOTE_DOHS | NOTE_H, \
                    NOTE_REH | NOTE_H, NOTE_MIH | NOTE_H, \
                    NOTE_FAHS | NOTE_W, NOTE_REH | NOTE_H, \
                    NOTE_FAHS | NOTE_W, NOTE_REH | NOTE_H, \
                    NOTE_FAHS | NOTE_W, 0

.equ TUNES_METRO_HEAD = (TUNES_METRO - TUNES_START) << 1
.equ TUNES_XP_STARTUP_HEAD = (TUNES_XP_STARTUP - TUNES_START) << 1
.equ TUNES_WARNING0_HEAD = (TUNES_WARNING0 - TUNES_START) << 1
.equ TUNES_WARNING1_HEAD = (TUNES_WARNING1 - TUNES_START) << 1
.equ TUNES_WARNING2_HEAD = (TUNES_WARNING2 - TUNES_START) << 1
.equ MIN_SONAR_DISTANCE_H = 2

.cseg

; init the tunes, copy the tunes into the player buffer
tunes_init:
    push ZH
    push ZL
    push XH
    push XL

    ldi ZH, high(TUNES_METRO << 1)
    ldi ZL, low(TUNES_METRO << 1)
    ldi XH, high(PLAYER_BUFFER)
    ldi XL, low(PLAYER_BUFFER)
    rcall copy_cstring_zx

    ldi ZH, high(TUNES_XP_STARTUP << 1)
    ldi ZL, low(TUNES_XP_STARTUP << 1)
    ldi XH, high(PLAYER_BUFFER + TUNES_XP_STARTUP_HEAD)
    ldi XL, low(PLAYER_BUFFER + TUNES_XP_STARTUP_HEAD)
    rcall copy_cstring_zx

    ldi ZH, high(TUNES_WARNING0 << 1)
    ldi ZL, low(TUNES_WARNING0 << 1)
    ldi XH, high(PLAYER_BUFFER + TUNES_WARNING0_HEAD)
    ldi XL, low(PLAYER_BUFFER + TUNES_WARNING0_HEAD)
    rcall copy_cstring_zx

    ldi ZH, high(TUNES_WARNING1 << 1)
    ldi ZL, low(TUNES_WARNING1 << 1)
    ldi XH, high(PLAYER_BUFFER + TUNES_WARNING1_HEAD)
    ldi XL, low(PLAYER_BUFFER + TUNES_WARNING1_HEAD)
    rcall copy_cstring_zx

    ldi ZH, high(TUNES_WARNING2 << 1)
    ldi ZL, low(TUNES_WARNING2 << 1)
    ldi XH, high(PLAYER_BUFFER + TUNES_WARNING2_HEAD)
    ldi XL, low(PLAYER_BUFFER + TUNES_WARNING2_HEAD)
    rcall copy_cstring_zx

    pop XL
    pop XH
    pop ZL
    pop ZH
    ret


; play the metro sound when motor starts
tunes_motor_update:
    push r16
    push r17

    lds r16, MOTOR0_CURRENT
    lds r17, MOTOR1_CURRENT
    or r16, r17
    brne tunes_motor_update_end

    lds r16, MOTOR0_TARGET
    lds r17, MOTOR1_TARGET
    or r16, r17
    breq tunes_motor_update_end

    ldi r16, TUNES_METRO_HEAD
	rcall player_set

tunes_motor_update_end:
    pop r17
    pop r16
    ret

tunes_sonar_update:
	push r16

	lds r16, SONAR_DELTA0H
	cpi r16, MIN_SONAR_DISTANCE_H
	brlo tunes_sonar_update0
    
	lds r16, SONAR_DELTA1H
	cpi r16, MIN_SONAR_DISTANCE_H
	brlo tunes_sonar_update1
   
	lds r16, SONAR_DELTA2H
	cpi r16, MIN_SONAR_DISTANCE_H
	brlo tunes_sonar_update2

    rjmp tunes_sonar_update_end
	
tunes_sonar_update0:
    ldi r16, TUNES_WARNING0_HEAD
	rcall player_set
    rjmp tunes_sonar_update_end
tunes_sonar_update1:
    ldi r16, TUNES_WARNING1_HEAD
    rcall player_set
    rjmp tunes_sonar_update_end
tunes_sonar_update2:
    ldi r16, TUNES_WARNING2_HEAD
    rcall player_set

tunes_sonar_update_end:
	pop r16
	ret