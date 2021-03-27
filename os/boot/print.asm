print:
    pusha ; push all registers to the stack

; iterate the string and print it until we get a null (0x00) character, like in c
; while (string[i] != 0) { print string[i]; i ++ }
start:
    mov al, [bx] ; bx is the base address of the string
    cmp al, 0 ; check if the character is null
    je done ; jump equals, if the above is true, jumps to the done label

    ; print the character
    mov ah, 0x0e ; tty mode
    int 0x10 ; al already contains the char

    ; increment the pointer and do the next loop
    add bx, 1
    jmp start

done: 
    popa ; pop the registers from the stack
    ret

print_nl:
    pusha

    mov ah, 0x0e ; tty mode
    mov al, 0x0a ; new line character
    int 0x10
    mov al, 0x0d ; carriage return
    int 0x10

    popa
    ret


; print hex

; we receive the data in the dx register
print_hex:
    pusha

    mov cx, 0 ; our index variable

; get the last char of dx and convert to ascii
; numeric ascii values 0 (ascii 0x30) to 9 (ascii 0x46) we'll add 0x40
; then move the ascii byte to the correct position on the resulting string
hex_loop:
    cmp cx, 4 ; loop 4 times
    je end

    ; convert last char of dx to ascii
    mov ax, dx ; ax will be our working register
    and ax, 0x000f ; mask the first 3 to zeros
    add al, 0x30 ; add 0x30 to convert it to ascii
    cmp al, 0x39 ; if > 9 add an extra 8 to represent A to F
    jle step2
    add al, 7 ; A is ascii 65 instead of 58 so 65-58 = 7

step2:
    mov bx, HEX_OUT + 5 ; base + length
    sub bx, cx ; our index var
    mov [bx], al ; copy the ascii char on al to the position pointed by bx
    ror dx, 4 ; rotate 4 times

    ; increment the index and loop
    add cx, 1
    jmp hex_loop

end:
    ; prepare the parameter and call print
    mov bx, HEX_OUT
    call print

    popa
    ret

HEX_OUT:
    db '0x0000', 0 ; reserve memory for our new string