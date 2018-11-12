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

#ifndef _SLS10ERE_H_
#define _SLS10ERE_H_

#include "SLS10ERE_type.h"

//NVM base address
#define DEF_NVM_PTR				0x1000

// protocol register
#define DEF_PROT_DEV_ID_0		(0x0B00 + 0x00)
#define DEF_PROT_DEV_ID_1		(0x0B00 + 0x01)
#define DEF_PROT_DEV_ID_2		(0x0B00 + 0x02)
#define DEF_PROT_DEV_ID_3		(0x0B00 + 0x03)
#define DEF_PROT_DEV_ID_4		(0x0B00 + 0x04)
#define DEF_PROT_DEV_ID_5		(0x0B00 + 0x05)
#define DEF_PROT_DEV_ID_6		(0x0B00 + 0x06)
#define DEF_PROT_DEV_ID_7		(0x0B00 + 0x07)

#define DEF_PROT_DEV_ADR		(0x0A00 + 0x00)
#define DEF_PROT_RSV_0			(0x0A00 + 0x01)
#define DEF_PROT_ERR_CODE		(0x0A00 + 0x02)
#define DEF_PROT_ERR_CNT		(0x0A00 + 0x03)
#define DEF_PROT_WCNT_0			(0x0A00 + 0x04)
#define DEF_PROT_WCNT_1			(0x0A00 + 0x05)

// slave control register
#define DEF_SCTRL_IRQ_EN		(0x0C00 + 0x00)
#define DEF_SCTRL_IRQ_STS		(0x0C00 + 0x01)
#define DEF_SCTRL_IRQ_CLR		(0x0C00 + 0x01)
#define DEF_SCTRL_TASK_TRIG		(0x0C00 + 0x02)
#define DEF_SCTRL_TASK_BUSY		(0x0C00 + 0x02)
#define DEF_SCTRL_TASK_AUTO		(0x0C00 + 0x03)

#define DEF_SCTRL_CS_IRQ_EN		(0x0C00 + 0x28)
#define DEF_SCTRL_CS_IRQ_STS		(0x0C00 + 0x29)
#define DEF_SCTRL_CS_IRQ_CLR		(0x0C00 + 0x29)
#define DEF_SCTRL_CS_TASK_TRIG		(0x0C00 + 0x2A)
#define DEF_SCTRL_CS_TASK_BUSY		(0x0C00 + 0x2A)
#define DEF_SCTRL_CS_TASK_AUTO		(0x0C00 + 0x2B)

// nvm register
#define DEF_NVM_LOCK_OFS_0		(0x0F00 + 0x00)
#define DEF_NVM_LOCK_OFS_1		(0x0F00 + 0x01)
#define DEF_NVM_REG_IDX			(0x0F00 + 0x02)
#define DEF_NVM_REG_FRZ		    (0x0F00 + 0x03)
#define DEF_NVM_REG_STS			(0x0F00 + 0x03)
#define DEF_NVM_RSV_0			(0x0F00 + 0x04)
#define DEF_NVM_REG_MAGIC		(0x0F00 + 0x05)
#define DEF_NVM_WR_OFS_0		(0x0F00 + 0x06)
#define DEF_NVM_WR_OFS_1		(0x0F00 + 0x07)
#define DEF_NVM_WR_CNT			(0x0F00 + 0x08)
#define DEF_NVM_WR_BUF_0		(0x0F00 + 0x09)
#define DEF_NVM_WR_BUF_1		(0x0F00 + 0x0A)
#define DEF_NVM_WR_BUF_2		(0x0F00 + 0x0B)
#define DEF_NVM_WR_BUF_3		(0x0F00 + 0x0C)
#define DEF_NVM_WR_BUF_4		(0x0F00 + 0x0D)
#define DEF_NVM_WR_BUF_5		(0x0F00 + 0x0E)
#define DEF_NVM_WR_BUF_6		(0x0F00 + 0x0F)
#define DEF_NVM_WR_BUF_7		(0x0F00 + 0x10)

