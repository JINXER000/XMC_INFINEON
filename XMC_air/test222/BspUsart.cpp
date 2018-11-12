/*! @file BspUsart.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Usart helper functions and ISR for board XMC4700Discovery
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#include "BspUsart.h"

#define USIC0_SR5          5





extern unsigned char come_data;
extern unsigned char Rx_length;
extern int Rx_adr;
extern int Rx_Handle_Flag;
extern unsigned char Rx_buff[];

#define PC_UART_RX_PIN      P1_4
#define PC_UART_TX_PIN      P1_5       //U0C0

#define STM_UART_RX_PIN      P3_12 //P0_4 
#define STM_UART_TX_PIN      P3_11 //P0_5       //U2C1     U1C1 used by I2C (DPS310)

#define GROUND_UART_RX_PIN      P3_7       //P5_1
#define GROUND_UART_TX_PIN      P3_8     //U2C0    P5_0


#define RADAR_UART_RX_PIN      P0_4   //P3_12
#define RADAR_UART_TX_PIN      P0_5  //P3_11     //U1C0 


/* PC_UART configuration */
XMC_USIC_CH_t           *pc_uart = XMC_UART0_CH0;
XMC_UART_CH_CONFIG_t     pc_uart_config =
{
		115200U,
		8U,
		8U,
		1U,
		16U,
		XMC_USIC_CH_PARITY_MODE_NONE
};	

XMC_GPIO_CONFIG_t        pc_rx_pin_config;
XMC_GPIO_CONFIG_t        pc_tx_pin_config;


/* STM_UART configuration */
XMC_USIC_CH_t           *stm_uart = XMC_UART2_CH1;
XMC_UART_CH_CONFIG_t     stm_uart_config =
{
		115200,
		8U,
		8U,
		1U,
		16U,
		XMC_USIC_CH_PARITY_MODE_NONE
};	

XMC_GPIO_CONFIG_t        stm_rx_pin_config;
XMC_GPIO_CONFIG_t        stm_tx_pin_config;



/* RADAR_UART configuration */
XMC_USIC_CH_t           *radar_uart = XMC_UART1_CH0;
XMC_UART_CH_CONFIG_t     radar_uart_config =
{
		115200U,
		8U,
		8U,
		1U,
		16U,
		XMC_USIC_CH_PARITY_MODE_NONE
};	

XMC_GPIO_CONFIG_t        radar_rx_pin_config;
XMC_GPIO_CONFIG_t        radar_tx_pin_config;

/* GROUND_UART configuration */
XMC_USIC_CH_t           *ground_uart = XMC_UART2_CH0;
XMC_UART_CH_CONFIG_t     ground_uart_config =
{
		115200U,
		8U,
		8U,
		1U,
		16U,
		XMC_USIC_CH_PARITY_MODE_NONE
};	

XMC_GPIO_CONFIG_t       ground_rx_pin_config;
XMC_GPIO_CONFIG_t       ground_tx_pin_config;

/*
 * PC_UART is used for receiving commands from PC and
 * printing debug information to PC
 */
void PC_UART_Config(void)
{
  XMC_UART_CH_Init(pc_uart, &pc_uart_config);
  XMC_UART_CH_SetInputSource(pc_uart, XMC_UART_CH_INPUT_RXD , USIC0_C0_DX0_P1_4);
  XMC_UART_CH_Start(pc_uart);  // uart.CCR.mode=UART-> USIC0_CH0 is switched for UART operation mode


  /*PC_UART Rx pin configuration*/
  pc_rx_pin_config.mode = XMC_GPIO_MODE_INPUT_PULL_UP;          // Rx input with internal pull-up
  XMC_GPIO_Init(PC_UART_RX_PIN, &pc_rx_pin_config);
  /*PC_UART Tx pin configuration*/
  pc_tx_pin_config.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH;
  pc_tx_pin_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2;  // Tx output in ALT2
  XMC_GPIO_Init(PC_UART_TX_PIN, &pc_tx_pin_config);
}

/*
 * STM_UART is used for communicating with the DJI flight controller
 * The Baud rate needs to match the Baud rate used by the flight controller
 */
void STM_UART_Config(void)
{

  XMC_UART_CH_Init(stm_uart, &stm_uart_config);
  XMC_UART_CH_SetInputSource(stm_uart, XMC_UART_CH_INPUT_RXD , USIC2_C1_DX0_P3_12);
  XMC_UART_CH_Start(stm_uart);  // uart.CCR.mode=UART-> USIC2_CH1 is switched for UART operation mode


  /*STM_UART Rx pin configuration*/
  stm_rx_pin_config.mode = XMC_GPIO_MODE_INPUT_PULL_UP;          // Rx input with internal pull-up
  XMC_GPIO_Init(STM_UART_RX_PIN, &stm_rx_pin_config);
  /*STM_UART Tx pin configuration*/
  stm_tx_pin_config.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH;
  stm_tx_pin_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1;  // Tx output in ALT1
  XMC_GPIO_Init(STM_UART_TX_PIN, &stm_tx_pin_config);
}	

