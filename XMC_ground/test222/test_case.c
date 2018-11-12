/***************************************************************************************
 * Copyright 2013 Infineon Technologies AG ( www.infineon.com ).                       *
 * All rights reserved.                                                                *
 *                                                                                     *
 * Licensed  Material-Property of Infineon Technologies AG.                            *
 * This software is made available solely pursuant to the terms of Infineon            *
 * Technologies AG agreement which governs its use. This code and the information      *
 * contained in it are proprietary and confidential to Infineon Technologies AG.       *
 * No person is allowed to copy, reprint, reproduce or publish any part of this code,  *
 * nor disclose its contents to others, nor make any use of it, nor allow or assist    *
 * others to make any use of it - unless by prior Written express authorization of     *
 * Infineon Technologies AG and then only to the extent authorized.                    *
 *                                                                                     *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,            *
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,           *
 * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT, ARE DISCLAIMED.  IN NO       *
 * EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,     *
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,                 *
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;         *
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY             *
 * WHETHER IN  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR            *
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF              *
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                          *
 *                                                                                     *
 ***************************************************************************************/

#include "./SLS10ERE1/include/SLS10ERE.h"
#include "./SLS10ERE1/include/SLS10ERE_SWI_BIF.h"
#include "./SLS10ERE1/include/SLS10ERE_auth.h"
#include "./SLS10ERE1/include/SLS10ERE_nvm.h"

#include "board.h"
#include "test_case.h"
#include "./SLS10ERE1/include/SWI_BIF_Transactor.h"

#include "stdio.h"

#define ECC_TEST
#define NVM_TEST

unsigned char devCnt = 0;

S_SLS10ERE_PUID stDetectedPuid[4];


unsigned char str[32];
#define SLS10ERE_NVM_START			0x0
#define SLS10ERE_NVM_SIZE				0x1A0
#define SLS10ERE_NVM_LOOP_TEST		1
static unsigned char NVM_Write_Buffer[SLS10ERE_NVM_SIZE];
static unsigned char NVM_Read_Buffer[SLS10ERE_NVM_SIZE];
unsigned char Sls10ere_NVM_Test(void)
{
	int i, loop;
	BOOL irq;

	for(i=0; i<sizeof(NVM_Write_Buffer); i++)
	{
		NVM_Read_Buffer[i] = 0xFF;
		NVM_Write_Buffer[i] = i;
	}

	if( Sls10ere_nvm_read_data(SLS10ERE_NVM_START, SLS10ERE_NVM_SIZE, NVM_Read_Buffer ) == FALSE )
		return 0;

	for(loop=0; loop<SLS10ERE_NVM_LOOP_TEST; loop++)
	{
		irq = TRUE;
		if(Sls10ere_nvm_write_data( TRUE, irq,  SLS10ERE_NVM_START, SLS10ERE_NVM_SIZE, NVM_Write_Buffer) == FALSE)
			return 0;

//		irq = FALSE;
//		if(Sls10ere_nvm_write_data( TRUE, irq,  SLS10ERE_NVM_START, SLS10ERE_NVM_SIZE, NVM_Write_Buffer) == FALSE)
//			return 0;
	}

	return 1;
}

#define SLS10ERE_ECC_LOOP_TEST		1
unsigned char Sls10ere_ECC_Test(void)
{
	int loop, dev;
	BOOL irq;

	irq = TRUE;
	for(loop=0; loop<SLS10ERE_ECC_LOOP_TEST; loop++)
	{
		for(dev=0; dev<devCnt; dev++)
		{
			SWI_BIFTrans_SDA(0, dev+2);
			if(Sls10ere_ECC(irq, &stDetectedPuid[dev]) == FALSE)
				return 0;
		}
	}
	return 1;
}

unsigned char Sls10ere_Test_Init(void)
{
	board_init(2);
	timing_init();

	SWI_BIFTrans_HardRST(0);
	SWI_BIFTrans_WakeUpFromPWDN(0);
	udelay(1000000);

	if(Sls10ere_SWI_BIF_search_ids (80, &stDetectedPuid[0], &devCnt) == FALSE)
	{
		test_fail();
		return 0;
	}
	else
	{
		return 1;
	}

}

unsigned char Sls10ere_Run_Test(void)
{
#ifdef ECC_TEST
	if(Sls10ere_ECC_Test() == 1)
	{
		test_output(0, 1, str);
	}
	else
	{
		test_fail();
		return 0;
	 }
#endif

#ifdef NVM_TEST
	if(Sls10ere_NVM_Test() == 1)
	{
		test_output(1, 1, str);
	}
	else
	{
		test_fail();
		return 0;
	}
#endif

	test_pass();

	return 1;
}

