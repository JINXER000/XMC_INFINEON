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

#ifndef _SLS10ERE_NVM_H_
#define _SLS10ERE_NVM_H_

BOOL Sls10ere_nvm_write_data(BOOL b_VerifyData, BOOL b_UseIrq, uint16_t uw_OfsAddress, uint16_t ub_BytesToProgram, uint8_t * ubp_Data );
BOOL Sls10ere_nvm_read_data( uint16_t uw_OfsAddress, uint16_t uw_BytesToRead, uint8_t * ubp_Data );
BOOL Sls10ere_nvm_read_data_with_mac_verify (uint16_t uw_OfsAddress, uint16_t uw_ByteToRead, uint8_t * ubp_Data, uint8_t * uid );

BOOL Sls10ere_nvm_prog_dev_addr ( uint8_t ub_DevAddr, uint8_t * ubp_MagicNum);
BOOL Sls10ere_nvm_prog_nvm_lock_ofs ( uint16_t uw_NvmLockOfs, uint8_t * ubp_MagicNum);
BOOL Sls10ere_nvm_read_nvm_lock_ofs (uint16_t *puw_NvmLockOfs);
BOOL Sls10ere_nvm_incr_transition_counter ( uint8_t ub_AmountIncr, uint8_t * ubp_MagicNum);
BOOL Sls10ere_nvm_incr_lifespan_counter ( uint8_t ub_AmountIncr, uint8_t * ubp_MagicNum);
BOOL Sls10ere_nvm_wait_wr_done( BOOL b_UseIrq );
#endif /* _SLS10ERE_NVM_H_ */
