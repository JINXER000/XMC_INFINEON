/*! @file Receive.h
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

#ifndef RECEIVE_H
#define	RECEIVE_H
#include "common.h"

#define MAX_RECEIVE 32
#define NUM_OF_TARGETS   6

typedef struct Target_List
{
	uint8_t    	target_num;		/**< non-zero value if target detected else zero */
	uint16_t   	distance[NUM_OF_TARGETS];		/**< distance of target detected in units of cm */
	float      	level[NUM_OF_TARGETS];			/**< absolute magnitude FFT spectrum values calculated by FMCW algorithm */

} Target_List_t;

extern Target_List_t s_target_list;

class TerminalCommand
{
public:
  uint32_t cmdReadyFlag; //Rx_Handle_Flag
  uint8_t  cmdIn[MAX_RECEIVE]; //Rx_buff
  int32_t  rxIndex;  //Rx_adr
  int32_t  rxLength; //Rx_length
  
	uint8_t data_len,data_cnt;
	uint8_t rx_state;
  void terminalCommandHandler(void);

  TerminalCommand():rx_state(0),data_len(0),data_cnt(0),cmdReadyFlag(0),rxIndex(0),rxLength(0){;}
};

void Data_Receive_Prepare(TerminalCommand* terminal,u8 data);
#endif //RECEIVE_H

