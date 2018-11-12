; generated by Component: ARM Compiler 5.06 update 5 (build 528) Tool: ArmCC [4d3621]
; commandline ArmCC [--c99 --split_sections --debug -c -S -o.\objects\yield.s --depend=.\objects\yield.d --cpu=Cortex-M4.fp --apcs=interwork -O0 --diag_suppress=9931 -I.\cores -I.\RTE\Device\XMC4700-F144x2048 -I.\RTE\_Target_1 -IC:\Keil_v5\ARM\PACK\ARM\CMSIS\5.0.1\CMSIS\Include -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\Device\XMC4700_series\Include -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\Device\XMClib\inc -IC:\Keil_v5\ARM\PACK\Infineon\XMC4000_DFP\2.9.1\RTE_Driver -D__UVISION_VERSION=524 -D_RTE_ -DXMC4700_F144x2048 --omf_browse=.\objects\yield.crf cores\yield.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.yield||, CODE, READONLY, ALIGN=1

yield PROC
        BX       lr
        ENDP


        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        EXPORT yield [CODE,WEAK]

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
