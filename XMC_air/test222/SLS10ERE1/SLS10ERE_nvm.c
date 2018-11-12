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

/**
 * @file   SLS10ERE_nvm.c
 * @date   August, 2012
 * @brief  non-volatile memory module
 *
 */

#include "include/SLS10ERE_include.h"
#include "include/SWI_BIF_transactor.h"

/**
 * Write nvm data.
 *
 * \param[in] if true, verify the write operation by readback test.
 * \param[in] if true, use IRQ to detect the write completion.
 * \param[in] starting location of the nvm address.
 * \param[in] number of bytes to write.
 * \param[in] write buffer.
 */
BOOL Sls10ere_nvm_write_data(BOOL b_VerifyData, BOOL b_UseIrq, uint16_t uw_OfsAddress, uint16_t uw_BytesToProgram, uint8_t * ubp_Data )
{
	BOOL bResult;
	uint8_t reg;
	uint16_t uwDataCount;

	uint8_t ubModCount;
	uint8_t ubModCountTemp;
	uint8_t ubData;
	uint16_t uw_OfsAddress_check;

    // set nvm auto-trigger
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg |= DEF_SCTRL_TASK_TRIG_KILLALL;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg |= DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

	// write nvm write offset address
	SWI_BIFTransWriteUint16(0, DEF_NVM_WR_OFS_0, uw_OfsAddress);

   	// write nvm count and nvm data
	ubModCount = uw_BytesToProgram >> 3;

    ubModCountTemp = 0;

	// send the sets of 8 of data
    while (ubModCount)
	{
		// for interrupt
		if (b_UseIrq)
		{
			if(SWI_BIFTransReadUint8(0, DEF_SCTRL_IRQ_EN, &reg)!= SWI_BIFRESULT_NO_ERROR)
				return FALSE;
			reg |= DEF_SCTRL_IRQ_NVM | DEF_SCTRL_IRQ_GLOBAL;
			SWI_BIFTransWriteUint8(0, DEF_SCTRL_IRQ_EN, reg);

			if(SWI_BIFTransReadUint8(0, DEF_SCTRL_IRQ_CLR, &reg)!= SWI_BIFRESULT_NO_ERROR)
				return FALSE;
			reg |= DEF_SCTRL_IRQ_NVM;
			SWI_BIFTransWriteUint8(0, DEF_SCTRL_IRQ_CLR, reg);
		}
		// cnt register
		SWI_BIFTransWriteUint8(0, DEF_NVM_WR_CNT, 8);

		for (uwDataCount=(ubModCountTemp*8); uwDataCount < ((ubModCountTemp+1)*8); uwDataCount++)
		{
			ubData = ubp_Data[uwDataCount];
			SWI_BIFTrans_WD(0, ubData);
		}
		++ubModCountTemp;
		--ubModCount;

		// wait for nvm write to complete
		if (!Sls10ere_nvm_wait_wr_done(b_UseIrq))
		{
		 	return FALSE;
		}
		if(SWI_BIFTransReadUint16(0, DEF_NVM_WR_OFS_0, &uw_OfsAddress_check)!= SWI_BIFRESULT_NO_ERROR)
			return FALSE;

		if(uw_OfsAddress_check != (uw_OfsAddress+(ubModCountTemp*8)))
		{
			return FALSE;
		}
	} // while (ubModCount)

	// for interrupt
	if (b_UseIrq)
	{
		if(SWI_BIFTransReadUint8(0, DEF_SCTRL_IRQ_EN, &reg)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		reg |= DEF_SCTRL_IRQ_NVM | DEF_SCTRL_IRQ_GLOBAL;
		SWI_BIFTransWriteUint8(0, DEF_SCTRL_IRQ_EN, reg);

		if(SWI_BIFTransReadUint8(0, DEF_SCTRL_IRQ_CLR, &reg)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		reg |= DEF_SCTRL_IRQ_NVM;
		SWI_BIFTransWriteUint8(0, DEF_SCTRL_IRQ_CLR, reg);
	}
	if((uw_BytesToProgram % 8)>0)
	{
		// send the rest of the data
		// cnt register
		SWI_BIFTransWriteUint8(0, DEF_NVM_WR_CNT, uw_BytesToProgram % 8);

	//	for (uwDataCount=(ubModCountTemp*8); uwDataCount < ((ubModCountTemp+1)*8); uwDataCount++)
		for (uwDataCount=ubModCountTemp*8; uwDataCount < uw_BytesToProgram; uwDataCount++)
		{
		    ubData = ubp_Data[uwDataCount];
			SWI_BIFTrans_WD(0, ubData);
		}

		if (!Sls10ere_nvm_wait_wr_done(b_UseIrq))
		{
	 		return FALSE;
		}
		if(SWI_BIFTransReadUint16(0, DEF_NVM_WR_OFS_0, &uw_OfsAddress_check)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		if(uw_OfsAddress_check != (uw_OfsAddress+uw_BytesToProgram))
		{
			return FALSE;
		}
	}

   	if( b_VerifyData == TRUE )
  	{
		for( uwDataCount = 0u; uwDataCount < uw_BytesToProgram; uwDataCount++ )
    	{
  			bResult = Sls10ere_nvm_read_data(uw_OfsAddress+uwDataCount, 1, &ubData);
			if( (bResult == FALSE) || (ubData != ubp_Data[uwDataCount]) )
     		{
       			return FALSE;
     		}
		}
  }

	/* unset nvm auto-trigger */
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg &= ~DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);
    return TRUE;
}

/**
 * Read nvm data.
 *
 * \param[in] starting location of the nvm address.
 * \param[in] number of bytes to read.
 * \param[in] read buffer.
 */
BOOL Sls10ere_nvm_read_data( uint16_t uw_OfsAddress, uint16_t uw_BytesToRead, uint8_t * ubp_Data )
{
  	uint16_t uwDdbReg;
  	uint16_t uwDataOfs=0;

	// NVM_READ_BASE is used as ERA, while addr_offset is used as RRA
	if(uw_OfsAddress < 0x200)
	{
		uwDdbReg = DEF_NVM_PTR + uw_OfsAddress;
	}
	else
	{
		return FALSE;
	}

 	if (uw_BytesToRead > 0x100)
	{
		uw_BytesToRead = uw_BytesToRead - 0x100;
 		if(SWI_BIFTransReadMultUint8(0, uwDdbReg, ubp_Data, 0x100)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		uwDdbReg += 0x100;
		uwDataOfs += 0x100;
	}
	if(SWI_BIFTransReadMultUint8(0, uwDdbReg, (ubp_Data+uwDataOfs), uw_BytesToRead)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;

   return TRUE;
}

/**
 * Read nvm data and verify the data with MAC.
 *
 * \param[in] starting location of the nvm address.
 * \param[in] number of bytes to read.
 * \param[in] read buffer.
 */
BOOL Sls10ere_nvm_read_data_with_mac_verify (uint16_t uw_OfsAddress, uint16_t uw_ByteToRead, uint8_t * ubp_Data, uint8_t * uid )
{
	uint8_t i;
	enum SLS10ERE_AUTH_RESULT result;
	uint16_t uwMacReg;
	uint8_t mac_data[10];
	uint8_t ubMACResult[10];
	uint8_t ubMACOutput[10];
	uint8_t ubData[10];

	uint8_t challenge[21];
	uint8_t checkvalue[21];
	uint8_t ubOdcData[77];
	uint8_t z_response[21];
	uint8_t mac_response[10];
	SLS10ERE_AUTH * infineon_auth;
	uint8_t reg;

	// read from nvm, first 10 bytes needed for MAC operation regardless of the actual uw_ByteToRead
    if ( !Sls10ere_nvm_read_data(uw_OfsAddress,10, mac_data) )
		return FALSE;

	// read for complete NVM data
    if ( !Sls10ere_nvm_read_data(uw_OfsAddress,uw_ByteToRead, ubp_Data) )
		return FALSE;


	// Perform ECC to generate the session key
	infineon_auth = Sls10ere_RegisterCrypto(uid);

	if(infineon_auth == NULL)
	{
		return FALSE;
	}

	if(!Sls10ere_read_ODC(ubOdcData))
	{
		return FALSE;
	}

	result = infineon_auth->initialize_crypto(infineon_auth, ubOdcData);
	if(result != SLS10ERE_AUTH_RESULT_SUCCESS)
		return FALSE;

	infineon_auth->generate_challenge(infineon_auth, challenge);
	infineon_auth->generate_checkvalue(infineon_auth, checkvalue);

	if(!Sls10ere_write_challenge_read_response(challenge, sizeof(challenge), z_response, sizeof(z_response), mac_response, sizeof(mac_response), TRUE))
	{
		return FALSE;
	}

	// set nvm mac pointer
	uwMacReg = DEF_MAC_NVM_RD_PTR_0;
	SWI_BIFTransWriteUint16(0, uwMacReg, uw_OfsAddress);

    // set mac read index, index 0 for NVM
	uwMacReg = DEF_MAC_RD_INDEX;
	SWI_BIFTransWriteUint8(0, uwMacReg, 0);

	// enable IRQ
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_CS_IRQ_EN, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg |= DEF_SCTRL_CS_IRQ_MAC;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_CS_IRQ_EN, reg);

	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_CS_IRQ_CLR, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg |= DEF_SCTRL_CS_IRQ_MAC;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_CS_IRQ_CLR, reg);

	// start task trigger
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_CS_TASK_TRIG, DEF_SCTRL_CS_TASK_TRIG_MAC);

	// wait for IRQ
	if(SWI_BIFTrans_EINT(0, 1000) != SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_CS_IRQ_STS, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	if((reg & DEF_SCTRL_CS_IRQ_MAC) == 0)
		return FALSE;

	// read back mac result
	if(SWI_BIFTransReadMultUint8(0, DEF_MAC_DATA4_0, ubData, 10)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;

	// mac result is stored in reverse order
	for(i=0; i<10; i++)
		ubMACResult[i] = ubData[10-i-1];

	// Perform MAC verification
	if (infineon_auth->mac_verify(infineon_auth, z_response, ubMACResult, mac_data, ubMACOutput) == SLS10ERE_AUTH_RESULT_SUCCESS)
		return TRUE;
	else
		return FALSE;
}

/**
 * program device address into NVM manufacturer region.
 *
 * \param[in] device address.
 * \param[in] magic number.
 */
BOOL Sls10ere_nvm_prog_dev_addr ( uint8_t ub_DevAddr, uint8_t * ubp_MagicNum )
{
	BOOL  bResult=TRUE;
	uint8_t ubDataTemp;
	uint8_t reg;

    // set nvm auto-trigger
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg |= DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

	// writes new dev addr to register
	SWI_BIFTransWriteUint8(0, DEF_PROT_DEV_ADR, ub_DevAddr);

	ubDataTemp = 0;
	// configures idx, and writes magic number to trigger
	SWI_BIFTransWriteUint8(0, DEF_NVM_REG_IDX, ubDataTemp);
	SWI_BIFTransWriteMultUint8(0, DEF_NVM_REG_MAGIC, ubp_MagicNum, 10);

	// unset nvm auto-trigger
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg &= ~DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

	if (bResult == FALSE)
	{
	  	return bResult;
	}
    return TRUE;
}

/**
 * read nvm lock ofs into NVM manufacturer region.
 *
 * \param[out] lock ofs.
 */
BOOL Sls10ere_nvm_read_nvm_lock_ofs (uint16_t *puw_NvmLockOfs)
{
	if(SWI_BIFTransReadUint16(0, DEF_NVM_LOCK_OFS_0, puw_NvmLockOfs)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;

    return TRUE;
}

/**
 * program nvm lock ofs into NVM manufacturer region.
 *
 * \param[in] device address.
 * \param[in] magic number.
 */
BOOL Sls10ere_nvm_prog_nvm_lock_ofs ( uint16_t uw_NvmLockOfs, uint8_t * ubp_MagicNum )
{
	BOOL  bResult=TRUE;
	uint8_t reg;

    // set nvm auto-trigger
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg |= DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

	// writes new nmv lock offset value to register
	SWI_BIFTransWriteUint16(0, DEF_NVM_LOCK_OFS_0, uw_NvmLockOfs);

	// configures idx, and writes magic number to trigger
	SWI_BIFTransWriteUint8(0, DEF_NVM_REG_IDX, 1);
	SWI_BIFTransWriteMultUint8(0, DEF_NVM_REG_MAGIC, ubp_MagicNum, 10);

	// wait for nvm write done
	bResult &= Sls10ere_nvm_wait_wr_done( FALSE );

	// unset nvm auto-trigger
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg &= ~DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

	if (bResult == FALSE)
	{
	  	return bResult;
	}
    return TRUE;
}

/**
 * increase transition counter and store it into NVM manufacturer region.
 *
 * \param[in] amount to increase.
 * \param[in] magic number.
 */
BOOL Sls10ere_nvm_incr_transition_counter ( uint8_t ub_AmountIncr, uint8_t * ubp_MagicNum)
{
	BOOL  bResult=TRUE;
	uint16_t uwDataIncr;
	uint8_t reg;

    // set nvm auto-trigger
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg |= DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

	// read current TRNS_CNT
	if(SWI_BIFTransReadUint16(0, DEF_MISC_TRNS_CNT_0, &uwDataIncr)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	uwDataIncr += ub_AmountIncr;

    // write in new incr value
	SWI_BIFTransWriteUint16(0, DEF_MISC_TRNS_CNT_0, uwDataIncr);

	// configures idx, and writes magic number to trigger
	SWI_BIFTransWriteUint8(0, DEF_NVM_REG_IDX, 0x82);
	SWI_BIFTransWriteMultUint8(0, DEF_NVM_REG_MAGIC, ubp_MagicNum, 10);

	//unset nvm auto-trigger
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg &= ~DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

 	if (bResult == FALSE)
	{
	  	return bResult;
	}

    return TRUE;

}

/**
 * increase life span counter and store it into NVM manufacturer region.
 *
 * \param[in] amount to increase.
 * \param[in] magic number.
 */
BOOL Sls10ere_nvm_incr_lifespan_counter ( uint8_t ub_AmountIncr, uint8_t * ubp_MagicNum)
{
	BOOL  bResult=TRUE;
	uint16_t uwDataIncr;
	uint8_t reg;

    // set nvm auto-trigger
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg |= DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

	// read current TRNS_CNT
	if(SWI_BIFTransReadUint16(0, DEF_MISC_LIFE_CNT_0, &uwDataIncr)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	uwDataIncr += ub_AmountIncr;

    // write in new incr value
	SWI_BIFTransWriteUint16(0, DEF_MISC_LIFE_CNT_0, uwDataIncr);

	// configures idx, and writes magic number to trigger
	SWI_BIFTransWriteUint8(0, DEF_NVM_REG_IDX, 0x83);
	SWI_BIFTransWriteMultUint8(0, DEF_NVM_REG_MAGIC, ubp_MagicNum, 10);

	//unset nvm auto-trigger
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_AUTO, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg &= ~DEF_SCTRL_TASK_TRIG_NVM;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_AUTO, reg);

 	if (bResult == FALSE)
	{
	  	return bResult;
	}

    return TRUE;
}

/**
 * wait for NVM operation done.
 *
 * \param[in] if true, use IRQ for completion.
 */
// ** please note that this count: g_culNvmTimeout should be recalibrated
// for different system
BOOL Sls10ere_nvm_wait_wr_done( BOOL b_UseIrq )
{
	uint8_t reg;

	if (b_UseIrq)
	{
		if(SWI_BIFTrans_EINT(0, 1000*100) != SWI_BIFRESULT_NO_ERROR)
		 	return FALSE;

		if(SWI_BIFTransReadUint8(0, DEF_SCTRL_IRQ_STS, &reg)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		if((reg & DEF_SCTRL_IRQ_NVM) == 0)
		 	return FALSE;
	}
	else
	{
		if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_BUSY, &reg)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		while((reg & DEF_SCTRL_IRQ_NVM))
		{
			SWI_BIFDelay(15);
			if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_BUSY, &reg)!=SWI_BIFRESULT_NO_ERROR)
				return FALSE;
		}
	}
	return TRUE;
}



