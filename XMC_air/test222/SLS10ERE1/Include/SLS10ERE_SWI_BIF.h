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

#ifndef _SLS10ERE_SWI_BIF_H_
#define _SLS10ERE_SWI_BIF_H_

/*************************** Include Files ***************************/
#include "SWI_BIF_transactor.h"
#include "SLS10ERE_type.h"

/************************* Constant Define ***************************/

// time base in micro second
#define SWI_BIF_DEFAULT_TIMEBASE 	5   // 200 kHz
#define SWI_BIF_TIMEBASE_MAX 		153
#define SWI_BIF_TIMEBASE_MIN 		2

// transaction elements (uint16_t CODE)
// broadcast
#define SWI_BIF_BC  	  	  	0x08  	// bus command
#define SWI_BIF_EDA  	  	  	0x09  	// extended device address
#define SWI_BIF_SDA  	  	  	0x0A  	// slave device address

// multicast
#define SWI_BIF_WD  	  	  	0x04  	// write data
#define SWI_BIF_ERA  	  	  	0x05  	// extended register address
#define SWI_BIF_WRA  	  	  	0x06  	// write register address
#define SWI_BIF_RRA  	  	  	0x07  	// read register address

// unicast
#define SWI_BIF_RD_ACK  	  	0x06  	// read data - ACK and not end of transmission
#define SWI_BIF_RD_NACK  	  	0x04  	// read data - no ACK and not end of transmission
#define SWI_BIF_RD_ACK_EOT  	0x07  	// read data - ACK and end of transmission
#define SWI_BIF_RD_NACK_EOT     0x05  	// read data - no ACK and end of transmission
#define SWI_BIF_TACK            0x07  	// transaction acknowledge

// bus command
#define SWI_BIF_BRES  	  	  	0x00  	// Bus Reset
#define SWI_BIF_PDWN            0x02    // Put slave devices into Power Down mode
#define SWI_BIF_STBY            0x03    // Put slave devices into Standby mode
#define SWI_BIF_EINT       		0x10    // Enable interrupts for all Slave devices
#define SWI_BIF_ISTS            0x11	// Poll Interrupt Status
#define SWI_BIF_DASM 			0x40    // Slave devices remain selected despite a new Device Address is set
#define SWI_BIF_DISS			0x80    // Starts a UID search at the MSB
#define SWI_BIF_DILC			0x81 	// Probes if the last bit of the UID is reached
#define SWI_BIF_DIE0			0x84 	// Enters the 0 UID branch
#define SWI_BIF_DIE1			0x85 	// Enters the 1 UID branch
#define SWI_BIF_DIP0			0x86	// Probes the 0 UID branch
#define SWI_BIF_DIP1			0x87 	// Probes the 1 UID branch
#define SWI_BIF_DRES			0xc0	// Reset the selected device
#define SWI_BIF_TQ              0xc2    // Transaction Query
#define SWI_BIF_AIO             0xc4    // Address Increment Off

// mainly used for SWI_BIFtch case in SWI_BIF_locate_function.
#define SWI_BIF_PROTOCOL_FUNCTION_TYPE		        0x01
#define SWI_BIF_SLAVE_CONTROL_FUNCTION_TYPE      	0x02
#define SWI_BIF_TEMPERATURE_SENSOR_FUNCTION_TYPE 	0x03
#define SWI_BIF_NVM_FUNCTION_TYPE                	0x04
#define SWI_BIF_AUTHENTICATION_FUNCTION_TYPE     	0x05
#define SWI_BIF_MAC_FUNCTION_TYPE     		        0x06
#define SWI_BIF_MISC_FUNCTION_TYPE     		        0x07
#define SWI_BIF_BKILL_FUNCTION_TYPE     		    0x08
#define SWI_BIF_USER_MANAGEMENT_FUNCTION_TYPE     	0x09
#define SWI_BIF_DEBUG_FUNCTION_TYPE     		    0x0A

/*************************** Type Define ****************************/

/*********************** ENUM, Structure and Union ***********************/

typedef enum _E_RBL
{
	SWI_BIF_NO_RBL	= 0x00,
	SWI_BIF_RBL0	= 0x20,
	SWI_BIF_RBL1 	= 0x21,
	SWI_BIF_RBL2 	= 0x22,
	SWI_BIF_RBL3 	= 0x23,
	SWI_BIF_RBL4 	= 0x24,
	SWI_BIF_RBL5 	= 0x25,
	SWI_BIF_RBL6 	= 0x26,
	SWI_BIF_RBL7 	= 0x27,
	SWI_BIF_RBL8 	= 0x28,
	SWI_BIF_RBL9 	= 0x29,
	SWI_BIF_RBLA 	= 0x2A,
	SWI_BIF_RBLB 	= 0x2B,
	SWI_BIF_RBLC 	= 0x2C,
	SWI_BIF_RBLD 	= 0x2D,
	SWI_BIF_RBLE 	= 0x2E,
	SWI_BIF_RBLF 	= 0x2F
}E_RBL;

