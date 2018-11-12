; generated by Component: ARM Compiler 5.06 update 5 (build 528) Tool: ArmCC [4d3621]
; commandline ArmCC [--c99 --split_sections --debug -c -S -o.\objects\itoa.s --depend=.\objects\itoa.d --cpu=Cortex-M4.fp --apcs=interwork -O0 --diag_suppress=9931 -I.\cores -I.\RTE\Device\XMC4700-F144x2048 -I.\RTE\_Target_1 -IC:\Keil_v5\ARM\PACK\ARM\CMSIS\5.0.1\CMSIS\Include -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\Device\XMC4700_series\Include -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\Device\XMClib\inc -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\RTE_Driver -D__UVISION_VERSION=524 -D_RTE_ -DXMC4700_F144x2048 --omf_browse=.\objects\itoa.crf cores\itoa.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.itoa||, CODE, READONLY, ALIGN=1

itoa PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        MOV      r6,r2
        MOV      r2,r6
        MOV      r1,r5
        MOV      r0,r4
        BL       ltoa
        POP      {r4-r6,pc}
        ENDP


        AREA ||i.ltoa||, CODE, READONLY, ALIGN=1

ltoa PROC
        PUSH     {r4-r7,lr}
        SUB      sp,sp,#0x24
        MOV      r5,r0
        MOV      r6,r1
        MOV      r1,sp
        CBNZ     r6,|L1.18|
        MOVS     r0,#0
|L1.14|
        ADD      sp,sp,#0x24
        POP      {r4-r7,pc}
|L1.18|
        CMP      r2,#0x24
        BGT      |L1.26|
        CMP      r2,#1
        BGT      |L1.30|
|L1.26|
        MOVS     r0,#0
        B        |L1.14|
|L1.30|
        CMP      r2,#0xa
        BNE      |L1.42|
        CMP      r5,#0
        BGE      |L1.42|
        MOVS     r0,#1
        B        |L1.44|
|L1.42|
        MOVS     r0,#0
|L1.44|
        MOV      r12,r0
        CMP      r12,#0
        BEQ      |L1.56|
        RSBS     r4,r5,#0
        B        |L1.58|
|L1.56|
        MOV      r4,r5
|L1.58|
        B        |L1.94|
|L1.60|
        UDIV     r0,r4,r2
        MLS      r7,r2,r0,r4
        UDIV     r4,r4,r2
        CMP      r7,#0xa
        BGE      |L1.86|
        ADD      r0,r7,#0x30
        STRB     r0,[r1],#1
        B        |L1.94|
|L1.86|
        ADD      r0,r7,#0x57
        STRB     r0,[r1],#1
|L1.94|
        CMP      r4,#0
        BNE      |L1.60|
        MOV      r0,sp
        CMP      r1,r0
        BEQ      |L1.60|
        MOV      r3,r6
        CMP      r12,#0
        BEQ      |L1.118|
        MOVS     r0,#0x2d
        STRB     r0,[r3],#1
|L1.118|
        B        |L1.130|
|L1.120|
        SUBS     r0,r1,#1
        MOV      r1,r0
        LDRB     r0,[r0,#0]
        STRB     r0,[r3],#1
|L1.130|
        MOV      r0,sp
        CMP      r1,r0
        BHI      |L1.120|
        MOVS     r0,#0
        STRB     r0,[r3,#0]
        MOV      r0,r6
        B        |L1.14|
        ENDP


        AREA ||i.ultoa||, CODE, READONLY, ALIGN=1

ultoa PROC
        PUSH     {r4-r7,lr}
        SUB      sp,sp,#0x24
        MOV      r7,r0
        MOV      r3,r1
        MOV      r1,sp
        MOV      r6,r7
        CBNZ     r3,|L2.20|
        MOVS     r0,#0
|L2.16|
        ADD      sp,sp,#0x24
        POP      {r4-r7,pc}
|L2.20|
        CMP      r2,#0x24
        BGT      |L2.28|
        CMP      r2,#1
        BGT      |L2.32|
|L2.28|
        MOVS     r0,#0
        B        |L2.16|
|L2.32|
        B        |L2.68|
|L2.34|
        UDIV     r0,r6,r2
        MLS      r4,r2,r0,r6
        UDIV     r6,r6,r2
        CMP      r4,#0xa
        BGE      |L2.60|
        ADD      r0,r4,#0x30
        STRB     r0,[r1],#1
        B        |L2.68|
|L2.60|
        ADD      r0,r4,#0x57
        STRB     r0,[r1],#1
|L2.68|
        CMP      r6,#0
        BNE      |L2.34|
        MOV      r0,sp
        CMP      r1,r0
        BEQ      |L2.34|
        MOV      r5,r3
        B        |L2.92|
|L2.82|
        SUBS     r0,r1,#1
        MOV      r1,r0
        LDRB     r0,[r0,#0]
        STRB     r0,[r5],#1
|L2.92|
        MOV      r0,sp
        CMP      r1,r0
        BHI      |L2.82|
        MOVS     r0,#0
        STRB     r0,[r5,#0]
        MOV      r0,r3
        B        |L2.16|
        ENDP


        AREA ||i.utoa||, CODE, READONLY, ALIGN=1

utoa PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r5,r1
        MOV      r6,r2
        MOV      r2,r6
        MOV      r1,r5
        MOV      r0,r4
        BL       ultoa
        POP      {r4-r6,pc}
        ENDP


        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        EXPORT itoa [CODE]
        EXPORT ltoa [CODE]
        EXPORT ultoa [CODE]
        EXPORT utoa [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]

        ATTR FILESCOPE
        ATTR SETVALUE Tag_ABI_PCS_wchar_t,2
        ATTR SETVALUE Tag_ABI_enum_size,1
        ATTR SETVALUE Tag_ABI_optimization_goals,6
        ATTR SETSTRING Tag_conformance,"2.09"
        ATTR SETVALUE AV,18,1

        ASSERT {ENDIAN} = "little"
        ASSERT {INTER} = {TRUE}
        ASSERT {ROPI} = {FALSE}
        ASSERT {RWPI} = {FALSE}
        ASSERT {IEEE_FULL} = {FALSE}
        ASSERT {IEEE_PART} = {FALSE}
        ASSERT {IEEE_JAVA} = {FALSE}
        END
