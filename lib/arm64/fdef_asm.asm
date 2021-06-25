;
;  fdef_asm.asm   Assembler code for large integer arithmetic in the default data format for the arm64 architecture
;
; Copyright (c) Microsoft Corporation. Licensed under the MIT license.
;

#include "ksarm64.h"

; As Arm assembler already uses C preprocessor, we can just hardcode this asm to include constants
; MASM for now. To be fixed properly when converting arm64 asm to symcryptasm.
#define SYMCRYPT_MASM
#include "C_asm_shared.inc"
#undef SYMCRYPT_MASM

#include "symcrypt_version.inc"
#include "symcrypt_name_mangling.inc"
#include "symcrypt_magic.inc"

; A digit consists of 4 words of 64 bits each

;UINT32
;SYMCRYPT_CALL
; SymCryptFdefRawAdd(
;   _In_reads_bytes_(nDigits * SYMCRYPT_FDEF_DIGIT_SIZE )   PCUINT32    pSrc1,
;   _In_reads_bytes_(nDigits * SYMCRYPT_FDEF_DIGIT_SIZE )   PCUINT32    pSrc2,
;   _Out_writes_bytes_(nDigits * SYMCRYPT_FDEF_DIGIT_SIZE ) PUINT32     pDst,
;                                                           UINT32      nDigits );
;
; Initial inputs to registers:
;       pSrc1       -> x0
;       pSrc2       -> x1
;       pDst        -> x2
;       nDigits     -> x3

    LEAF_ENTRY ARM64EC_NAME_MANGLE(SymCryptFdefRawAddAsm)

    neg     x3, x3                  ; negate the digit count
    ands    x4, x4, x4              ; Zero the carry flag

SymCryptFdefRawAddAsmLoop
    add     x3, x3, #1              ; Increment the digit count by one
    ; carry is in the carry flag

    ldp     x4, x6, [x0], #16       ; Load two words of pSrc1
    ldp     x5, x7, [x1], #16       ; Load two words of pSrc2
    adcs    x4, x4, x5
    adcs    x6, x6, x7
    stp     x4, x6, [x2], #16       ; Store the result in the destination

    ldp     x4, x6, [x0], #16       ; Load two words of pSrc1
    ldp     x5, x7, [x1], #16       ; Load two words of pSrc2
    adcs    x4, x4, x5
    adcs    x6, x6, x7
    stp     x4, x6, [x2], #16       ; Store the result in the destination

    cbnz    x3, SymCryptFdefRawAddAsmLoop

    csetcs  x0                      ; Set the return value equal to the carry

    ret

    LEAF_END ARM64EC_NAME_MANGLE(SymCryptFdefRawAddAsm)

;UINT32
;SYMCRYPT_CALL
;SymCryptFdefRawSub(
;    _In_reads_bytes_(nDigits * SYMCRYPT_FDEF_DIGIT_SIZE )   PCUINT32    Src1,
;    _In_reads_bytes_(nDigits * SYMCRYPT_FDEF_DIGIT_SIZE )   PCUINT32    Src2,
;    _Out_writes_bytes_(nDigits * SYMCRYPT_FDEF_DIGIT_SIZE ) PUINT32     Dst,
;                                                            UINT32      nDigits )
;
; Initial inputs to registers:
;       pSrc1       -> x0
;       pSrc2       -> x1
;       pDst        -> x2
;       nDigits     -> x3

    LEAF_ENTRY ARM64EC_NAME_MANGLE(SymCryptFdefRawSubAsm)

    neg     x3, x3                  ; negate the digit count
    subs    x4, x4, x4              ; Set the carry flag (i.e. no borrow)

SymCryptFdefRawSubAsmLoop
    add     x3, x3, #1              ; Increment the digit count by one
    ; borrow is in the carry flag (flipped)

    ldp     x4, x6, [x0], #16       ; Load two words of pSrc1
    ldp     x5, x7, [x1], #16       ; Load two words of pSrc2
    sbcs    x4, x4, x5
    sbcs    x6, x6, x7
    stp     x4, x6, [x2], #16       ; Store the result in the destination

    ldp     x4, x6, [x0], #16       ; Load two words of pSrc1
    ldp     x5, x7, [x1], #16       ; Load two words of pSrc2
    sbcs    x4, x4, x5
    sbcs    x6, x6, x7
    stp     x4, x6, [x2], #16       ; Store the result in the destination

    cbnz    x3, SymCryptFdefRawSubAsmLoop

    csetcc  x0                      ; If the carry is clear (borrow), set the return value to 1

    ret

    LEAF_END ARM64EC_NAME_MANGLE(SymCryptFdefRawSubAsm)

