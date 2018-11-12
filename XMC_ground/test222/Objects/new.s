; generated by Component: ARM Compiler 5.06 update 5 (build 528) Tool: ArmCC [4d3621]
; commandline ArmCC [--cpp --split_sections --debug -c -S -o.\objects\new.s --depend=.\objects\new.d --cpu=Cortex-M4.fp --apcs=interwork -O0 --diag_suppress=9931 -I.\cores -I.\RTE\Device\XMC4700-F144x2048 -I.\RTE\_Target_1 -IC:\Keil_v5\ARM\PACK\ARM\CMSIS\5.0.1\CMSIS\Include -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\Device\XMC4700_series\Include -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\Device\XMClib\inc -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\RTE_Driver -D__UVISION_VERSION=524 -D_RTE_ -DXMC4700_F144x2048 --omf_browse=.\objects\new.crf cores\new.cpp]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i._ZdaPv||, CODE, READONLY, ALIGN=1

_ZdaPv PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        MOV      r0,r4
        BL       free
        POP      {r4,pc}
        ENDP


        AREA ||i._ZdlPv||, CODE, READONLY, ALIGN=1

_ZdlPv PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        MOV      r0,r4
        BL       free
        POP      {r4,pc}
        ENDP


        AREA ||i._Znaj||, CODE, READONLY, ALIGN=1

_Znaj PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        MOV      r0,r4
        BL       malloc
        POP      {r4,pc}
        ENDP


        AREA ||i._Znwj||, CODE, READONLY, ALIGN=1

_Znwj PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        MOV      r0,r4
        BL       malloc
        POP      {r4,pc}
        ENDP


        AREA ||.ARM.exidx||, LINKORDER=||i._Znwj||, DATA, READONLY, SECTYPE={SHT_ARM_EXIDX}, ALIGN=2

        DCD      0x00000000
        RELOC 42, ||i._Znwj||
        DCD      0x00000001

        AREA ||area_number.5||, LINKORDER=||i._Znaj||, DATA, READONLY, SECTYPE={SHT_ARM_EXIDX}, ALIGN=2

        EXPORTAS ||area_number.5||, ||.ARM.exidx||
        DCD      0x00000000
        RELOC 42, ||i._Znaj||
        DCD      0x00000001

        AREA ||area_number.6||, LINKORDER=||i._ZdlPv||, DATA, READONLY, SECTYPE={SHT_ARM_EXIDX}, ALIGN=2

        EXPORTAS ||area_number.6||, ||.ARM.exidx||
        DCD      0x00000000
        RELOC 42, ||i._ZdlPv||
        DCD      0x00000001

        AREA ||area_number.7||, LINKORDER=||i._ZdaPv||, DATA, READONLY, SECTYPE={SHT_ARM_EXIDX}, ALIGN=2

        EXPORTAS ||area_number.7||, ||.ARM.exidx||
        DCD      0x00000000
        RELOC 42, ||i._ZdaPv||
        DCD      0x00000001

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        EXPORT _ZdaPv [CODE]
        EXPORT _ZdlPv [CODE]
        EXPORT _Znaj [CODE]
        EXPORT _Znwj [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT ||Lib$$Request$$cpplib|| [CODE,WEAK]
        IMPORT malloc [CODE]
        IMPORT free [CODE]

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
