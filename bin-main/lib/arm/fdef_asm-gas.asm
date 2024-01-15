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
SymCryptFdefRawAddAsm: .global SymCryptFdefRawAddAsm
.type SymCryptFdefRawAddAsm, %function
push {r4,r5,r6,r7,r8,r9,lr}
    neg r3, r3
    mov r8, #0
    mov r9, #0
SymCryptFdefRawAddAsmLoop:
    rrxs r8, r8
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    adcs r4, r4, r5
    adcs r6, r6, r7
    stmia r2!, {r4, r6}
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    adcs r4, r4, r5
    adcs r6, r6, r7
    stmia r2!, {r4, r6}
    adc r8, r9, r9
    adds r3, r3, #1
    bne SymCryptFdefRawAddAsmLoop
    mov r0, r8
pop {r4,r5,r6,r7,r8,r9,pc}
SymCryptFdefRawSubAsm: .global SymCryptFdefRawSubAsm
.type SymCryptFdefRawSubAsm, %function
push {r4,r5,r6,r7,r8,r9,lr}
    neg r3, r3
    mov r8, #0
    mov r9, #0
SymCryptFdefRawSubAsmLoop:
    subs r8, r9, r8
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    sbcs r4, r4, r5
    sbcs r6, r6, r7
    stmia r2!, {r4, r6}
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    sbcs r4, r4, r5
    sbcs r6, r6, r7
    stmia r2!, {r4, r6}
    sbc r8, r9, r9
    adds r3, r3, #1
    bne SymCryptFdefRawSubAsmLoop
    and r0, r8, #1
pop {r4,r5,r6,r7,r8,r9,pc}
SymCryptFdefMaskedCopyAsm: .global SymCryptFdefMaskedCopyAsm
.type SymCryptFdefMaskedCopyAsm, %function
push {r4,r5,r6,r7,r8,r9,lr}
    neg r2, r2
    mov r9, #0
    subs r4, r9, r3
    sbc r3, r9, r9
    orr r9, r9, r3
    mvn r9, r9
    mov r8, r1
SymCryptFdefMaskedCopyAsmLoop:
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    and r4, r4, r3
    and r5, r5, r9
    orr r4, r4, r5
    and r6, r6, r3
    and r7, r7, r9
    orr r6, r6, r7
    stmia r8!, {r4, r6}
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    and r4, r4, r3
    and r5, r5, r9
    orr r4, r4, r5
    and r6, r6, r3
    and r7, r7, r9
    orr r6, r6, r7
    stmia r8!, {r4, r6}
    adds r2, r2, #1
    bne SymCryptFdefMaskedCopyAsmLoop
