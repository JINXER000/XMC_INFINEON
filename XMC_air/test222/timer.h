/*! @file timer.h
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Timer helper functions and ISR for board XMC4700Discovery
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#ifndef  TIMER_H
#define  TIMER_H
//#include "DJI_API.h"
#include "string.h"
#include <XMC4700.h>


#define MODULE_PTR      	CCU40
#define MODULE_NUMBER   	(0U)

#define SLICE0_PTR       	CCU40_CC40
#define SLICE0_NUMBER    	(0U)
#define SLICE1_PTR       	CCU40_CC41
#define SLICE1_NUMBER    	(1U)


//using namespace DJI::onboardSDK;
void SystickConfig(void);
void Timer1Config(void);
void Timer2Config(void);
void delay_ms(uint32_t time);
extern uint32_t tick;
extern uint32_t counter_20ms;
extern uint32_t counter_110ms;
#endif //TIMER_H