typedef enum _E_RBE
{
	SWI_BIF_NO_RBE	= 0x00,
	SWI_BIF_RBE0 	= 0x30,
	SWI_BIF_RBE1 	= 0x31,
	SWI_BIF_RBE2 	= 0x32,
	SWI_BIF_RBE3 	= 0x33,
	SWI_BIF_RBE4 	= 0x34,
	SWI_BIF_RBE5 	= 0x35,
	SWI_BIF_RBE6 	= 0x36,
	SWI_BIF_RBE7 	= 0x37,
	SWI_BIF_RBE8 	= 0x38,
	SWI_BIF_RBE9 	= 0x39,
	SWI_BIF_RBEA 	= 0x3A,
	SWI_BIF_RBEB 	= 0x3B,
	SWI_BIF_RBEC 	= 0x3C,
	SWI_BIF_RBED 	= 0x3D,
	SWI_BIF_RBEE 	= 0x3E,
	SWI_BIF_RBEF 	= 0x3F
}E_RBE;

typedef struct _S_SWI_BIF_WORDBITS
{
  	uint16_t unused	:15;
  	uint16_t INV  	:1;
  	uint16_t P0		:1;
  	uint16_t P1     :1;
  	uint16_t P2     :1;
  	uint16_t P3     :1;
  	uint16_t D0  	:1;
	uint16_t D1  	:1;
  	uint16_t D2  	:1;
  	uint16_t D3  	:1;
  	uint16_t D4  	:1;
  	uint16_t D5  	:1;
  	uint16_t D6  	:1;
  	uint16_t D7  	:1;
  	uint16_t D8  	:1;
  	uint16_t D9  	:1;
  	uint16_t _BCF   :1;
  	uint16_t BCF  	:1;
}S_SWI_BIF_WORDBITS;

typedef struct _S_SWI_BIF_WORDABSTRACT
{
  	uint16_t unused 	:15;
  	uint16_t INV  	:1;
  	uint16_t PARITY  	:4;
  	uint16_t PAYLOAD	:8;
  	uint16_t CODE  	:4;   //BCF + _BCF + D9 + D8
}S_SWI_BIF_WORDABSTRACT;

typedef struct _S_SWI_BIF_WORDINVERT
{
  	uint16_t unused 	:15;
  	uint16_t INV  	:1;
  	uint16_t DATA  	:14;  // D9..P0
  	uint16_t BCF  	:2;   // BCF + _BCF
}S_SWI_BIF_WORDINVERT;

typedef union _U_SWI_BIF_WORD
{
  	S_SWI_BIF_WORDBITS      sSWI_BIFBits;
 	S_SWI_BIF_WORDABSTRACT  sSWI_BIFAbstact;
 	S_SWI_BIF_WORDINVERT    sSWI_BIFInvRelevant;
  	ULONG               uwWord;
}U_SWI_BIF_WORD;

typedef struct _S_SWI_BIF_WRITE_REGISTER_PARA
{
	uint16_t 	uwAddress;
	uint8_t 	ubData[256];
	uint8_t 	ubDataSize;
	BOOL 	bAddressIncrementOff;
	BOOL 	bTransactionQuery;
}S_SWI_BIF_WRITE_REGISTER_PARA;

typedef struct _S_SWI_BIF_READ_REGISTER_PARA
{
	uint16_t 	uwAddress;
	uint8_t 	ubData[256];
	E_RBE 	ubRBE;
	E_RBL 	ubRBL;
	BOOL 	bTransactionQuery;
}S_SWI_BIF_READ_REGISTER_PARA;

typedef struct _S_SLS10ERE_PUID	// 80bits
{
  uint8_t uwManuId1;
  uint8_t uwManuId0;
  uint8_t uwId7;
  uint8_t uwId6;
  uint8_t uwId5;
  uint8_t uwId4;
  uint8_t uwId3;
  uint8_t uwId2;
  uint8_t uwId1;
  uint8_t uwId0;
}S_SLS10ERE_PUID;

/*********************** Function Prototypes *************************/
BOOL Sls10ere_SWI_BIF_search_id( uint8_t ub_BitsToSearch, S_SLS10ERE_PUID * stp_DetectedPuid );
BOOL Sls10ere_SWI_BIF_search_ids(uint8_t ub_BitsToSearch, S_SLS10ERE_PUID * stp_DetectedPuid, uint8_t * ubp_DevCnt );

#endif /* _SLS10ERE_SWI_BIF_H_ */