;VOID
;SYMCRYPT_CALL
;SymCryptFdefMaskedCopy(
;    _In_reads_bytes_( nDigits * SYMCRYPT_FDEF_DIGIT_SIZE )      PCBYTE      pbSrc,
;    _InOut_writes_bytes_( nDigits * SYMCRYPT_FDEF_DIGIT_SIZE )  PBYTE       pbDst,
;                                                                UINT32      nDigits,
;                                                                UINT32      mask )

    LEAF_ENTRY ARM64EC_NAME_MANGLE(SymCryptFdefMaskedCopyAsm)

    neg     x2, x2                  ; negate the digit count
    subs    x4, XZR, x3             ; If (x3 > 0) clear the carry flag (i.e. borrow)

SymCryptFdefMaskedCopyAsmLoop
    add     x2, x2, #1              ; Increment the digit count by one

    ldp     x4, x6, [x0], #16       ; Load two words of the source
    ldp     x5, x7, [x1]            ; Load two words of the destination
    cselcc  x4, x4, x5              ; If the carry is clear, select the source operands
    cselcc  x6, x6, x7
    stp     x4, x6, [x1], #16       ; Store the two words in the destination

    ldp     x4, x6, [x0], #16
    ldp     x5, x7, [x1]
    cselcc  x4, x4, x5
    cselcc  x6, x6, x7
    stp     x4, x6, [x1], #16

    cbnz    x2, SymCryptFdefMaskedCopyAsmLoop

    ; Done, no return value

    ret

    LEAF_END ARM64EC_NAME_MANGLE(SymCryptFdefMaskedCopyAsm)

;VOID
;SYMCRYPT_CALL
;SymCryptFdefRawMul(
;    _In_reads_(nWords1)             PCUINT32    pSrc1,
;                                    UINT32      nDigits1,
;    _In_reads_(nWords2)             PCUINT32    pSrc2,
;                                    UINT32      nDigits2,
;    _Out_writes_(nWords1 + nWords2) PUINT32     pDst )
;
; Initial inputs to registers:
;       pSrc1       -> x0
;       nDigits1    -> x1
;       pSrc2       -> x2
;       nDigits2    -> x3
;       pDst        -> x4
;
; Basic structure:
;   for each word in Src1:
;       Dst += Src2 * word
;
; Register assignments
;       x0  = pSrc1 (moving forward one word every outer loop)
;       x1  = negated word count of pSrc1
;       x2  = pSrc2 (moving forward one *digit* every inner loop)
;       x3  = negated digit count of pSrc2 and pDst
;       x4  = pDst (moving forward one *digit* every inner loop)
;       x5  = Stored pDst (moving forward one word every outer loop)
;       x6  = Current word loaded from pSrc1
;       x8, x9   = Current words loaded in pairs from pSrc2
;       x10, x11 = Current words loaded in pairs from pDst
;       x12, x15 = "128-bit" sliding register to hold the result of multiplies
;       x16 = Stored pSrc2
;       x17 = Stored negated digit count of pSrc2
; Note x13, x14 are reserved in ARM64EC and thus are not used


    LEAF_ENTRY ARM64EC_NAME_MANGLE(SymCryptFdefRawMulAsm)

    lsl     x1, x1, #2                  ; Calculate word count

    neg     x1, x1                      ; negate nWords1
    neg     x3, x3                      ; negate nDigits2

    mov     x5, x4                      ; store pDst
    mov     x16, x2                     ; store pSrc2
    mov     x17, x3                     ; store -nDigits2 for later

    ;
    ; First iteration of main loop (no adding of previous values from pDst)
    ;
    ands    x15, x15, XZR               ; Clearing the carry flag and setting x15 = 0
    ldr     x6, [x0]                    ; load the first word from pSrc1