// authentication register
#define DEF_AUTH_CHALLENGE_0	(0x1300 + 0x00)
#define DEF_AUTH_CHALLENGE_1	(0x1300 + 0x01)
#define DEF_AUTH_CHALLENGE_2	(0x1300 + 0x02)
#define DEF_AUTH_CHALLENGE_3	(0x1300 + 0x03)
#define DEF_AUTH_CHALLENGE_4	(0x1300 + 0x04)
#define DEF_AUTH_CHALLENGE_5	(0x1300 + 0x05)
#define DEF_AUTH_CHALLENGE_6	(0x1300 + 0x06)
#define DEF_AUTH_CHALLENGE_7	(0x1300 + 0x07)
#define DEF_AUTH_CHALLENGE_8	(0x1300 + 0x08)
#define DEF_AUTH_CHALLENGE_9	(0x1300 + 0x09)
#define DEF_AUTH_CHALLENGE_A	(0x1300 + 0x0A)
#define DEF_AUTH_CHALLENGE_B	(0x1300 + 0x0B)
#define DEF_AUTH_CHALLENGE_C	(0x1300 + 0x0C)
#define DEF_AUTH_CHALLENGE_D	(0x1300 + 0x0D)
#define DEF_AUTH_CHALLENGE_E	(0x1300 + 0x0E)
#define DEF_AUTH_CHALLENGE_F	(0x1300 + 0x0F)
#define DEF_AUTH_CHALLENGE_10	(0x1300 + 0x10)
#define DEF_AUTH_CHALLENGE_11	(0x1300 + 0x11)
#define DEF_AUTH_CHALLENGE_12	(0x1300 + 0x12)
#define DEF_AUTH_CHALLENGE_13	(0x1300 + 0x13)
#define DEF_AUTH_CHALLENGE_14	(0x1300 + 0x14)
#define DEF_AUTH_CHALLENGE_15	(0x1300 + 0x15)
#define DEF_AUTH_CHALLENGE_16	(0x1300 + 0x16)
#define DEF_AUTH_CHALLENGE_17	(0x1300 + 0x17)
#define DEF_AUTH_CHALLENGE_18	(0x1300 + 0x18)
#define DEF_AUTH_CHALLENGE_19	(0x1300 + 0x19)
#define DEF_AUTH_RESPONSE_0		(0x1400 + 0x00)
#define DEF_AUTH_RESPONSE_1		(0x1400 + 0x01)
#define DEF_AUTH_RESPONSE_2		(0x1400 + 0x02)
#define DEF_AUTH_RESPONSE_3		(0x1400 + 0x03)
#define DEF_AUTH_RESPONSE_4		(0x1400 + 0x04)
#define DEF_AUTH_RESPONSE_5		(0x1400 + 0x05)
#define DEF_AUTH_RESPONSE_6		(0x1400 + 0x06)
#define DEF_AUTH_RESPONSE_7		(0x1400 + 0x07)
#define DEF_AUTH_RESPONSE_8		(0x1400 + 0x08)
#define DEF_AUTH_RESPONSE_9		(0x1400 + 0x09)
#define DEF_AUTH_RESPONSE_10	(0x1400 + 0x0A)
#define DEF_AUTH_RESPONSE_11	(0x1400 + 0x0B)
#define DEF_AUTH_RESPONSE_12	(0x1400 + 0x0C)
#define DEF_AUTH_RESPONSE_13	(0x1400 + 0x0D)
#define DEF_AUTH_RESPONSE_14	(0x1400 + 0x0E)
#define DEF_AUTH_RESPONSE_15	(0x1400 + 0x0F)
#define DEF_AUTH_RESPONSE_16	(0x1400 + 0x10)
#define DEF_AUTH_RESPONSE_17	(0x1400 + 0x11)
#define DEF_AUTH_RESPONSE_18	(0x1400 + 0x12)
#define DEF_AUTH_RESPONSE_19	(0x1400 + 0x13)
#define DEF_AUTH_RESPONSE_20	(0x1400 + 0x14)
#define DEF_AUTH_RESPONSE_21	(0x1400 + 0x15)
#define DEF_AUTH_RESPONSE_22	(0x1400 + 0x16)
#define DEF_AUTH_RESPONSE_23	(0x1400 + 0x17)
#define DEF_AUTH_RESPONSE_24	(0x1400 + 0x18)
#define DEF_AUTH_RESPONSE_25	(0x1400 + 0x19)
#define DEF_AUTH_CERT 			(0x1500 + 0x00)

// mac register
#define DEF_MAC			      	(0x1600 + 0x00)
#define DEF_MAC_NVM_RD_PTR_0	(0x1600 + 0x00)
#define DEF_MAC_NVM_RD_PTR_1	(0x1600 + 0x01)
#define DEF_MAC_DATA4_0			(0x1600 + 0x02)
#define DEF_MAC_DATA4_1			(0x1600 + 0x03)
#define DEF_MAC_DATA3_0			(0x1600 + 0x04)
#define DEF_MAC_DATA3_1			(0x1600 + 0x05)
#define DEF_MAC_DATA2_0			(0x1600 + 0x06)
#define DEF_MAC_DATA2_1			(0x1600 + 0x07)
#define DEF_MAC_DATA1_0			(0x1600 + 0x08)
#define DEF_MAC_DATA1_1			(0x1600 + 0x09)
#define DEF_MAC_DATA0_0			(0x1600 + 0x0A)
#define DEF_MAC_DATA0_1			(0x1600 + 0x0B)
#define DEF_MAC_RD_INDEX        (0x1600 + 0x0C)

// misc register
#define DEF_MISC_TRNS_CNT_0		(0x1700 + 0x00)
#define DEF_MISC_TRNS_CNT_1		(0x1700 + 0x01)
#define DEF_MISC_LIFE_CNT_0		(0x1700 + 0x02)
#define DEF_MISC_LIFE_CNT_1		(0x1700 + 0x03)

//Interrupt Bit definitions
#define DEF_SCTRL_IRQ_GLOBAL		(1<<0)
#define DEF_SCTRL_IRQ_NVM			(1<<1)
#define DEF_SCTRL_IRQ_TEMP			(1<<2)
#define DEF_SCTRL_IRQ_AUTH			(1<<3)
#define DEF_SCTRL_CS_IRQ_MAC		(1<<0)
#define DEF_SCTRL_CS_IRQ_NVMERR		(1<<1)
#define DEF_SCTRL_CS_IRQ_NVMCORRERR (1<<2)

#define DEF_SCTRL_TASK_TRIG_KILLALL	(1<<0)
#define DEF_SCTRL_TASK_TRIG_NVM		(1<<1)
#define DEF_SCTRL_TASK_TRIG_TEMP	(1<<2)
#define DEF_SCTRL_TASK_TRIG_AUTH	(1<<3)
#define DEF_SCTRL_CS_TASK_TRIG_MAC	(1<<0)
//#define DEF_SCTRL_CS_TASK_TRIG_MAC		(1<<1)
//#define DEF_SCTRL_CS_TASK_TRIG_TEMP	(1<<2)
//#define DEF_SCTRL_CS_TASK_TRIG_AUTH	(1<<3)
#endif	/* _SLS10ERE_H_ */

