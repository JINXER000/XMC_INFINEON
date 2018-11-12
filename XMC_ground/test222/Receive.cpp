/*! @file Receive.cpp
 *
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  This function parses Rx buffer and execute commands sent from computer.
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#include "Receive.h"
//#include "LocalNavigation.h"
#include "common.h"

extern XMC_USIC_CH_t           *pc_uart;
extern XMC_USIC_CH_t           *stm_uart;
extern XMC_USIC_CH_t           *radar_uart;
extern XMC_USIC_CH_t           *air_uart;
extern int32_t pressure_ground;
extern uint16_t baro_alt_ground;
extern volatile  int8_t  baro_h_offset;
#define CMD_PC_FLAG  1
#define CMD_STM_FLAG  2
#define CMD_RADAR_FLAG  3
#define CMD_GROUND_FLAG  4

uint8_t ground_pack_len = 0xFF;
uint8_t test_rx = 0;
Target_List_t  s_target_list;


/*
 * @brief Helper function to assemble two bytes into a float number
 */
static float hex2Float(uint8_t HighByte, uint8_t LowByte)
{
  float high = (float) (HighByte & 0x7f);
  float low  = (float) LowByte;
  if (HighByte & 0x80)//MSB is 1 means a negative number
  {
    return -(high*256.0f + low)/100.0f;
  }
  else
  {
    return (high*256.0f + low)/100.0f;
  }
}

TerminalCommand Terminal_pc;
TerminalCommand Terminal_stm;
TerminalCommand Terminal_radar;
TerminalCommand Terminal_air;

void Data_Receive_Prepare(TerminalCommand* terminal,u8 data)
{
	
		if(terminal->rx_state==0&&data==0xFA)
	{
		terminal->rx_state=1;
		terminal->cmdIn[0]=data;
	}
	else if(terminal->rx_state==1&&data==0xFB)
	{
		terminal->rx_state=2;
		terminal->cmdIn[1]=data;
	}
	else if(terminal->rx_state==2&&data<0XF1)     //功能字
	{
		terminal->rx_state=3;
		terminal->cmdIn[2]=data;
	}
	else if(terminal->rx_state==3&&data<50)
	{
		terminal->rx_state = 4;
		terminal->cmdIn[3]=data;
		terminal->data_len = data;
		terminal->data_cnt = 0;
	}
	else if(terminal->rx_state==4&&terminal->data_len>0)
	{
		terminal->data_len--;
		terminal->cmdIn[4+terminal->data_cnt++]=data;
		if(terminal->data_len==0)
			terminal->rx_state = 5;
	}
	else if(terminal->rx_state==5)
	{
		terminal->rx_state = 0;
		terminal->cmdIn[4+terminal->data_cnt]=data;
		terminal->terminalCommandHandler();
	}
	else
	{
		terminal->rx_state = 0;
		//terminal->cmdReadyFlag = 0;
	}
		
}


void TerminalCommand::terminalCommandHandler(void)
{
	uint16_t checksum=0;
	for(uint8_t i=0;i<(4+data_cnt);i++)
	checksum+=cmdIn[i];
	
	checksum = checksum%256;
	if(checksum != cmdIn[data_cnt+4])
	{	
		//printf("check failed\n");
		
		return;
	}
	switch(cmdIn[2])
	{
		case 0x00:       //ground only send pressure data
      			
			baro_h_offset =(int8_t) cmdIn[4];
		  break;
		case 0x01:       //ground  send pressure data and temperature data
      pressure_ground  = ((uint32_t)cmdIn[4]<<24)|((uint32_t)cmdIn[5]<<16)|((uint32_t)cmdIn[6]<<8)|((uint32_t)cmdIn[7]);
			break;
    case 0x02:       // other cmds reserved;
		
		case 0x10:       //雷达数据
			 s_target_list.target_num = cmdIn[4];
		   for(uint8_t i=0;i<cmdIn[4];i++){
			    s_target_list.distance[i] = ((uint16_t)cmdIn[5+6*i]<<8)|((uint16_t)cmdIn[6+6*i]);
				  s_target_list.level[i] =  ((uint32_t)cmdIn[7+6*i]<<24)|((uint32_t)cmdIn[8+6*i]<<16)|((uint32_t)cmdIn[9+6*i]<<8)|((uint32_t)cmdIn[10+6*i]);
			 }
		   
			break;					
	  default:printf("others");
	    break;
	}
			


  
}

#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus

void USIC0_5_IRQHandler(void)     //pc Communication
{
    uint8_t oneByte = XMC_UART_CH_GetReceivedData(pc_uart);
	  Data_Receive_Prepare(&Terminal_pc,oneByte);
}	

void USIC1_5_IRQHandler(void)     //STM Communication
{
   	uint8_t oneByte = XMC_UART_CH_GetReceivedData(stm_uart);
	  Data_Receive_Prepare(&Terminal_stm,oneByte);
}

void USIC2_4_IRQHandler(void)    //RADAR Communication
{
	  uint8_t oneByte = XMC_UART_CH_GetReceivedData(radar_uart); 
	  Data_Receive_Prepare(&Terminal_radar,oneByte);
}
void USIC2_5_IRQHandler(void)     //AIR Communication
{
	  uint8_t oneByte = XMC_UART_CH_GetReceivedData(air_uart);
	   Data_Receive_Prepare(&Terminal_air,oneByte);
  
}
#ifdef __cplusplus
}
#endif //__cplusplus


