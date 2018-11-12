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

#ifndef _SLS10ERE_INT_H_
#define _SLS10ERE_INT_H_

/*************************** Include Files ***************************/

/************************* Constant Define ***************************/

/*************************** Type Define ****************************/

/*********************** ENUM, Structure and Union ***********************/

typedef enum _INT_STATE {
   INT_DIS  = 0x00,
   INT_EN   = (!INT_DIS),
} EINT_STATE;

typedef enum _INT_SEL {
	INT_ALL 	= 0x00,
	INT_GLOBAL 	= 0x01,
	INT_NVM_WR	= 0x02,
	INT_ADC_EN	= 0x03,
	INT_AUTH_EN = 0x04,
	INT_MAC_EN	= 0x05,
	INT_NVM_ERR_EN	= 0x06,
	INT_NVM_CORR_EN	= 0x07
} EINT_SEL;

/*********************** Function Prototypes *************************/

BOOL Sls10ere_interrupt_en_disable ( EINT_STATE ub_mode, EINT_SEL ub_sel, BOOL b_EnGlobal );
BOOL Sls10ere_interrupt_wait_by_EINT (void);
BOOL Sls10ere_interrupt_check_int_status (EINT_SEL ub_sel);
BOOL Sls10ere_interrupt_clr (EINT_SEL ub_sel);

#endif /* _SLS10ERE_INT_H_ */
