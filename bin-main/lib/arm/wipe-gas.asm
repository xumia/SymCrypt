SymCryptWipeAsm: .global SymCryptWipeAsm
.type SymCryptWipeAsm, %function
push {lr}
        mov r2, r1
        mov r1, #0
        bl memset
pop {pc}
