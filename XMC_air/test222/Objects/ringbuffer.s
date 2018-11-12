; generated by Component: ARM Compiler 5.06 update 5 (build 528) Tool: ArmCC [4d3621]
; commandline ArmCC [--cpp --split_sections --debug -c -S -o.\objects\ringbuffer.s --depend=.\objects\ringbuffer.d --cpu=Cortex-M4.fp --apcs=interwork -O0 --diag_suppress=9931 -I.\cores -I.\RTE\Device\XMC4700-F144x2048 -I.\RTE\_Target_1 -IC:\Keil_v5\ARM\PACK\ARM\CMSIS\5.0.1\CMSIS\Include -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\Device\XMC4700_series\Include -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\Device\XMClib\inc -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\RTE_Driver -D__UVISION_VERSION=524 -D_RTE_ -DXMC4700_F144x2048 --omf_browse=.\objects\ringbuffer.crf cores\RingBuffer.cpp]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i._ZN10RingBuffer10store_charEh||, CODE, READONLY, ALIGN=1

_ZN10RingBuffer10store_charEh PROC
        LDR      r3,[r0,#0x80]
        ADDS     r3,r3,#1
        AND      r2,r3,#0x7f
        LDR      r3,[r0,#0x84]
        CMP      r3,r2
        BEQ      |L0.28|
        LDR      r3,[r0,#0x80]
        STRB     r1,[r0,r3]
        STR      r2,[r0,#0x80]
|L0.28|
        BX       lr
        ENDP


        AREA ||i._ZN10RingBufferC1Ev||, CODE, READONLY, ALIGN=1

_ZN10RingBufferC2Ev                  ; Alternate entry point
_ZN10RingBufferC1Ev PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        MOVS     r1,#0x80
        MOV      r0,r4
        BL       __aeabi_memclr4
        MOVS     r0,#0
        STR      r0,[r4,#0x80]
        STR      r0,[r4,#0x84]
        MOV      r0,r4
        POP      {r4,pc}
        ENDP


        AREA ||.ARM.exidx||, LINKORDER=||i._ZN10RingBufferC1Ev||, DATA, READONLY, SECTYPE={SHT_ARM_EXIDX}, ALIGN=2

        DCD      0x00000000
        RELOC 42, ||i._ZN10RingBufferC1Ev||
        DCD      0x00000001

        AREA ||area_number.3||, LINKORDER=||i._ZN10RingBuffer10store_charEh||, DATA, READONLY, SECTYPE={SHT_ARM_EXIDX}, ALIGN=2

        EXPORTAS ||area_number.3||, ||.ARM.exidx||
        DCD      0x00000000
        RELOC 42, ||i._ZN10RingBuffer10store_charEh||
        DCD      0x00000001

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        EXPORT _ZN10RingBuffer10store_charEh [CODE]
        EXPORT _ZN10RingBufferC2Ev [CODE]
        EXPORT _ZN10RingBufferC1Ev [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT ||Lib$$Request$$cpplib|| [CODE,WEAK]
        IMPORT __aeabi_memclr4 [CODE]

        ATTR FILESCOPE
        ATTR SETVALUE Tag_ABI_PCS_wchar_t,2
        ATTR SETVALUE Tag_ABI_enum_size,1
        ATTR SETVALUE Tag_ABI_optimization_goals,6
        ATTR SETSTRING Tag_conformance,"2.09"
        ATTR SETVALUE AV,6,0
        ATTR SETVALUE AV,18,1

        ASSERT {ENDIAN} = "little"
        ASSERT {INTER} = {TRUE}
        ASSERT {ROPI} = {FALSE}
        ASSERT {RWPI} = {FALSE}
        ASSERT {IEEE_FULL} = {FALSE}
        ASSERT {IEEE_PART} = {FALSE}
        ASSERT {IEEE_JAVA} = {FALSE}
        END