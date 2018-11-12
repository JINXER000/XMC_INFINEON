/*! @file BspUsart.h
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Usart helper functions and ISR for board XMC4700Discovery
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#ifndef _BSPUSART_H
#define _BSPUSART_H
#include <XMC4700.h>
#include <xmc_scu.h>
#include "xmc_gpio.h"
#include "xmc_uart.h"
#include "stdio.h"
extern XMC_USIC_CH_t           *pc_uart;
extern XMC_USIC_CH_t           *stm_uart;
extern XMC_USIC_CH_t           *ground_uart;
extern XMC_USIC_CH_t           *radar_uart ;

void PC_UART_Config(void);
void STM_UART_Config(void);
void RADAR_UART_Config(void);
void GROUND_UART_Config(void);
void UARTxNVIC_Config(void);
void UartConfig(void);
void NVIC_Config(void);
#endif  //_USART_H
