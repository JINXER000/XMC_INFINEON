/*! @file driver.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Implementation of HardDriver for the XMC4700Discovery board.
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#include <driver.h>
#include <XMC4700.h>
#include <Receive.h>
#include "xmc_gpio.h"
#include "xmc_uart.h"
extern volatile uint16_t baro_alt_air;
extern volatile uint16_t baro_h_t;
extern volatile uint8_t baro_height;
extern volatile int8_t baro_h_offset;

volatile uint8_t UART1_rx_buffer;
extern XMC_USIC_CH_t           *stm_uart;
extern XMC_USIC_CH_t           *pc_uart;
extern XMC_USIC_CH_t           *air_uart;
static uint8_t uart1_tx_data_agm[23];
static uint8_t uart1_tx_data_attitude[17];
static uint8_t uart1_tx_data_states[40];
static uint8_t data2stm[50];

static uint8_t data2xmc_air[10];



void send(XMC_USIC_CH_t *const channel,const uint8_t* buf, size_t len)
{
  char* p = (char*)buf;

//  if (NULL == buf)
//  {
//    return 0;
//  }

  while (len--)
  {

    XMC_UART_CH_Transmit(channel, *p++);
  }
//  return 1;
}

void send2xmc_air(uint8_t send_flag)
{
	unsigned int temp = 0xFA + 0xFB; //  for sum check
	
	data2xmc_air[0] = 0xFA;
	data2xmc_air[1] = 0xFB;
	data2xmc_air[2] = send_flag;          //0x21: Only enable Radar ; 0x22: Only enable delta_h; 0x23: enable Radar and delta_h
  data2xmc_air[3] = 1;                //data_len
	data2xmc_air[4] = (int8_t)baro_h_offset;
	
	for(uint8_t j=2; j<5; j++)
	temp += data2xmc_air[j];
 
	data2xmc_air[5]=temp%256;
	
	send(air_uart,data2xmc_air,6);

}

void send2stm(uint8_t send_flag)
{
	unsigned int temp = 0xFA + 0xFB; //  for sum check
	uint8_t ctemp; // for transmitted data
	//int16_t atemp; // for data multiplication

	data2stm[0] = 0xFA;
	data2stm[1] = 0xFB;
	data2stm[2] = send_flag;          //0x21: Only enable Radar ; 0x22: Only enable delta_h; 0x23: enable Radar and delta_h
	data2stm[3] = 1+1+6*s_target_list.target_num; //数据长度 ： 相对高度+ 目标数量 + 目标数据内容（uint16_t distance /float level: 2+4=6）
	data2stm[4] = (uint8_t)baro_h_offset;
	data2stm[5] =s_target_list.target_num;
  for(uint8_t i=0;i< s_target_list.target_num;i++)
	{
		ctemp = (uint8_t)(s_target_list.distance[i]>>8);
		data2stm[6+6*i] = ctemp ;
		ctemp =  (uint8_t)(s_target_list.distance[i]);
		data2stm[7+6*i] = ctemp ;
		ctemp = (uint8_t)((uint32_t)s_target_list.level[i]>>24);
		data2stm[8+6*i] = ctemp ;
		ctemp = (uint8_t)((uint32_t)s_target_list.level[i]>>16);
		data2stm[9+6*i] = ctemp ;
		ctemp = (uint8_t)((uint32_t)s_target_list.level[i]>>8);
		data2stm[10+6*i] = ctemp ;
		ctemp =  (uint8_t)((uint32_t)s_target_list.level[i]);
		data2stm[11+6*i] = ctemp ;
	}
	
	uint8_t n = 6+6*s_target_list.target_num;
	for(uint8_t j=2; j<n; j++)
	temp += data2stm[j];
 
	data2stm[n]=temp%256;
	
	send(stm_uart,data2stm,(n+1));
	
}

void UART1_Prepare_Attitude(void) 
{
	unsigned int temp = 0xAA + 0xAA + 0x01 + 12; //  for sum check
	char ctemp; // for transmitted data
	int16_t atemp; // for data multiplication

	uart1_tx_data_attitude[0] = 0xAA;
	uart1_tx_data_attitude[1] = 0xAA;
	uart1_tx_data_attitude[2] = 0x01;
	uart1_tx_data_attitude[3] = 12;
	
	//Roll
	atemp = (int16_t)(0*100.0); //roll
	ctemp = atemp>>8;
	uart1_tx_data_attitude[4] = ctemp;
	temp += ctemp;
	ctemp = atemp;
	uart1_tx_data_attitude[5] = ctemp;
	temp += ctemp;
	
	//Pitch
	atemp = (int16_t)(0*100.0); // pitch
	ctemp = atemp>>8;
	uart1_tx_data_attitude[6] = ctemp;
	temp += ctemp;
	ctemp = atemp;
	uart1_tx_data_attitude[7] = ctemp;
	temp += ctemp;
	
	//Yaw
	atemp = (int16_t)(0*100.0); //yaw
	ctemp = atemp>>8;
	uart1_tx_data_attitude[8] = ctemp;
	temp += ctemp;
	ctemp = atemp;
	uart1_tx_data_attitude[9] = ctemp;
	temp += ctemp;
	
	// Sonar_height

	atemp = baro_h_offset*10;
	ctemp = 0;
//	ctemp = atemp>>24;
	uart1_tx_data_attitude[10] = ctemp;
	temp += ctemp;
	ctemp = 0;
//	ctemp = atemp>>16;
	uart1_tx_data_attitude[11] = ctemp;
	temp += ctemp;
	ctemp = atemp>>8;
	uart1_tx_data_attitude[12] = ctemp;
	temp += ctemp;
	ctemp = atemp;
	uart1_tx_data_attitude[13] = ctemp;
	temp += ctemp;
	

	//Fly_mode
	ctemp = 0;
	uart1_tx_data_attitude[14] = ctemp;
	temp += ctemp;
	
	// is_Armed
	ctemp = 0;
	uart1_tx_data_attitude[15] = ctemp;
	temp += ctemp;
	
	// send sum check byte
	uart1_tx_data_attitude[16] = temp%256;
}
void UART1_Prepare_AGM(void) 
{
	unsigned int temp = 0xAA + 0xAA + 0x02 + 18; // for sum check
	char ctemp; // for transmitted data
	unsigned int i;
	int16_t acc_gyro_test[6];
	
	uart1_tx_data_agm[0] = 0xAA;
	uart1_tx_data_agm[1] = 0xAA;
	uart1_tx_data_agm[2] = 0x02;
	uart1_tx_data_agm[3] = 18;
	
	for(i=0;i<6;i++)
	{
		acc_gyro_test[i] =0;// Acc_Gyro[i];
	}
	acc_gyro_test[3] *= 0;//GYRO_GA;
	//acc_gyro_test[4] = p_Tgt;
	
	//Acc_Gyro
	for(i=0;i<6;i++)
	{
		ctemp = acc_gyro_test[i]>>8;
		uart1_tx_data_agm[2*i+4] = ctemp;
		temp += ctemp;
		ctemp = acc_gyro_test[i];
		uart1_tx_data_agm[2*i+5] = ctemp;
		temp += ctemp;
	}
	
	//Mag
	for(i=0;i<3;i++)
	{
		ctemp = 0;//Mag[i]>>8;
		uart1_tx_data_agm[2*i+16] = ctemp;
		temp += ctemp;
		ctemp = 0;//Mag[i];
		uart1_tx_data_agm[2*i+17] = ctemp;
		temp += ctemp;
	}
	
	// send sum check byte
	uart1_tx_data_agm[22] = temp%256;
}
void UART1_Report_States(void)
{
	static unsigned int i, j;
	
	
	//*** prepare for the acc_gyro_gam data ***
	UART1_Prepare_AGM();
	for(i=0;i<23;i++)
	{
		uart1_tx_data_states[i] = uart1_tx_data_agm[i];
	}	
	
	//*** prepare for the attitude data ***
	UART1_Prepare_Attitude();
	for(j=0;j<17;j++)
	{
		uart1_tx_data_states[j+23] = uart1_tx_data_attitude[j];
	}
	
	//start sending
	send(pc_uart,uart1_tx_data_states, 40);	
}
