/*! @file timer.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Timer helper functions and ISR for board XMC4700Discovery
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */


#include "timer.h"
//#include "DJI_VirtualRC.h"
//#include "main.h"
#include <xmc_ccu4.h>
#include <xmc_gpio.h>
#include <xmc_scu.h>



uint32_t tick = 0; //tick is the time stamp,which record how many ms since u initialize the system.
//extern VirtualRC virtualrc;
//extern VirtualRCData myVRCdata;
//extern FlightData flightData;
//extern Flight flight;
//extern unsigned char Rx_buff[];
//extern TerminalCommand myTerminal;

const XMC_CCU4_SLICE_COMPARE_CONFIG_t SLICE_config =
{
  (uint32_t) XMC_CCU4_SLICE_TIMER_COUNT_MODE_EA,
  (uint32_t) false,
  (uint32_t) 0,
  (uint32_t) 0,
  (uint32_t) 0,
  (uint32_t) XMC_CCU4_SLICE_PRESCALER_MODE_NORMAL,
  (uint32_t) 0,
  (uint32_t) 8, 	/* 144MHz/256 = 0.5625MHz */
  (uint32_t) 0,
  (uint32_t) 0,
  (uint32_t) XMC_CCU4_SLICE_OUTPUT_PASSIVE_LEVEL_LOW,
  (uint32_t) 0
};

void Timer1Config(void)
{
	
	/* Enable clock, enable prescaler block and configure global control */
	XMC_CCU4_Init(MODULE_PTR, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);

	/* Start the prescaler and restore clocks to slices */
	XMC_CCU4_StartPrescaler(MODULE_PTR);

	/* Start of CCU4 configurations */
	/* Ensure fCCU reaches CCU40 */
	XMC_CCU4_SetModuleClock(MODULE_PTR, XMC_CCU4_CLOCK_SCU);

	//•	Configure Slice(s) Functions, Interrupts and Start-up
	/* Initialize the Slice */
	XMC_CCU4_SLICE_CompareInit(SLICE0_PTR, &SLICE_config);

	XMC_CCU4_SLICE_SetTimerCompareMatch(SLICE0_PTR, 3000-1);
	XMC_CCU4_SLICE_SetTimerPeriodMatch(SLICE0_PTR, 5625-1); // 5625 * (1/562500Hz) = 10ms

	/* Enable shadow transfer */
	XMC_CCU4_EnableShadowTransfer(MODULE_PTR, 	\
			(uint32_t)(XMC_CCU4_SHADOW_TRANSFER_SLICE_0|	\
					XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_0));

	/* Enable Period match events */
	XMC_CCU4_SLICE_EnableEvent(SLICE0_PTR, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);


	/* Connect period match event to SR0 */
	XMC_CCU4_SLICE_SetInterruptNode(SLICE0_PTR, \
	XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH, XMC_CCU4_SLICE_SR_ID_0);

	/* Set NVIC priority */
	NVIC_SetPriority(CCU40_0_IRQn, 3U);

	/* Enable IRQ */
	NVIC_EnableIRQ(CCU40_0_IRQn);

	/* Get the slice out of idle mode */
	XMC_CCU4_EnableClock(MODULE_PTR, SLICE0_NUMBER);

	
}
void Timer2Config(void)
{
		
	/* Enable clock, enable prescaler block and configure global control */
	XMC_CCU4_Init(MODULE_PTR, XMC_CCU4_SLICE_MCMS_ACTION_TRANSFER_PR_CR);

	/* Start the prescaler and restore clocks to slices */
	XMC_CCU4_StartPrescaler(MODULE_PTR);

	/* Start of CCU4 configurations */
	/* Ensure fCCU reaches CCU40 */
	XMC_CCU4_SetModuleClock(MODULE_PTR, XMC_CCU4_CLOCK_SCU);
	//•	Configure Slice(s) Functions, Interrupts and Start-up
	/* Initialize the Slice */
	XMC_CCU4_SLICE_CompareInit(SLICE1_PTR, &SLICE_config);

	XMC_CCU4_SLICE_SetTimerCompareMatch(SLICE1_PTR, 3000-1);
	XMC_CCU4_SLICE_SetTimerPeriodMatch(SLICE1_PTR, 5625-1); // 5625 * (1/562500Hz) = 10ms

	/* Enable shadow transfer */
	XMC_CCU4_EnableShadowTransfer(MODULE_PTR, 	\
			(uint32_t)(XMC_CCU4_SHADOW_TRANSFER_SLICE_1|	\
					XMC_CCU4_SHADOW_TRANSFER_PRESCALER_SLICE_1));

	/* Enable Period match events */
	XMC_CCU4_SLICE_EnableEvent(SLICE1_PTR, XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH);


	/* Connect period match event to SR0 */
	XMC_CCU4_SLICE_SetInterruptNode(SLICE1_PTR, \
	XMC_CCU4_SLICE_IRQ_ID_PERIOD_MATCH, XMC_CCU4_SLICE_SR_ID_1);

	/* Set NVIC priority */
	NVIC_SetPriority(CCU40_1_IRQn, 3U);

	/* Enable IRQ */
	NVIC_EnableIRQ(CCU40_1_IRQn);

	/* Get the slice out of idle mode */
	XMC_CCU4_EnableClock(MODULE_PTR, SLICE1_NUMBER);
	//Start Timer Running
	//XMC_CCU4_SLICE_StartTimer(SLICE0_PTR);
	
}
void SystickConfig(void)
{
  if (SysTick_Config(SystemCoreClock / 1000)) //1000 ticks per second.
  {
    while (1)
      ;  //run here when error.
  }
}

/*
void delay(uint16_t time)
{
  uint32_t i = 0;
  while (time--)
  {
    i = 30000;
    while (i--)
      ;
  }
}*/
void delay_ms(uint32_t delayTimeInMillis)
{
	uint32_t now = tick;
	while((now+delayTimeInMillis)>tick);
}

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

void SysTick_Handler(void)
{
//  if (tick > 4233600000ll)  //49 days non-reset would cost a tick reset.
//  {
//    tick = 0;
//  }
  tick++;
}
void CCU40_0_IRQHandler(void)
{

    //virtualrc.sendData(myVRCdata);
  
}

void CCU40_1_IRQHandler()
{
    /*if ((myTerminal.cmdIn[2] == 0x04) && (myTerminal.cmdIn[3] == 0x01))
    {
      flight.setFlight(&flightData);
    }
    else
    {
      XMC_CCU4_SLICE_StopTimer(SLICE0_PTR);
    }*/

}
#ifdef __cplusplus
}
#endif //__cplusplus
