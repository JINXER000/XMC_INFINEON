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

#ifndef _SLS10ERE_AUTH_H_
#define _SLS10ERE_AUTH_H_

/*************************** Include Files ***************************/
#include "SLS10ERE.h"

/************************* Constant Define ***************************/


/*********************** Function Prototypes *************************/

BOOL Sls10ere_ECC(BOOL b_UseIrq, S_SLS10ERE_PUID *pstDetectedPuid);
BOOL Sls10ere_read_ODC (uint8_t* ubOdcData);
BOOL Sls10ere_write_challenge(uint8_t *ubp_challenge, uint8_t ub_length);
BOOL Sls10ere_read_response(uint8_t *ubp_zResponse, uint8_t ub_zResponseLength, uint8_t *ubp_macResponse, uint8_t ub_macResponseLength);
BOOL Sls10ere_write_challenge_read_response (uint8_t *ubp_challenge, uint8_t ub_length, uint8_t *ubp_zResponse, uint8_t ub_zResponseLength, uint8_t *ubp_macResponse, uint8_t ub_macResponseLength, BOOL b_UseIrq);


#endif				/* _SLS10ERE_AUTH_H_ */

