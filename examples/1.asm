start:
    setint 33, print_handler ; register handler for interrupt 33 (print_handler)
    movq r1, 3               ; set r1 to 3 (for counter)

loop_top:
    jz r1, exit ; if r1 counter hits zero exit loop
    int 33 ; fire interrupt 33 to print "hello world"

resume:
    sub r1, 1, r1 ; decrement r1 counter by 1
    jmp loop_top  ; jump back to check counter and loop

exit:
    stop 0

print_handler:
    movq [0x100], 0x6f77206f6c6c6568 ; pack "hello wo" into memory
    movl [0x108], 0x0a646c72         ; write "rld\n" into memory
    bufout 1, 0x100, 12              ; write 12 bytes from address 0x100 to port 1

    jmp resume ; jump to resume label