SymCryptFdefRawMulAsmLoopInner1
    add     x3, x3, #1                  ; move one digit up

    ldp     x8, x9, [x2]                ; load 2 words from pSrc2

    mul     x12, x6, x8                 ; Bits <63:0> of pSrc1[0]*pSrc2[j]
    adcs    x12, x12, x15               ; Adding the previous word (if there was a carry from the last addition it is added)
    umulh   x15, x6, x8                 ; Bits <127:64> of pSrc1[0]*pSrc2[j]
    str     x12, [x4]                   ; Store to destination

    mul     x12, x6, x9                 ; Bits <63:0> of pSrc1[0]*pSrc2[j+1]
    adcs    x12, x12, x15               ; Adding the previous word (if there was a carry from the last addition it is added)
    umulh   x15, x6, x9                 ; Bits <127:64> of pSrc1[0]*pSrc2[j+1]
    str     x12, [x4, #8]               ; Store to destination

    ldp     x8, x9, [x2, #16]           ; load 2 words from pSrc2

    mul     x12, x6, x8                 ; Bits <63:0> of pSrc1[0]*pSrc2[j+2]
    adcs    x12, x12, x15               ; Adding the previous word (if there was a carry from the last addition it is added)
    umulh   x15, x6, x8                 ; Bits <127:64> of pSrc1[0]*pSrc2[j+2]
    str     x12, [x4, #16]              ; Store to destination

    mul     x12, x6, x9                 ; Bits <63:0> of pSrc1[0]*pSrc2[j+3]
    adcs    x12, x12, x15               ; Adding the previous word (if there was a carry from the last addition it is added)
    umulh   x15, x6, x9                 ; Bits <127:64> of pSrc1[0]*pSrc2[j+3]
    str     x12, [x4, #24]              ; Store to destination

    add     x2, x2, #32
    add     x4, x4, #32

    cbnz    x3, SymCryptFdefRawMulAsmLoopInner1

    adc     x15, x15, XZR               ; Store the next word into the destination (with the carry if any)
    str     x15, [x4]

    add     x1, x1, #1                  ; move one word up
    add     x0, x0, #8                  ; move start of pSrc1 one word up
    add     x5, x5, #8                  ; move start of pDst one word up

    ;
    ; MAIN LOOP
    ;
SymCryptFdefRawMulAsmLoopOuter
    mov     x3, x17                     ; set -nDigits2
    mov     x2, x16                     ; set pSrc2
    mov     x4, x5                      ; set pDst

    ands    x15, x15, XZR               ; Clearing the carry flag and setting x15 = 0
    ldr     x6, [x0]                    ; load the next word from pSrc1

SymCryptFdefRawMulAsmLoopInner
    add     x3, x3, #1                  ; move one digit up

    ldp     x8, x9, [x2]                ; load 2 words from pSrc2
    ldp     x10, x11, [x4]              ; load 2 words from pDst

    mul     x12, x6, x8                 ; Bits <63:0> of pSrc1[i]*pSrc2[j]
    adcs    x12, x12, x15               ; Adding the previous word (if there was a carry from the last addition it is added)
    umulh   x15, x6, x8                 ; Bits <127:64> of pSrc1[i]*pSrc2[j]
    adc     x15, x15, XZR               ; Add the carry if any and don't update the flags
                                        ; Note: this cannot overflow as the maximum for <x15:x12> is (2^64-1)(2^64-1)+(2^64-1)+1 = 2^128 - 2^64 + 1
    adds    x12, x12, x10               ; add the word from the destination and update the flags (this can overflow)
    str     x12, [x4]                   ; Store to destination

    mul     x12, x6, x9                 ; Bits <63:0> of pSrc1[i]*pSrc2[j+1]
    adcs    x12, x12, x15               ; Adding the previous word (if there was a carry from the last addition it is added)
    umulh   x15, x6, x9                 ; Bits <127:64> of pSrc1[i]*pSrc2[j+1]
    adc     x15, x15, XZR               ; Add the carry if any and don't update the flags
    adds    x12, x12, x11               ; add the word from the destination and update the flags (this can overflow)
    str     x12, [x4, #8]               ; Store to destination

    ldp     x8, x9, [x2, #16]           ; load 2 words from pSrc2
    ldp     x10, x11, [x4, #16]         ; load 2 words from pDst

    mul     x12, x6, x8                 ; Bits <63:0> of pSrc1[i]*pSrc2[j+2]
    adcs    x12, x12, x15               ; Adding the previous word (if there was a carry from the last addition it is added)
    umulh   x15, x6, x8                 ; Bits <127:64> of pSrc1[i]*pSrc2[j+2]
    adc     x15, x15, XZR               ; Add the carry if any and don't update the flags
    adds    x12, x12, x10               ; add the word from the destination and update the flags (this can overflow)
    str     x12, [x4, #16]              ; Store to destination

    mul     x12, x6, x9                 ; Bits <63:0> of pSrc1[i]*pSrc2[j+3]
    adcs    x12, x12, x15               ; Adding the previous word (if there was a carry from the last addition it is added)
    umulh   x15, x6, x9                 ; Bits <127:64> of pSrc1[i]*pSrc2[j+3]
    adc     x15, x15, XZR               ; Add the carry if any and don't update the flags
    adds    x12, x12, x11               ; add the word from the destination and update the flags (this can overflow)
    str     x12, [x4, #24]              ; Store to destination

    add     x2, x2, #32
    add     x4, x4, #32

    cbnz    x3, SymCryptFdefRawMulAsmLoopInner

    adc     x15, x15, XZR               ; Store the next word into the destination (with the carry if any)
    str     x15, [x4]

    adds    x1, x1, #1                  ; move one word up
    add     x0, x0, #8                  ; move start of pSrc1 one word up
    add     x5, x5, #8                  ; move start of pDst one word up

    bne     SymCryptFdefRawMulAsmLoopOuter

    ; Done, no return value

    ret

    LEAF_END ARM64EC_NAME_MANGLE(SymCryptFdefRawMulAsm)






    ; Macro for the first loop of the first pass of RawSquareAsm.
    ; It takes one word from the source, multiplies it with the mulword,
    ; adds the high level word of the previous macro call, and stores it into
    ; the destination.
    ;
    ; No carry flag is propagated from the previous macro call as the maximum is
    ; (2^64-1)^2 + 2^64-1 = 2^128 - 2^64
    MACRO
    SQR_SINGLEADD_64 $index

        ldr     x8, [x2, #8*$index]         ; pSrc[i+j]

        mul     x12, x6, x8                 ; Bits <63:0> of pSrc[i]*pSrc[i+j]
        adds    x12, x12, x15               ; Adding the previous word
        umulh   x15, x6, x8                 ; Bits <127:64> of pSrc[i]*pSrc[i+j]
        adc     x15, x15, XZR               ; Add the intermediate carry and don't update the flags

        str     x12, [x4, #8*$index]        ; Store to destination

    MEND

    ; Macro for the remaining loops of the first pass of RawSquareAsm.
    ; The only difference to the above is that it also adds the word loaded
    ; from the destination buffer.
    ;
    ; No carry flag is propagated from the previous macro call as the maximum is
    ; (2^64-1)^2 + 2(2^64-1) = 2^128 - 1
    MACRO
    SQR_DOUBLEADD_64 $index

        ldr     x8, [x2, #8*$index]         ; pSrc[i+j]
        ldr     x10, [x4, #8*$index]        ; pDst[2*(i+j)]

        mul     x12, x6, x8                 ; Bits <63:0> of pSrc[i]*pSrc[i+j]
        adds    x12, x12, x15               ; Adding the previous word
        umulh   x15, x6, x8                 ; Bits <127:64> of pSrc[i]*pSrc[i+j]
        adc     x15, x15, XZR               ; Add the intermediate carry and don't update the flags

        adds    x12, x12, x10               ; Add the word from the destination
        adc     x15, x15, XZR               ; Add the intermediate carry and don't update the flags

        str     x12, [x4, #8*$index]        ; Store to destination

    MEND

    ; Macro for the third pass loop of RawSquareAsm.
    ; It takes one mulword from the source, squares it, and
    ; adds it to the even columns of the destination. The carries are propagated
    ; to the odd columns.
    ;
    ; Here we can have a (1-bit) carry to the next call because the maximum value for
    ; a pair of columns is (2^64-1)^2+(2^128-1)+1 = 2^129 - 2^65 + 1 < 2^129 - 1
    MACRO
    SQR_DIAGONAL_PROP $index
        ldr     x6, [x0, #8*$index]         ; mulword
        mul     x12, x6, x6                 ; Bits <63:0> of m^2
        umulh   x15, x6, x6                 ; Bits <127:64> of m^2

        ldp     x8, x9, [x4, #16*$index]    ; Load

        ; Adding the square to the even column
        adcs    x12, x12, x8                ; carry from previous and update the flags

        ; Propagating the sum to the next column
        adcs    x15, x15, x9                ; This can generate a carry

        stp     x12, x15, [x4, #16*$index]  ; Store
    MEND

; VOID
; SYMCRYPT_CALL
; SymCryptFdefRawSquareAsm(
;   _In_reads_(nDgigits*SYMCRYPT_FDEF_DIGIT_NUINT32)    PCUINT32    pSrc,
;                                                       UINT32      nDigits,
;   _Out_writes_(2*nWords)                              PUINT32     pDst )
;
; Initial inputs to registers:
;       pSrc        -> x0
;       nDigits     -> x1
;       pDst        -> x2
;
; Register assignments
;       x0  = pSrc
;       x1  = negated word count of pSrc
;       x2  = pSrc (moving forward one digit / 4 words every inner loop)
;       x3  = negated digit count of pSrc
;       x4  = pDst (moving forward one digit every inner loop)
;       x5  = pDst (moving forward one word every outer loop)
;       x6  = Current word loaded from pSrc
;       x8, x9   = Current words loaded in pairs from pSrc2
;       x10, x11 = Current words loaded in pairs from pDst
;       x12, x15 = "128-bit" sliding register to hold the result of multiplies
;       x16 = Stored pSrc
;       x17 = Negated digit count of pSrc
;       x19 = Stored negated digit count of pSrc
;       x20 = Stored pDst
; Note x13, x14 are reserved in ARM64EC and thus are not used


    NESTED_ENTRY ARM64EC_NAME_MANGLE(SymCryptFdefRawSquareAsm)
    PROLOG_SAVE_REG_PAIR fp, lr, #-32!  ; allocate 32 bytes of stack; store FP/LR
    PROLOG_SAVE_REG_PAIR x19, x20, #16  ; free up x19/x20

    mov     x3, x1                      ; digit count into x3

    lsl     x1, x1, #2                  ; Calculate word count

    neg     x1, x1                      ; negate nWords
    neg     x3, x3                      ; negate nDigits

    mov     x4, x2                      ; pDst
    mov     x5, x2                      ; store pDst
    mov     x20, x2                      ; store pDst
    mov     x16, x0                     ; store pSrc
    mov     x2, x0                      ; inner loop pSrc
    mov     x17, x3                     ; store -nDigits for later
    mov     x19, x3                     ; store -nDigits for later

    ;
    ; First iteration of main loop (no adding of previous values from pDst)
    ;
    ands    x15, x15, XZR               ; Clearing the carry flag and setting x15 = 0
    ldr     x6, [x0]                    ; load the first word from pSrc1
    str     x15, [x4]                   ; store 0 for the first word

    b       SymCryptFdefRawSquareAsmInnerLoopInit_Word1

SymCryptFdefRawSquareAsmInnerLoopInit_Word0
    SQR_SINGLEADD_64    0

SymCryptFdefRawSquareAsmInnerLoopInit_Word1
    SQR_SINGLEADD_64    1

    SQR_SINGLEADD_64    2

    SQR_SINGLEADD_64    3

    add     x3, x3, #1                  ; move one digit up
    add     x2, x2, #32
    add     x4, x4, #32

    cbnz    x3, SymCryptFdefRawSquareAsmInnerLoopInit_Word0

    str     x15, [x4]                   ; Store the next word into the destination

    add     x1, x1, #1                  ; move one word up

    mov     x9, #1                      ; Cyclic counter

    ;
    ; MAIN LOOP
    ;
SymCryptFdefRawSquareAsmOuterLoop

    add     x5, x5, #8                  ; move start of pDst one word up

    mov     x3, x17                     ; set -nDigits
    mov     x2, x0                      ; set pSrc
    mov     x4, x5                      ; set pDst

    ands    x15, x15, XZR               ; Clearing the carry flag and setting x15 = 0
    ldr     x6, [x0, x9, LSL #3]        ; load the next word from pSrc

    ; Cyclic counter and jump logic
    add     x9, x9, #1
    cmp     x9, #1
    beq     SymCryptFdefRawSquareAsmInnerLoop_Word1
    cmp     x9, #2
    beq     SymCryptFdefRawSquareAsmInnerLoop_Word2
    cmp     x9, #3
    beq     SymCryptFdefRawSquareAsmInnerLoop_Word3

    ; The following instructions are only executed when x9 == 4
    mov     x9, XZR                 ; Set it to 0

    add     x0, x0, #32             ; move start of pSrc 4 words up
    add     x5, x5, #32             ; move pDst 4 words up

    mov     x2, x0                  ; set pSrc
    mov     x4, x5                  ; set pDst

    adds    x17, x17, #1            ; add 1 digit
    mov     x3, x17                 ; set the new digit counter

SymCryptFdefRawSquareAsmInnerLoop_Word0
    SQR_DOUBLEADD_64    0

SymCryptFdefRawSquareAsmInnerLoop_Word1
    SQR_DOUBLEADD_64    1

SymCryptFdefRawSquareAsmInnerLoop_Word2
    SQR_DOUBLEADD_64    2

SymCryptFdefRawSquareAsmInnerLoop_Word3
    SQR_DOUBLEADD_64    3

    add     x3, x3, #1                  ; move one digit up
    add     x2, x2, #32
    add     x4, x4, #32

    cbnz    x3, SymCryptFdefRawSquareAsmInnerLoop_Word0

    str     x15, [x4]                   ; Store the next word into the destination

    adds    x1, x1, #1                  ; move one word up
    cmn     x1, #1                      ; Compare with -1
    bne     SymCryptFdefRawSquareAsmOuterLoop

    ands    x15, x15, XZR               ; Setting x15 = 0
    str     x15, [x5, #40]              ; Store 0 to destination for the top word

    ; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Second Pass - Shifting all results 1 bit left
    ; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    mov     x3, x19         ; -nDigits
    lsl     x3, x3, #1      ; Double digits
    mov     x4, x20         ; pDst pointer
    ands    x8, x8, XZR     ; Clear the flags

SymCryptFdefRawSquareAsmSecondPass

    add     x3, x3, #1      ; move one digit up

    ldp     x8, x9, [x4]
    adcs    x8, x8, x8      ; Shift left and add the carry
    adcs    x9, x9, x9
    stp     x8, x9, [x4], #16

    ldp     x10, x11, [x4]
    adcs    x10, x10, x10   ; Shift left and add the carry
    adcs    x11, x11, x11
    stp     x10, x11, [x4], #16

    cbnz    x3, SymCryptFdefRawSquareAsmSecondPass

    ; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
    ; Third Pass - Adding the squares on the even columns and propagating the sum
    ; ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

    ands    x8, x8, XZR     ; Clear the flags
    mov     x0, x16         ; src pointer
    mov     x4, x20         ; pDst pointer
    mov     x3, x19         ; -nDigits

SymCryptFdefRawSquareAsmThirdPass
    SQR_DIAGONAL_PROP 0
    SQR_DIAGONAL_PROP 1
    SQR_DIAGONAL_PROP 2
    SQR_DIAGONAL_PROP 3

    add     x3, x3, #1          ; move one digit up
    add     x0, x0, #32         ; One digit up (not updated in SQR_DIAGONAL_PROP)
    add     x4, x4, #64         ; Two digits up (not updated in SQR_DIAGONAL_PROP)

    cbnz    x3, SymCryptFdefRawSquareAsmThirdPass

    ; Done, no return value

    EPILOG_RESTORE_REG_PAIR x19, x20, #16
    EPILOG_RESTORE_REG_PAIR fp, lr, #32!
    EPILOG_RETURN

    NESTED_END ARM64EC_NAME_MANGLE(SymCryptFdefRawSquareAsm)

;VOID
;SymCryptFdefMontgomeryReduceAsm(
;    _In_                            PCSYMCRYPT_MODULUS      pmMod,
;    _In_                            PUINT32                 pSrc,
;    _Out_                           PUINT32                 pDst )
;
; Initial inputs to registers:
;       pmMod       -> x0
;       pSrc        -> x1
;       pDst        -> x2
;
; Register assignments
;       x0  = pMod (moving forward one *digit* every inner loop)
;       x1  = pSrc (moving forward one *digit* every inner loop)
;       x2  = pDst (used only in the end for subtract / result)
;       x3  = negated digit count of pSrc and pMod
;       x4  = negated word count of pSrc
;       x5  = Inv64 of the modulus
;       x6  = m = pSrc[i]*Inv64
;       x7  = hc = high carry variable
;       x8, x9   = Current words loaded in pairs from pSrc
;       x10, x11 = Current words loaded in pairs from pMod
;       x12, x15 = c variable = "128-bit" register to hold the result of multiplies
;                  It is flipped between [x12:x15] and [x15:x12] intstead of doing c>>=64
;       x16 = Temporary intermediate result
;       x17 = Stored negated digit count of pSrc
;       x19 = Stored pMod pointer
;       x20 = Stored pSrc pointer (moving forward one word every outer loop)
; Note x13, x14 are reserved in ARM64EC and thus are not used

    NESTED_ENTRY ARM64EC_NAME_MANGLE(SymCryptFdefMontgomeryReduceAsm)
    PROLOG_SAVE_REG_PAIR fp, lr, #-32!
    PROLOG_SAVE_REG_PAIR x19, x20, #16

    ldr     w3, [x0, #SymCryptModulusNdigitsOffsetArm64]            ; # of Digits
    ldr     x5, [x0, #SymCryptModulusMontgomeryInv64OffsetArm64]    ; Inv64 of modulus
    add     x0, x0, #SymCryptModulusValueOffsetArm64                ; pMod

    lsl     x4, x3, #2                  ; Multiply by 4 to get the number of words

    neg     x3, x3                      ; Negate the digit count
    neg     x4, x4                      ; Negate the word count

    mov     x17, x3                     ; Store the digit count for later
    mov     x19, x0                     ; Store the pMod pointer
    mov     x20, x1                     ; Store the pSrc pointer

    ands    x7, x7, XZR                 ; Set hc to 0

    ;
    ; Main loop
    ;
SymCryptFdefMontgomeryReduceAsmOuter
    ldr     x8, [x1]                    ; Load 1 word from pSrc
    mul     x6, x8, x5                  ; <63:0> bits of pSrc[i]*Inv64 = m

    ands    x12, x12, XZR               ; Set c to 0
    ands    x15, x15, XZR               ; Set c to 0

SymCryptFdefMontgomeryReduceAsmInner
    ldp     x10, x11, [x0]              ; pMod[j]
    ldp     x8, x9, [x1]                ; pSrc[j]

    mul     x16, x6, x10                ; <63:0> of pMod[j]*m
    adds    x16, x16, x8                ; Adding pSrc[j]
    umulh   x15, x6, x10                ; <127:64> of pMod[j]*m
    adc     x15, x15, XZR               ; Add the carry if any (***)
    adds    x12, x12, x16               ; Add the lower bits of c
    adc     x15, x15, XZR               ; Add the carry if any (***)
    ; ***: These cannot produce extra carry as the maximum is
    ;      (2^64 - 1)*(2^64-1) + 2^64-1 + 2^64-1 = 2^128 - 1
    str     x12, [x1]                   ; pSrc[j] = (UINT64) c

    mul     x16, x6, x11                ; <63:0> of pMod[j]*m
    adds    x16, x16, x9                ; Adding pSrc[j]
    umulh   x12, x6, x11                ; <127:64> of pMod[j]*m
    adc     x12, x12, XZR               ; Add the carry if any (***)
    adds    x15, x15, x16               ; Add the lower bits of c
    adc     x12, x12, XZR               ; Add the carry if any (***)
    str     x15, [x1, #8]               ; pSrc[j] = (UINT64) c

    ldp     x10, x11, [x0, #16]         ; pMod[j]
    ldp     x8, x9, [x1, #16]           ; pSrc[j]

    mul     x16, x6, x10                ; <63:0> of pMod[j]*m
    adds    x16, x16, x8                ; Adding pSrc[j]
    umulh   x15, x6, x10                ; <127:64> of pMod[j]*m
    adc     x15, x15, XZR               ; Add the carry if any (***)
    adds    x12, x12, x16               ; Add the lower bits of c
    adc     x15, x15, XZR               ; Add the carry if any (***)
    str     x12, [x1, #16]              ; pSrc[j] = (UINT64) c

    mul     x16, x6, x11                ; <63:0> of pMod[j]*m
    adds    x16, x16, x9                ; Adding pSrc[j]
    umulh   x12, x6, x11                ; <127:64> of pMod[j]*m
    adc     x12, x12, XZR               ; Add the carry if any (***)
    adds    x15, x15, x16               ; Add the lower bits of c
    adc     x12, x12, XZR               ; Add the carry if any (***)
    str     x15, [x1, #24]              ; pSrc[j] = (UINT64) c

    add     x0, x0, #32
    add     x1, x1, #32
    adds    x3, x3, #1                  ; Move one digit up
    bne     SymCryptFdefMontgomeryReduceAsmInner

    ldr     x8, [x1]                    ; pSrc[nWords]
    adds    x12, x12, x8                ; c + pSrc[nWords]
    adc     x15, XZR, XZR               ; Add the carry if any

    adds    x12, x12, x7                ; c + pSrc[nWords] + hc
    adc     x7, x15, XZR                ; Add the carry if any and store into hc

    str     x12, [x1]                   ; pSrc[nWords] = c

    adds    x4, x4, #1                  ; Move one word up

    add     x20, x20, #8                ; Move stored pSrc pointer one word up
    mov     x0, x19                     ; Restore pMod pointer
    mov     x1, x20                     ; Restore pSrc pointer

    mov     x3, x17                     ; Restore the digit counter

    bne     SymCryptFdefMontgomeryReduceAsmOuter

    ;
    ; Subtraction
    ;

    mov     x16, x2                 ; Store pDst pointer

    ; Prepare the pointers for subtract
    mov     x0, x20                 ; pSrc
    mov     x1, x19                 ; pMod

    mov     x10, x7                 ; x10 = hc
    mov     x3, x17                 ; Restore the digit counter
    subs    x4, x4, x4              ; Set the carry flag (i.e. no borrow)

SymCryptFdefMontgomeryReduceRawSubAsmLoop
    add     x3, x3, #1              ; Increment the digit count by one
    ; borrow is in the carry flag (flipped)

    ldp     x4, x6, [x0], #16       ; Load two words of pSrc1
    ldp     x5, x7, [x1], #16       ; Load two words of pSrc2
    sbcs    x4, x4, x5
    sbcs    x6, x6, x7
    stp     x4, x6, [x2], #16       ; Store the result in the destination


    ldp     x4, x6, [x0], #16       ; Load two words of pSrc1
    ldp     x5, x7, [x1], #16       ; Load two words of pSrc2
    sbcs    x4, x4, x5
    sbcs    x6, x6, x7
    stp     x4, x6, [x2], #16       ; Store the result in the destination

    cbnz    x3, SymCryptFdefMontgomeryReduceRawSubAsmLoop

    csetcc  x0                      ; If the carry is clear (borrow), set the return value to 1

    orr     x11, x10, x0            ; x11 = hc|d

    ; Prepare the pointers for masked copy
    mov     x0, x20                 ; pSrc
    mov     x1, x16                 ; pDst

    mov     x2, x17                 ; Restore the digit counter
    subs    x4, x10, x11            ; If (x11 > x10) clear the carry flag (i.e. borrow)

SymCryptFdefMontgomeryReduceMaskedCopyAsmLoop
    add     x2, x2, #1              ; Increment the digit count by one

    ldp     x4, x6, [x0], #16       ; Load two words of the source
    ldp     x5, x7, [x1]            ; Load two words of the destination
    cselcc  x4, x4, x5              ; If the carry is clear, select the source operands
    cselcc  x6, x6, x7
    stp     x4, x6, [x1], #16       ; Store the two words in the destination

    ldp     x4, x6, [x0], #16
    ldp     x5, x7, [x1]
    cselcc  x4, x4, x5
    cselcc  x6, x6, x7
    stp     x4, x6, [x1], #16

    cbnz    x2, SymCryptFdefMontgomeryReduceMaskedCopyAsmLoop

    ; Done, no return value

    EPILOG_RESTORE_REG_PAIR x19, x20, #16
    EPILOG_RESTORE_REG_PAIR fp, lr, #32!
    EPILOG_RETURN

    NESTED_END ARM64EC_NAME_MANGLE(SymCryptFdefMontgomeryReduceAsm)

    END

