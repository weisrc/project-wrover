; Author: Wei
; Tune player utilities

.equ PLAYER_TPB = 255 ; ticks per beat / 4

.dseg

PLAYER_ACTIVE: .byte 1 ; is the player active
PLAYER_HEAD: .byte 1 ; track which note
PLAYER_DURATION: .byte 1 ; how long to play the note
PLAYER_TICK: .byte 1 ; track which tick

PLAYER_BUFFER: .byte 128 ; buffer for the notes

.cseg

; init the player
player_init:
    push r16
    clr r16
    sts PLAYER_ACTIVE, r16
    sts PLAYER_HEAD, r16
    sts PLAYER_TICK, r16
    sts PLAYER_DURATION, r16
    pop r16
    ret

; update the player
player_update:
    push r16

    lds r16, PLAYER_ACTIVE
    cpi r16, 0
    breq player_update_end ; not active
    
    lds r16, PLAYER_TICK
    inc r16
    sts PLAYER_TICK, r16
    cpi r16, PLAYER_TPB
    brlo player_update_end ; not yet time to update
    clr r16
    sts PLAYER_TICK, r16

    lds r16, PLAYER_DURATION
    dec r16
    sts PLAYER_DURATION, r16
    brne player_update_end ; continue playing the same note

    rcall player_play

player_update_end:
    pop r16
    ret

; immediately play the note at the head
player_play:
    push XH
    push XL
    push ZH
    push ZL
    push r16

    clr r16
    sts PLAYER_TICK, r16
    sts PLAYER_DURATION, r16
    ldi r16, 1
    sts PLAYER_ACTIVE, r16

    lds XL, PLAYER_HEAD
	andi XL, 0x7f
	clr XH
	subi XL, low(-PLAYER_BUFFER) ; X = HEAD + PLAYER_BUFFER
	sbci XH, high(-PLAYER_BUFFER)

    ld XH, X ; load the note
    lds XL, PLAYER_HEAD
    inc XL
    sts PLAYER_HEAD, XL ; increment the head

    cpi XH, 0
    breq player_play_stop ; reached the end of tune

    mov XL, XH
    andi XL, $e0 ; get the duration
    lsr XL ; shift to get the duration * 4
    lsr XL
    lsr XL
    sts PLAYER_DURATION, XL ; set the duration

    andi XH, $1F ; get the note
    cpi XH, NOTE_SILENCE
    breq player_play_silence

    mov ZL, XH
    clr ZH
    subi ZL, low(-(NOTES << 1)) ; word space to byte space
    sbci ZH, high(-(NOTES << 1))
    lpm r16, Z
    rcall speaker_on
    rjmp player_play_end

player_play_stop:
    clr r16
    sts PLAYER_ACTIVE, r16

player_play_silence:
    rcall speaker_off

player_play_end:
    pop r16
    pop ZL
    pop ZH
    pop XL
    pop XH
    ret

; play the player at r16
player_set:
    push r16
    sts PLAYER_HEAD, r16
    rcall player_play
    pop r16
    ret
