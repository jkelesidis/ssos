global _start:
    [bits 32]

_start:
    [extern kernel_main] ; define the calling point. Must have same name as kernel.c main function
    call kernel_main ; calls the c function. The linker will know where it is placed in memory
    jmp $