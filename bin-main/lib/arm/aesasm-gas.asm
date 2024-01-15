.set SymCryptModulusNdigitsOffsetAmd64, 4;
.set SymCryptModulusInv64OffsetAmd64, 24;
.set SymCryptModulusValueOffsetAmd64, 128;
.set SymCryptNegDivisorSingleDigitOffsetAmd64, 256;
.set SymCryptModulusNdigitsOffsetX86, 4;
.set SymCryptModulusInv64OffsetX86, 24;
.set SymCryptModulusValueOffsetX86, 96;
.set SymCryptModulusNdigitsOffsetArm64, 4;
.set SymCryptModulusInv64OffsetArm64, 24;
.set SymCryptModulusValueOffsetArm64, 128;
.set SymCryptModulusNdigitsOffsetArm, 4;
.set SymCryptModulusInv64OffsetArm, 24;
.set SymCryptModulusValueOffsetArm, 96;
.macro SYMCRYPT_CHECK_MAGIC temp1, temp2, ptr, offset, check_magic_label
.endm
.macro ENC_MIX keyptr
        uxtb r4, r0
        uxtb r7, r0, ror #8
        uxtb r6, r0, ror #16
        uxtb r5, r0, ror #24
        ldr r4, [r12, r4, lsl #2]
        ldr r7, [r12, r7, lsl #2]
        ldr r6, [r12, r6, lsl #2]
        ldr r5, [r12, r5, lsl #2]
        uxtb r0, r1
        uxtb lr, r1, ror #8
        ldr r0, [r12, r0, lsl #2]
        ldr lr, [r12, lr, lsl #2]
        eor r5, r5, r0, ror #24
        eor r4, r4, lr, ror #24
        uxtb r0, r1, ror #16
        uxtb r1, r1, ror #24
        ldr r0, [r12, r0, lsl #2]
        ldr r1, [r12, r1, lsl #2]
        eor r7, r7, r0, ror #24
        eor r6, r6, r1, ror #24
        uxtb r0, r2
        uxtb r1, r2, ror #8
        uxtb lr, r2, ror #16
        uxtb r2, r2, ror #24
        ldr r0, [r12, r0, lsl #2]
        ldr r1, [r12, r1, lsl #2]
        ldr lr, [r12, lr, lsl #2]
        ldr r2, [r12, r2, lsl #2]
        eor r6, r6, r0, ror #16
        eor r5, r5, r1, ror #16
        eor r4, r4, lr, ror #16
        eor r7, r7, r2, ror #16
        uxtb r0, r3
        uxtb r1, r3, ror #8
        uxtb r2, r3, ror #16
        uxtb r3, r3, ror #24
        ldr r0, [r12, r0, lsl #2]
        ldr r1, [r12, r1, lsl #2]
        ldr r2, [r12, r2, lsl #2]
        ldr r3, [r12, r3, lsl #2]
        eor r7, r7, r0, ror #8
        eor r6, r6, r1, ror #8
        eor r5, r5, r2, ror #8
        eor r4, r4, r3, ror #8
        ldrd r0, r1, [\keyptr, #0]
        ldrd r2, r3, [\keyptr, #8]
        adds \keyptr, \keyptr, #16
        eors r0, r0, r4
        eor r1, r1, r5, ror #8
        eor r2, r2, r6, ror #16
        eor r3, r3, r7, ror #24
.endm
.macro DEC_MIX keyptr
        uxtb r4, r0
        uxtb r5, r0, ror #8
        uxtb r6, r0, ror #16
        uxtb r7, r0, ror #24
        ldr r4, [r12, r4, lsl #2]
        ldr r5, [r12, r5, lsl #2]
        ldr r6, [r12, r6, lsl #2]
        ldr r7, [r12, r7, lsl #2]
        uxtb r0, r1
        uxtb lr, r1, ror #8
        ldr r0, [r12, r0, lsl #2]
        ldr lr, [r12, lr, lsl #2]
        eor r5, r5, r0, ror #8
        eor r6, r6, lr, ror #8
        uxtb r0, r1, ror #16
        uxtb r1, r1, ror #24
        ldr r0, [r12, r0, lsl #2]
        ldr r1, [r12, r1, lsl #2]
        eor r7, r7, r0, ror #8
        eor r4, r4, r1, ror #8
        uxtb r0, r2
        uxtb r1, r2, ror #8
        uxtb lr, r2, ror #16
        uxtb r2, r2, ror #24
        ldr r0, [r12, r0, lsl #2]
        ldr r1, [r12, r1, lsl #2]
        ldr lr, [r12, lr, lsl #2]
        ldr r2, [r12, r2, lsl #2]
        eor r6, r6, r0, ror #16
        eor r7, r7, r1, ror #16
        eor r4, r4, lr, ror #16
        eor r5, r5, r2, ror #16
        uxtb r0, r3
        uxtb r1, r3, ror #8
        uxtb r2, r3, ror #16
        uxtb r3, r3, ror #24
        ldr r0, [r12, r0, lsl #2]
        ldr r1, [r12, r1, lsl #2]
        ldr r2, [r12, r2, lsl #2]
        ldr r3, [r12, r3, lsl #2]
        eor r7, r7, r0, ror #24
        eor r4, r4, r1, ror #24
        eor r5, r5, r2, ror #24
        eor r6, r6, r3, ror #24
        ldrd r0, r1, [\keyptr, #0]
        ldrd r2, r3, [\keyptr, #8]
        adds \keyptr, \keyptr, #16
        eors r0, r0, r4
        eor r1, r1, r5, ror #24
        eor r2, r2, r6, ror #16
        eor r3, r3, r7, ror #8
.endm
.macro AES_ENCRYPT loopLabel
        ldrd r4, r5, [r8, #0]
        ldrd r6, r7, [r8, #8]
        eors r0, r0, r4
        eors r1, r1, r5
        eors r2, r2, r6
        eors r3, r3, r7
        add r8, r8, #16
\loopLabel:
        ENC_MIX r8
        cmp r8, r9
        blo \loopLabel
        add r12, r12, #1
        uxtb r4, r0
        uxtb r7, r0, ror #8
        uxtb r6, r0, ror #16
        uxtb r5, r0, ror #24
        ldrb r4, [r12, r4, lsl #2]
        ldrb r7, [r12, r7, lsl #2]
        ldrb r6, [r12, r6, lsl #2]
        ldrb r5, [r12, r5, lsl #2]
        uxtb r0, r1
        uxtb lr, r1, ror #8
        ldrb r0, [r12, r0, lsl #2]
        ldrb lr, [r12, lr, lsl #2]
        orr r5, r5, r0, lsl #8
        orr r4, r4, lr, lsl #8
        uxtb r0, r1, ror #16
        uxtb r1, r1, ror #24
        ldrb r0, [r12, r0, lsl #2]
        ldrb r1, [r12, r1, lsl #2]
        orr r7, r7, r0, lsl #8
        orr r6, r6, r1, lsl #8
        uxtb r0, r2
        uxtb r1, r2, ror #8
        uxtb lr, r2, ror #16
        uxtb r2, r2, ror #24
        ldrb r0, [r12, r0, lsl #2]
        ldrb r1, [r12, r1, lsl #2]
        ldrb lr, [r12, lr, lsl #2]
        ldrb r2, [r12, r2, lsl #2]
        orr r6, r6, r0, lsl #16
        orr r5, r5, r1, lsl #16
        orr r4, r4, lr, lsl #16
        orr r7, r7, r2, lsl #16
        uxtb r0, r3
        uxtb r1, r3, ror #8
        uxtb r2, r3, ror #16
        uxtb r3, r3, ror #24
        ldrb r0, [r12, r0, lsl #2]
        ldrb r1, [r12, r1, lsl #2]
        ldrb r2, [r12, r2, lsl #2]
        ldrb r3, [r12, r3, lsl #2]
        orr r7, r7, r0, lsl #24
        orr r6, r6, r1, lsl #24
        orr r5, r5, r2, lsl #24
        orr r4, r4, r3, lsl #24
        sub r12, r12, #1
        ldrd r0, r1, [r9, #0]
        ldrd r2, r3, [r9, #8]
        eors r4, r4, r0
        eor r5, r1, r5, ror #8
        eor r6, r2, r6, ror #16
        eor r7, r3, r7, ror #24
.endm
.macro AES_DECRYPT loopLabel
        ldrd r4, r5, [r8, #0]
        ldrd r6, r7, [r8, #8]
        eors r0, r0, r4
        eors r1, r1, r5
        eors r2, r2, r6
        eors r3, r3, r7
        add r8, r8, #16
\loopLabel:
        DEC_MIX r8
        cmp r8, r9
        blo \loopLabel
        uxtb r4, r0
        uxtb r5, r0, ror #8
        uxtb r6, r0, ror #16
        uxtb r7, r0, ror #24
        ldrb r4, [r10, r4]
        ldrb r5, [r10, r5]
        ldrb r6, [r10, r6]
        ldrb r7, [r10, r7]
        uxtb r0, r1
        uxtb lr, r1, ror #8
        ldrb r0, [r10, r0]
        ldrb lr, [r10, lr]
        orr r5, r5, r0, lsl #24
        orr r6, r6, lr, lsl #24
        uxtb r0, r1, ror #16
        uxtb r1, r1, ror #24
        ldrb r0, [r10, r0]
        ldrb r1, [r10, r1]
        orr r7, r7, r0, lsl #24
        orr r4, r4, r1, lsl #24
        uxtb r0, r2
        uxtb r1, r2, ror #8
        uxtb lr, r2, ror #16
        uxtb r2, r2, ror #24
        ldrb r0, [r10, r0]
        ldrb r1, [r10, r1]
        ldrb lr, [r10, lr]
        ldrb r2, [r10, r2]
        orrs r6, r6, r0, lsl #16
        orr r7, r7, r1, lsl #16
        orr r4, r4, lr, lsl #16
        orr r5, r5, r2, lsl #16
        uxtb r0, r3
        uxtb r1, r3, ror #8
        uxtb r2, r3, ror #16
        uxtb r3, r3, ror #24
        ldrb r0, [r10, r0]
        ldrb r1, [r10, r1]
        ldrb r2, [r10, r2]
        ldrb r3, [r10, r3]
        orr r7, r7, r0, lsl #8
        orr r4, r4, r1, lsl #8
        orr r5, r5, r2, lsl #8
        orr r6, r6, r3, lsl #8
        ldrd r0, r1, [r9, #0]
        ldrd r2, r3, [r9, #8]
        eors r4, r4, r0
        eor r5, r1, r5, ror #24
        eor r6, r2, r6, ror #16
        eor r7, r3, r7, ror #8
.endm
SymCryptAesEncryptAsm: .global SymCryptAesEncryptAsm
.type SymCryptAesEncryptAsm, %function
push {r4,r5,r6,r7,r8,r9,r10,r11,lr}
        push {r2}
        SYMCRYPT_CHECK_MAGIC r4, r5, r0, SYMCRYPT_AES_EXPANDED_KEY_magic, SymCryptAesEncryptAsm_check_magic_label
        ldr r9, [r0, #(29*4*4)]
        mov r8, r0
        ldr r12, =SymCryptAesSboxMatrixMult
        ldr r0, [r1, #0]
        ldr r2, [r1, #8]
        ldr r3, [r1, #12]
        ldr r1, [r1, #4]
        AES_ENCRYPT SymCryptAesEncryptAsm_loopLabel
        ldr r0, [sp]
        str r4, [r0, #0]
        str r5, [r0, #4]
        str r6, [r0, #8]
        str r7, [r0, #12]
        pop {r2}
pop {r4,r5,r6,r7,r8,r9,r10,r11,pc}
SymCryptAesDecryptAsm: .global SymCryptAesDecryptAsm
.type SymCryptAesDecryptAsm, %function
push {r4,r5,r6,r7,r8,r9,r10,r11,lr}
        push {r2}
        SYMCRYPT_CHECK_MAGIC r4, r5, r0, SYMCRYPT_AES_EXPANDED_KEY_magic, SymCryptAesDecryptAsm_check_magic_label
        ldr r8, [r0, #(29*4*4)]
        ldr r9, [r0, #(29*4*4+4)]
        ldr r10, =SymCryptAesInvSbox
        ldr r12, =SymCryptAesInvSboxMatrixMult
        ldr r0, [r1, #0]
        ldr r2, [r1, #8]
        ldr r3, [r1, #12]
        ldr r1, [r1, #4]
        AES_DECRYPT SymCryptAesDecryptAsm_loopLabel
        ldr r0, [sp]
        str r4, [r0, #0]
        str r5, [r0, #4]
        str r6, [r0, #8]
        str r7, [r0, #12]
        pop {r2}
pop {r4,r5,r6,r7,r8,r9,r10,r11,pc}
SymCryptAesCbcEncryptAsm: .global SymCryptAesCbcEncryptAsm
.type SymCryptAesCbcEncryptAsm, %function
push {r4,r5,r6,r7,r8,r9,r10,r11,lr}
        push {r0-r2}
        sub sp, sp, #16
        SYMCRYPT_CHECK_MAGIC r4, r5, r0, SYMCRYPT_AES_EXPANDED_KEY_magic, SymCryptAesCbcEncryptAsm_check_magic_label
        pld [r2]
        ldr r4, [sp, #64]
        mov r8, r2
        ldr r9, [r0, #(29*4*4)]
        mov r10, r3
        ldr r12, =SymCryptAesSboxMatrixMult
        bics r4, r4, #15
        beq SymCryptAesCbcEncryptNoData
        adds r4, r4, r2
        str r2, [sp]
        str r4, [sp, #4]
        pld [r8, #32]
        ldr r4, [r1, #0]
        ldr r5, [r1, #4]
        ldr r6, [r1, #8]
        ldr r7, [r1, #12]
SymCryptAesCbcEncryptAsmLoop:
        ldr r0, [r8]
        ldr r1, [r8, #4]
        ldr r2, [r8, #8]
        ldr r3, [r8, #12]
        pld [r8, #64]
        add r8, r8, #16
        str r8, [sp]
        eors r0, r0, r4
        eors r1, r1, r5
        eors r2, r2, r6
        eors r3, r3, r7
        ldr r8, [sp, #16]
        AES_ENCRYPT SymCryptAesCbcEncryptAsm_loopLabel
        ldr r8, [sp]
        ldr r0, [sp, #4]
        str r4, [r10, #0]
        str r5, [r10, #4]
        str r6, [r10, #8]
        str r7, [r10, #12]
        add r10, r10, #16
        cmp r8, r0
        blo SymCryptAesCbcEncryptAsmLoop
        ldr r0, [sp, #20]
        str r4, [r0, #0]
        str r5, [r0, #4]
        str r6, [r0, #8]
        str r7, [r0, #12]
SymCryptAesCbcEncryptNoData:
        add sp, sp, #16
        pop {r0-r2}
pop {r4,r5,r6,r7,r8,r9,r10,r11,pc}
SymCryptAesCbcDecryptAsm: .global SymCryptAesCbcDecryptAsm
.type SymCryptAesCbcDecryptAsm, %function
push {r4,r5,r6,r7,r8,r9,r10,r11,lr}
        push {r0-r2}
        sub sp, sp, #32
        SYMCRYPT_CHECK_MAGIC r4, r5, r0, SYMCRYPT_AES_EXPANDED_KEY_magic, SymCryptAesCbcDecryptAsm_check_magic_label
        ldr r4, [sp, #80]
        bics r4, r4, #15
        beq SymCryptAesCbcDecryptNoData
        ldr r9, [r0, #(29*4*4+4)]
        ldr r8, [r0, #(29*4*4)]
        subs r4, r4, #16
        adds r3, r3, r4
        adds r2, r2, r4
        pld [r2]
        str r3, [sp, #4]
        str r2, [sp, #0]
        str r8, [sp, #8]
        ldr r10, =SymCryptAesInvSbox
        ldr r12, =SymCryptAesInvSboxMatrixMult
        pld [r2, #-32]
        ldr r0, [r2, #0]
        ldr r1, [r2, #4]
        ldr r3, [r2, #12]
        ldr r2, [r2, #8]
        strd r0, r1, [sp, #16]
        strd r2, r3, [sp, #24]
        b SymCryptAesCbcDecryptAsmLoopEntry
SymCryptAesCbcDecryptAsmLoop:
        ldr r0, [r8, #-16]
        ldr r1, [r8, #-12]
        ldr r2, [r8, #-8]
        ldr r3, [r8, #-4]
        pld [r8, #-64]
        eors r4, r4, r0
        eors r5, r5, r1
        eors r6, r6, r2
        eors r7, r7, r3
        str r4, [lr, #0]
        str r5, [lr, #4]
        str r6, [lr, #8]
        str r7, [lr, #12]
        sub lr, lr, #16
        sub r8, r8, #16
        str r8, [sp]
        str lr, [sp, #4]
SymCryptAesCbcDecryptAsmLoopEntry:
        ldr r8, [sp, #8]
        AES_DECRYPT SymCryptAesCbcDecryptAsm_loopLabel
        ldr r8, [sp, #0]
        ldr lr, [sp, #4]
        ldr r0, [sp, #40]
        cmp r8, r0
        bhi SymCryptAesCbcDecryptAsmLoop
        ldr r8, [sp, #36]
        ldr r0, [r8, #0]
        ldr r1, [r8, #4]
        ldr r2, [r8, #8]
        ldr r3, [r8, #12]
        eors r4, r4, r0
        eors r5, r5, r1
        eors r6, r6, r2
        eors r7, r7, r3
        str r4, [lr, #0]
        str r5, [lr, #4]
        str r6, [lr, #8]
        str r7, [lr, #12]
        ldrd r0, r1, [sp, #16]
        ldrd r2, r3, [sp, #24]
        str r0, [r8, #0]
        str r1, [r8, #4]
        str r2, [r8, #8]
        str r3, [r8, #12]
SymCryptAesCbcDecryptNoData:
        add sp, sp, #32
        pop {r0-r2}
pop {r4,r5,r6,r7,r8,r9,r10,r11,pc}
SymCryptAesCtrMsb64Asm: .global SymCryptAesCtrMsb64Asm
.type SymCryptAesCtrMsb64Asm, %function
push {r4,r5,r6,r7,r8,r9,r10,r11,lr}
        push {r0-r2}
        sub sp, sp, #32
        SYMCRYPT_CHECK_MAGIC r4, r5, r0, SYMCRYPT_AES_EXPANDED_KEY_magic, SymCryptAesCtrMsb64Asm_check_magic_label
        pld [r2]
        ldr r4, [sp, #80]
        ldr r9, [r0, #(29*4*4)]
        mov r10, r2
        ldr r12, =SymCryptAesSboxMatrixMult
        bics r4, r4, #15
        beq SymCryptAesCtrMsb64NoData
        adds r4, r4, r2
        str r3, [sp]
        str r4, [sp, #4]
        pld [r10, #32]
        mov r3, r1
        ldr r0, [r3, #0]
        ldr r1, [r3, #4]
        ldr r2, [r3, #8]
        ldr r3, [r3, #12]
        strd r0, r1, [sp, #16]
        strd r2, r3, [sp, #24]
SymCryptAesCtrMsb64AsmLoop:
        ldr r8, [sp, #32]
        AES_ENCRYPT loopLabel
        ldr r0, [r10, #0]
        ldr r1, [r10, #4]
        ldr r2, [r10, #8]
        ldr r3, [r10, #12]
        pld [r10, #64]
        ldr r8, [sp]
        ldr lr, [sp, #4]
        eors r0, r0, r4
        eors r1, r1, r5
        eors r2, r2, r6
        eors r3, r3, r7
        str r0, [r8, #0]
        str r1, [r8, #4]
        str r2, [r8, #8]
        str r3, [r8, #12]
        ldrd r0, r1, [sp, #16]
        ldrd r2, r3, [sp, #24]
        add r8, r8, #16
        add r10, r10, #16
        str r8, [sp]
        rev r3, r3
        rev r2, r2
        adds r3, r3, #1
        adcs r2, r2, #0
        rev r3, r3
        rev r2, r2
        strd r2, r3, [sp, #24]
        cmp r10, lr
        blo SymCryptAesCtrMsb64AsmLoop
        ldr r0, [sp, #36]
        movs r1, #0
        str r2, [r0, #8]
        str r3, [r0, #12]
        str r1, [sp, #16]
        str r1, [sp, #20]
        str r1, [sp, #24]
        str r1, [sp, #28]
SymCryptAesCtrMsb64NoData:
        add sp, sp, #32
        pop {r0-r2}
pop {r4,r5,r6,r7,r8,r9,r10,r11,pc}
