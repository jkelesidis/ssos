; load dh sectors from drive dl into es:bx
disk_load:
    pusha
    ; reading from disk requires settings specific values in all registers
    ; so save dx as we'll overwrite it
    push dx

    mov ah, 0x02 ; ah <- int 0x13 function. 0x02 = read
    mov al, dh ; al <- number of sectors to read
    mov cl, 0x02 ; cl <- sector 2. the first sector is the boot sector
                 ; 0x01 is our boot sector, 0x02 is the first available sector
    mov ch, 0x00 ; ch <- cylinder 0
                 ; dl <- drive number. our caller sets it as a parameter and gets it from biod
                 ; 0 = floppy, 1 = floppy 2, 0x80 = hdd, 0x81 = hdd2
    mov dh, 0x00 ; dh <- head number

    ; [es:bx] <- pointer to buffer where data will be stored
    ; caller sets it up for us and it is the standard location for int 13h
    int 0x13
    jc disk_error ; if there's an error the carry bit will be set

    pop dx
    popa
    ret

disk_error:
    mov bx, DISK_ERROR
    call print
    call print_nl
    mov dh, ah ; ah = error code, dl = disk drive that dropped the error
    call print_hex
    jmp $

DISK_ERROR: db "Disk read error", 0
SECTORS_ERROR: db "Incorrect number of sectors read", 0