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

#include "../../SLS10ERE1/Include/SWI_BIF_Transactor.h"
#include <XMC4700.h>
//#include "Hardware.h"
//#include "support.h"
//#include "xmc_ccu4.h"
#include "xmc_gpio.h"
#include "board.h"

unsigned long g_ulBaudLow;				//1 * SWI_DEFAULT_TIMEBASE
unsigned long g_ulBaudHigh;				//3 * SWI_DEFAULT_TIMEBASE
unsigned long g_ulBaudStop;				//5 * SWI_DEFAULT_TIMEBASE
unsigned long g_ulResponseTimeOut;		//20 * SWI_DEFAULT_TIMEBASE
unsigned long g_ulBaudPowerUpPulse;
unsigned long g_ulBaudPowerDownTime;
unsigned long g_ulBaudPowerUpTime;
unsigned long g_ulBaudResetTime;
unsigned long g_ulIntTimeoutDefault;
unsigned long g_ulImmediateIntTimeOut;

unsigned long g_culNvmTimeout;

unsigned char g_ubDevType;

//#define GET_P5_0	((PORT5->IN & (0x01<<0))?1:0)//lily changed 
//#define GET_P2_5	((PORT2->IN & (0x01<<5))?1:0)//lily changed

//#define SLS10ERE_INTERNAL_PWR_CTRL(A,B)	Control_P1_6(A,B)
//#define	SLS10ERE_INT_PWR_PIN				P1_6
//#define SLS10ERE_EXTERNAL_PWR_CTRL(A,B)	Control_P1_7(A,B)
//#define	SLS10ERE_EXT_PWR_PIN				P1_7
//#define SLS10ERE_SWI_CTRL(A,B)			Control_P5_0(A,B)
//#define SET_SLS10ERE_INT_PWR_PIN 			SET_P1_6		/*!< macro to set a specified pin */
//#define SET_SLS10ERE_EXT_PWR_PIN 			SET_P1_7		/*!< macro to set a specified pin */
//#define RESET_SLS10ERE_INT_PWR_PIN 		RESET_P1_6		/*!< macro to set a specified pin */
//#define RESET_SLS10ERE_EXT_PWR_PIN 		RESET_P1_7		/*!< macro to set a specified pin */


void board_init(unsigned char dev)
{
	g_ubDevType = dev;

	if(g_ubDevType == 2)
	{
		/*SLS10ERE_SWI_CTRL(OUTPUT_OD_GP, STRONG);
		SLS10ERE_INTERNAL_PWR_CTRL(OUTPUT_PP_GP, STRONG);
		Set (SLS10ERE_INT_PWR_PIN);
		SLS10ERE_EXTERNAL_PWR_CTRL(OUTPUT_PP_GP, STRONG);
		Reset (SLS10ERE_EXT_PWR_PIN);*/
		
	}

	// 10us
	SWI_BIFHALSetTauSWI_BIF(0, 200);//
}

void SWI_BIFDelay(unsigned short delay)
{
	unsigned long data = delay;

	for(;data;data--);
}

void timing_init(void)
{
	g_ulBaudLow 				= 50;		//1 * SWI_DEFAULT_TIMEBASE;	5us
	g_ulBaudHigh 				= 50*3+20;		//3 * SWI_DEFAULT_TIMEBASE; 15us
	g_ulBaudStop				= 50*5;		//5 * SWI_DEFAULT_TIMEBASE; 25us
	g_ulResponseTimeOut 		= 50*20;	//20 * SWI_DEFAULT_TIMEBASE; 100 us

	g_ulBaudPowerUpPulse 		= 450;		// 30us
	g_ulBaudPowerDownTime		= 40000;	// 2ms
	g_ulBaudPowerUpTime			= 15000;	// 1ms
	g_ulBaudResetTime			= 5000;

	g_ulIntTimeoutDefault     	= 300000; 	// 200ms
	g_ulImmediateIntTimeOut   	= 30000; 	// 20ms

	g_culNvmTimeout 			= 500000;
}


unsigned char get_pin()
{
	if(g_ubDevType == 1)
		return XMC_GPIO_GetInput(P5_11);//Lily
	else if(g_ubDevType==2)
		return XMC_GPIO_GetInput(P5_11);//Lily
	else
		return 0;
}

void set_pin(unsigned char level)
{
	if(g_ubDevType == 1)
	{

	}// DO NOTHING
	else if (g_ubDevType == 2)
	{
		if(level==1)
	  XMC_GPIO_SetOutputHigh(P5_11);//set
		else if(level==0)
		XMC_GPIO_SetOutputLow(P5_11);//reset
	}
}

void set_pin_dir(unsigned char dir)
{
if(dir==1)
	XMC_GPIO_SetMode(P5_11, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);//out put
		else if(dir==0)
		{ 
			XMC_GPIO_SetMode(P5_11, XMC_GPIO_MODE_INPUT_PULL_UP);
     /* Enable digital input. Only needed because P15.12 is an analog port */ //?
     XMC_GPIO_EnableDigitalInput(P5_11);
		}
}

void test_output(unsigned char testnum, unsigned char res, unsigned char * str)
{
	if(res)
		//LED(testnum);
	 SWI_BIFDelay(50);
}

void test_pass(void)
{
		XMC_GPIO_SetMode(P5_8, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);//out put
    XMC_GPIO_SetOutputHigh(P5_8);//LED on 
	  SWI_BIFDelay(1000);

}

void test_fail(void)
{
	XMC_GPIO_SetMode(P5_9, XMC_GPIO_MODE_OUTPUT_PUSH_PULL);//out put
    XMC_GPIO_SetOutputHigh(P5_9);//LED on 
	  SWI_BIFDelay(10000);
		XMC_GPIO_SetOutputLow(P5_9);//LED off
	  SWI_BIFDelay(10000);
	  XMC_GPIO_SetOutputHigh(P5_9);//LED on 
	  SWI_BIFDelay(10000);
		XMC_GPIO_SetOutputLow(P5_9);//LED off
}

void udelay(volatile unsigned long ul_ticks)
{

	for(;ul_ticks;ul_ticks--);

}