void RADAR_UART_Config(void)
{

  XMC_UART_CH_Init(radar_uart, &radar_uart_config);
  XMC_UART_CH_SetInputSource(radar_uart, XMC_UART_CH_INPUT_RXD , USIC1_C0_DX0_P0_4);
  XMC_UART_CH_Start(radar_uart);  // uart.CCR.mode=UART-> USIC2_CH1 is switched for UART operation mode


  /*RADAR_UART Rx pin configuration*/
  radar_rx_pin_config.mode = XMC_GPIO_MODE_INPUT_PULL_UP;          // Rx input with internal pull-up
  XMC_GPIO_Init(RADAR_UART_RX_PIN, &radar_rx_pin_config);
  /*RADAR_UART Tx pin configuration*/
  radar_tx_pin_config.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH;
  radar_tx_pin_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2;  // Tx output in ALT2
  XMC_GPIO_Init(RADAR_UART_TX_PIN, &radar_tx_pin_config);
	
	
	XMC_USIC_CH_EnableEvent(radar_uart, XMC_USIC_CH_EVENT_STANDARD_RECEIVE);                                            // enable interrupt RI
  XMC_USIC_CH_SetInterruptNodePointer(radar_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE, 5);             // set USIC0 SR5 for RI
  XMC_USIC_CH_EnableEvent(radar_uart, XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE);                                         // 'USIC_AI.007' -> AIR ON
  XMC_USIC_CH_SetInterruptNodePointer(radar_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE, 5);

  NVIC_SetPriority(USIC1_5_IRQn,5);//NVIC_EncodePriority(NVIC_GetPriorityGrouping(),63U, 0U));   // CMSIS function for NVIC control: NVIC_SetPriority(IRQn_t IRQn, uint32_t priority): priority=0..63
  NVIC_EnableIRQ(USIC1_5_IRQn);       // CMSIS function for NVIC control: enable IRQn

}	

void GROUND_UART_Config(void)
{

  XMC_UART_CH_Init(ground_uart, &ground_uart_config);
  XMC_UART_CH_SetInputSource(ground_uart, XMC_UART_CH_INPUT_RXD , USIC2_C0_DX0_P3_7);
  XMC_UART_CH_Start(ground_uart);  // uart.CCR.mode=UART-> USIC2_CH0 is switched for UART operation mode


  /*GROUND_UART Rx pin configuration*/
  ground_rx_pin_config.mode = XMC_GPIO_MODE_INPUT_PULL_UP;          // Rx input with internal pull-up
  XMC_GPIO_Init(GROUND_UART_RX_PIN, &ground_rx_pin_config);
  /*GROUND_UART Tx pin configuration*/
  ground_tx_pin_config.output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH;
  ground_tx_pin_config.mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2;  // Tx output in ALT2
  XMC_GPIO_Init(GROUND_UART_TX_PIN, &ground_tx_pin_config);
	
	
  XMC_USIC_CH_EnableEvent(ground_uart, XMC_USIC_CH_EVENT_STANDARD_RECEIVE);                                            // enable interrupt RI
  XMC_USIC_CH_SetInterruptNodePointer(ground_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE, USIC0_SR5);             // set USIC0 SR5 for RI
  XMC_USIC_CH_EnableEvent(ground_uart, XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE);                                         // 'USIC_AI.007' -> AIR ON
  XMC_USIC_CH_SetInterruptNodePointer(ground_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE, USIC0_SR5);

  NVIC_SetPriority(USIC2_5_IRQn,4);   // CMSIS function for NVIC control: NVIC_SetPriority(IRQn_t IRQn, uint32_t priority): priority=0..63
  NVIC_EnableIRQ(USIC2_5_IRQn);       // CMSIS function for NVIC control: enable IRQn
	
	
}	

void UARTxNVIC_Config()
{
	
	XMC_USIC_CH_EnableEvent(pc_uart, XMC_USIC_CH_EVENT_STANDARD_RECEIVE);                                            // enable interrupt RI
  XMC_USIC_CH_SetInterruptNodePointer(pc_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE, USIC0_SR5);             // set USIC0 SR5 for RI
  XMC_USIC_CH_EnableEvent(pc_uart, XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE);                                         // 'USIC_AI.007' -> AIR ON
  XMC_USIC_CH_SetInterruptNodePointer(pc_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE, USIC0_SR5);

  NVIC_SetPriority(USIC0_5_IRQn,6);   // CMSIS function for NVIC control: NVIC_SetPriority(IRQn_t IRQn, uint32_t priority): priority=0..63
  NVIC_EnableIRQ(USIC0_5_IRQn);       // CMSIS function for NVIC control: enable IRQn

	XMC_USIC_CH_EnableEvent(stm_uart, XMC_USIC_CH_EVENT_STANDARD_RECEIVE);                                            // enable interrupt RI
  XMC_USIC_CH_SetInterruptNodePointer(stm_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_RECEIVE, 4);             // set USIC0 SR5 for RI
  XMC_USIC_CH_EnableEvent(stm_uart, XMC_USIC_CH_EVENT_ALTERNATIVE_RECEIVE);                                         // 'USIC_AI.007' -> AIR ON
  XMC_USIC_CH_SetInterruptNodePointer(stm_uart, XMC_USIC_CH_INTERRUPT_NODE_POINTER_ALTERNATE_RECEIVE, 4);

  NVIC_SetPriority(USIC2_4_IRQn,7);   // CMSIS function for NVIC control: NVIC_SetPriority(IRQn_t IRQn, uint32_t priority): priority=0..63
  NVIC_EnableIRQ(USIC2_4_IRQn);       // CMSIS function for NVIC control: enable IRQn
	




}

void UartConfig()
{
  PC_UART_Config();
  STM_UART_Config();
	GROUND_UART_Config();
	RADAR_UART_Config();
  UARTxNVIC_Config();
}

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

//void USIC1_5_IRQHandler(void)
//{
//   // coreApi->byteHandler(XMC_UART_CH_GetReceivedData(stm_uart)); //Data from M100 were committed to "byteHandler"
//}
//void USIC2_4_IRQHandler(void)    //RADAR communication
//{
//	
// 

//}

#ifdef __cplusplus
}
#endif //__cplusplus