pop {r4,r5,r6,r7,r8,r9,pc}
SymCryptFdefRawMulAsm: .global SymCryptFdefRawMulAsm
.type SymCryptFdefRawMulAsm, %function
push {r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    sub sp, sp, #8
    lsl r1, r1, #2
    ldr r4, [sp, #(8+4*10)]
    neg r1, r1
    neg r3, r3
    mov r5, r4
    str r2, [sp, #0]
    str r3, [sp, #4]
    mov r11, #0
    mov r12, #0
    ldmia r0!, {r6, r7}
SymCryptFdefRawMulAsmLoopInner1:
    adds r3, r3, #1
    ldmia r2!, {r8, r9}
    mov r10, #0
    umaal r10, r11, r6, r8
    str r10, [r4], #4
    umaal r11, r12, r7, r8
    mov r10, #0
    umaal r10, r11, r6, r9
    str r10, [r4], #4
    umaal r11, r12, r7, r9
    ldmia r2!, {r8, r9}
    mov r10, #0
    umaal r10, r11, r6, r8
    str r10, [r4], #4
    umaal r11, r12, r7, r8
    mov r10, #0
    umaal r10, r11, r6, r9
    str r10, [r4], #4
    umaal r11, r12, r7, r9
    bne SymCryptFdefRawMulAsmLoopInner1
    stmia r4, {r11, r12}
    add r1, r1, #2
    add r5, r5, #8
SymCryptFdefRawMulAsmLoopOuter:
    ldr r3, [sp, #4]
    ldr r2, [sp, #0]
    mov r4, r5
    mov r11, #0
    mov r12, #0
    ldmia r0!, {r6, r7}
SymCryptFdefRawMulAsmLoopInner:
    adds r3, r3, #1
    ldmia r2!, {r8, r9}
    ldr r10, [r4]
    umaal r10, r11, r6, r8
    str r10, [r4], #4
    umaal r11, r12, r7, r8
    ldr r10, [r4]
    umaal r10, r11, r6, r9
    str r10, [r4], #4
    umaal r11, r12, r7, r9
    ldmia r2!, {r8, r9}
    ldr r10, [r4]
    umaal r10, r11, r6, r8
    str r10, [r4], #4
    umaal r11, r12, r7, r8
    ldr r10, [r4]
    umaal r10, r11, r6, r9
    str r10, [r4], #4
    umaal r11, r12, r7, r9
    bne SymCryptFdefRawMulAsmLoopInner
    adds r1, r1, #2
    add r5, r5, #8
    stmia r4, {r11, r12}
    bne SymCryptFdefRawMulAsmLoopOuter
    add sp, sp, #8
pop {r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}
.macro SQR_SINGLEADD_32 index
        mov r10, #0
        ldr r8, [r2, #4*\index]
        umaal r10, r11, r6, r8
        str r10, [r4, #4*\index]
.endm
.macro SQR_DOUBLEADD_32 index
        ldr r8, [r2, #4*\index]
        ldr r10, [r4, #4*\index]
        umaal r10, r11, r6, r8
        str r10, [r4, #4*\index]
.endm
.macro SQR_DIAGONAL_PROP index
        ldr r6, [r0, #4*\index]
        umull r10, r11, r6, r6
        ldr r8, [r4, #8*\index]
        ldr r9, [r4, #8*\index + 4]
        adcs r10, r10, r8
        adcs r11, r11, r9
        str r10, [r4, #8*\index]
        str r11, [r4, #8*\index + 4]
.endm
SymCryptFdefRawSquareAsm: .global SymCryptFdefRawSquareAsm
.type SymCryptFdefRawSquareAsm, %function
push {r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    sub sp, sp, #8
    mov r3, r1
    lsl r1, r1, #2
    neg r1, r1
    neg r3, r3
    mov r4, r2
    mov r5, r2
    str r0, [sp, #4]
    str r5, [sp, #0]
    mov r7, r3
    mov r12, r3
    mov r2, r0
    ands r11, r11, #0
    ldr r6, [r0]
    str r11, [r4]
    b SymCryptFdefRawSquareAsmInnerLoopInit_Word1
SymCryptFdefRawSquareAsmInnerLoopInit_Word0:
    SQR_SINGLEADD_32 0
SymCryptFdefRawSquareAsmInnerLoopInit_Word1:
    SQR_SINGLEADD_32 1
    SQR_SINGLEADD_32 2
    SQR_SINGLEADD_32 3
    add r2, r2, #16
    add r4, r4, #16
    adds r3, r3, #1
    bne SymCryptFdefRawSquareAsmInnerLoopInit_Word0
    str r11, [r4]
    add r1, r1, #2
    mov r9, #1
SymCryptFdefRawSquareAsmOuterLoop:
    add r5, r5, #4
    mov r3, r12
    mov r2, r0
    mov r4, r5
    ands r11, r11, #0
    ldr r6, [r0, r9, LSL #2]
    add r9, r9, #1
    cmp r9, #1
    beq SymCryptFdefRawSquareAsmInnerLoop_Word1
    cmp r9, #2
    beq SymCryptFdefRawSquareAsmInnerLoop_Word2
    cmp r9, #3
    beq SymCryptFdefRawSquareAsmInnerLoop_Word3
    mov r9, #0
    add r0, r0, #16
    add r5, r5, #16
    mov r2, r0
    mov r4, r5
    adds r3, r3, #1
    mov r12, r3
SymCryptFdefRawSquareAsmInnerLoop_Word0:
    SQR_DOUBLEADD_32 0
SymCryptFdefRawSquareAsmInnerLoop_Word1:
    SQR_DOUBLEADD_32 1
SymCryptFdefRawSquareAsmInnerLoop_Word2:
    SQR_DOUBLEADD_32 2
SymCryptFdefRawSquareAsmInnerLoop_Word3:
    SQR_DOUBLEADD_32 3
    add r2, r2, #16
    add r4, r4, #16
    adds r3, r3, #1
    bne SymCryptFdefRawSquareAsmInnerLoop_Word0
    str r11, [r4]
    adds r1, r1, #1
    bne SymCryptFdefRawSquareAsmOuterLoop
    eor r11, r11, r11
    str r11, [r5, #20]
    mov r3, r7
    lsl r3, r3, #1
    ldr r4, [sp, #0]
    ands r1, r1, #0
    ands r2, r2, #0
SymCryptFdefRawSquareAsmSecondPass:
    rrxs r2, r2
    ldmia r4, {r8, r9}
    adcs r8, r8, r8
    adcs r9, r9, r9
    stmia r4!, {r8, r9}
    ldmia r4, {r10, r11}
    adcs r10, r10, r10
    adcs r11, r11, r11
    stmia r4!, {r10, r11}
    adc r2, r1, r1
    adds r3, r3, #1
    bne SymCryptFdefRawSquareAsmSecondPass
    ldr r0, [sp, #4]
    ldr r4, [sp, #0]
    ands r2, r2, #0
SymCryptFdefRawSquareAsmThirdPass:
    rrxs r2, r2
    SQR_DIAGONAL_PROP 0
    SQR_DIAGONAL_PROP 1
    SQR_DIAGONAL_PROP 2
    SQR_DIAGONAL_PROP 3
    adc r2, r1, r1
    add r0, r0, #16
    add r4, r4, #32
    adds r7, r7, #1
    bne SymCryptFdefRawSquareAsmThirdPass
    add sp, sp, #8
pop {r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}
SymCryptFdefMontgomeryReduceAsm: .global SymCryptFdefMontgomeryReduceAsm
.type SymCryptFdefMontgomeryReduceAsm, %function
push {r4,r5,r6,r7,r8,r9,r10,r11,r12,lr}
    sub sp, sp, #16
    str r2, [sp, #4]
    ldr r3, [r0, #SymCryptModulusNdigitsOffsetArm]
    ldr r5, [r0, #SymCryptModulusInv64OffsetArm]
    add r0, r0, #SymCryptModulusValueOffsetArm
    str r5, [sp, #12]
    lsl r4, r3, #2
    neg r3, r3
    neg r4, r4
    str r0, [sp, #0]
    mov r2, r1
    str r3, [sp, #8]
    eor r7, r7, r7
SymCryptFdefMontgomeryReduceAsmOuter:
    ldr r3, [sp, #12]
    ldmia r1, {r10, r12}
    ldmia r0, {r8,r9}
    mov r11, #0
    mul r5, r10, r3
    umaal r10, r11, r5, r8
    umaal r12, r11, r5, r9
    mul r6, r12, r3
    ldr r3, [sp, #8]
    mov r11, #0
    mov r12, #0
SymCryptFdefMontgomeryReduceAsmInner:
    adds r3, r3, #1
    ldmia r0!, {r8, r9}
    ldr r10, [r1]
    umaal r10, r11, r5, r8
    str r10, [r1], #4
    umaal r11, r12, r6, r8
    ldr r10, [r1]
    umaal r10, r11, r5, r9
    str r10, [r1], #4
    umaal r11, r12, r6, r9
    ldmia r0!, {r8, r9}
    ldr r10, [r1]
    umaal r10, r11, r5, r8
    str r10, [r1], #4
    umaal r11, r12, r6, r8
    ldr r10, [r1]
    umaal r10, r11, r5, r9
    str r10, [r1], #4
    umaal r11, r12, r6, r9
    bne SymCryptFdefMontgomeryReduceAsmInner
    mov r8, #0
    mov r9, #0
    ldmia r1, {r5, r6}
    adds r11, r11, r5
    adc r8, r8, #0
    adds r11, r11, r7
    adc r8, r8, #0
    str r11, [r1], #4
    adds r12, r12, r6
    adc r9, r9, #0
    adds r12, r12, r8
    adc r7, r9, #0
    str r12, [r1]
    adds r4, r4, #2
    add r2, r2, #8
    ldr r0, [sp, #0]
    mov r1, r2
    bne SymCryptFdefMontgomeryReduceAsmOuter
    mov r0, r2
    mov r11, r2
    ldr r1, [sp, #0]
    ldr r2, [sp, #4]
    ldr r3, [sp, #8]
    mov r10, r7
    mov r8, #0
    mov r9, #0
SymCryptFdefMontgomeryReduceRawSubAsmLoop:
    subs r8, r9, r8
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    sbcs r4, r4, r5
    sbcs r6, r6, r7
    stmia r2!, {r4, r6}
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    sbcs r4, r4, r5
    sbcs r6, r6, r7
    stmia r2!, {r4, r6}
    sbc r8, r9, r9
    adds r3, r3, #1
    bne SymCryptFdefMontgomeryReduceRawSubAsmLoop
    mov r0, r11
    ldr r1, [sp, #4]
    and r9, r8, #1
    orr r11, r10, r9
    ldr r2, [sp, #8]
    mov r9, #0
    subs r4, r10, r11
    sbc r3, r9, r9
    orr r9, r9, r3
    mvn r9, r9
    mov r8, r1
SymCryptFdefMontgomeryReduceMaskedCopyAsmLoop:
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    and r4, r4, r3
    and r5, r5, r9
    orr r4, r4, r5
    and r6, r6, r3
    and r7, r7, r9
    orr r6, r6, r7
    stmia r8!, {r4, r6}
    ldmia r0!, {r4, r6}
    ldmia r1!, {r5, r7}
    and r4, r4, r3
    and r5, r5, r9
    orr r4, r4, r5
    and r6, r6, r3
    and r7, r7, r9
    orr r6, r6, r7
    stmia r8!, {r4, r6}
    adds r2, r2, #1
    bne SymCryptFdefMontgomeryReduceMaskedCopyAsmLoop
    add sp, sp, #16
pop {r4,r5,r6,r7,r8,r9,r10,r11,r12,pc}
