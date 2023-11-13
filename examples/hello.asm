org 8000h
ld hl,Message
call PrintString
ret

PrintString:
    ld a,(hl)
    cp 255
    ret z
    inc hl
    call PrintChar
    jr PrintString

PrintChar:
    push hl
    push bc
    push de
    push af
    ld a,2
    call 1601h
    pop af
    push af
    rst 16
    pop af
    pop de
    pop bc
    pop hl
    ret

Message: db "Hello World 323!",255
