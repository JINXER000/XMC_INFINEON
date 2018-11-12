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
 * @file   SLS10ERE_auth.c
 * @date   February, 2013
 * @brief  authentication module.
 *
 */

#include "include/SLS10ERE_include.h"
#include "include/SLS10ERE_crypto.h"
#include "include/SWI_BIF_transactor.h"


#define AUTH_REGISTER_LENGTH	26

/**
 * Single function to handle complete ECC operation.
 *
 * \param[in] interrupot or polling mode
 * \param[in] pointer to the UID
 * \return TRUE or FALSE
 */
BOOL Sls10ere_ECC(BOOL b_UseIrq, S_SLS10ERE_PUID *pstDetectedPuid)
{
	uint8_t challenge[21];
	uint8_t checkvalue[21];
	uint8_t ubOdcData[78];
	uint8_t z_response[21];
	uint8_t mac_response[10];
	uint8_t uid[10];
	SLS10ERE_AUTH * infineon_auth;
	enum SLS10ERE_AUTH_RESULT result;

	uid[0] = pstDetectedPuid->uwId0;
	uid[1] = pstDetectedPuid->uwId1;
	uid[2] = pstDetectedPuid->uwId2;
	uid[3] = pstDetectedPuid->uwId3;
	uid[4] = pstDetectedPuid->uwId4;
	uid[5] = pstDetectedPuid->uwId5;
	uid[6] = pstDetectedPuid->uwId6;
	uid[7] = pstDetectedPuid->uwId7;
	uid[8] = pstDetectedPuid->uwManuId0;
	uid[9] = pstDetectedPuid->uwManuId1;

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

	if(!Sls10ere_write_challenge_read_response(challenge, sizeof(challenge), z_response, sizeof(z_response), mac_response, sizeof(mac_response), b_UseIrq))
	{
		return FALSE;
	}

	result = infineon_auth->authenticate(infineon_auth, z_response, mac_response);

	if(result == SLS10ERE_AUTH_RESULT_SUCCESS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * Read ODC routine
 *
 * \param[out] pointer to the ODC data (78 bytes)
 * \return TRUE or FALSE
 */
BOOL Sls10ere_read_ODC (uint8_t* ubOdcData)
{

	uint16_t uwRegAdd = 0;
	uint8_t ubSwapData;
	uint8_t ubCounter;

	// Complete ODC is 78 bytes, stored in reverse order at DEF_AUTH_CERT (max 96 bytes)
	// byte 0 is stored at (DEF_AUTH_CERT+95)
	// byte 1 is stored at (DEF_AUTH_CERT+94)
	// ...
	// byte 78 is stored at (DEF_AUTH_CERT+18)
	// Therefore ODC is stored reversely from (DEF_AUTH_CERT+18) to (DEF_AUTH_CERT+95)
	uwRegAdd = DEF_AUTH_CERT + 18;

	// Burst read: RBE4+RBLE = 16*4+14=78
	if (SWI_BIFTransReadMultUint8(0, uwRegAdd, ubOdcData, 78) != SWI_BIFRESULT_NO_ERROR)
    {
		return FALSE;
	}
    // reverse the bytes
    for (ubCounter=0; ubCounter<(78/2); ubCounter++)
	{
		ubSwapData = ubOdcData[(78-1)-ubCounter];
		ubOdcData[(78-1)-ubCounter] = ubOdcData[ubCounter];
		ubOdcData[ubCounter] = ubSwapData;
	}
	return TRUE;
}

/**
 * Write challenge routine
 *
 * \param[in] pointer to the challenge data
 * \param[in] length of the challenge data
 * \return TRUE or FALSE
 */
BOOL Sls10ere_write_challenge(uint8_t *ubp_challenge, uint8_t ub_length)
{
	uint8_t ubData[AUTH_REGISTER_LENGTH];
	uint8_t i, j;

	// Challenge is stored reversely in AUTH_REGISTER
	// byte 0 is stored at (DEF_AUTH_CHALLENGE_0+25)
	// byte 1 is stored at (DEF_AUTH_CHALLENGE_0+24)

	// byte 21 is stored at (DEF_AUTH_CHALLENGE_0+4)
	// byte 22-25 should be set to 0
	j = 0;

	for(i=0; i<(AUTH_REGISTER_LENGTH - ub_length); i++)
	{
		ubData[j] = 0;
		j++;
	}

	for (i = ub_length; i>0; i--)
	{
		ubData[j] = ubp_challenge[i-1];
		j++;
	}

	if(SWI_BIFTransWriteMultUint8(0, DEF_AUTH_CHALLENGE_0, ubData, AUTH_REGISTER_LENGTH) == SWI_BIFRESULT_NO_ERROR)
		return TRUE;
	else
		return FALSE;
}

/**
 * Read response routine
 *
 * \param[out] pointer to Z response
 * \param[in] length of Z response
 * \param[out] pointer to MAC response
 * \param[in] length of MAC response
 * \return TRUE or FALSE
 */
BOOL Sls10ere_read_response(uint8_t *ubp_zResponse, uint8_t ub_zResponseLength, uint8_t *ubp_macResponse, uint8_t ub_macResponseLength)
{
	uint8_t i;
	uint8_t ubDataResult[AUTH_REGISTER_LENGTH];

  	// Read out response
	// Burst read RBE1+BRLA=16+10=26
    if(SWI_BIFTransReadMultUint8(0, DEF_AUTH_RESPONSE_0, ubDataResult, AUTH_REGISTER_LENGTH) != SWI_BIFRESULT_NO_ERROR)
    {
        return FALSE;
	}

	// Z response is stored reversely at DEF_AUTH_RESPONSE_0
	// byte 0 is stored at (DEF_AUTH_RESPONSE_0+25)
	// byte 1 is stored at (DEF_AUTH_RESPONSE_0+24)
	// ...
	// byte 20 is stored at (DEF_AUTH_RESPONSE_0+5)
	// byte 21-25 should be set to 0
	for(i=0; i<ub_zResponseLength; i++)
		ubp_zResponse[i] = ubDataResult[AUTH_REGISTER_LENGTH-i-1];

	// Read out MAC
	// Burst read SWI_BIF_NO_RBE+BRLA=10
	if(SWI_BIFTransReadMultUint8(0, DEF_MAC + 0x02, ubDataResult, ub_macResponseLength)!= SWI_BIFRESULT_NO_ERROR)
	{
		return FALSE;
	}

	// MAC response is stored reversely at DEF_MAC+0x2
	for(i=0; i<ub_macResponseLength; i++)
		ubp_macResponse[i] = ubDataResult[ub_macResponseLength-i-1];

	return TRUE;
}

/**
 * Write challenge and Read response routine
 *
 * \param[in] pointer to challenge
 * \param[in] length of challenge
 * \param[out] pointer to Z response
 * \param[in] length of Z response
 * \param[out] pointer to MAC response
 * \param[in] length of MAC response
 * \param[in] interrupt or polling mode
 * \return TRUE or FALSE
 */
BOOL Sls10ere_write_challenge_read_response (uint8_t *ubp_challenge, uint8_t ub_length, uint8_t *ubp_zResponse, uint8_t ub_zResponseLength, uint8_t *ubp_macResponse, uint8_t ub_macResponseLength, BOOL b_UseIrq)
{
	uint8_t reg;

	// host send challenge
	Sls10ere_write_challenge(ubp_challenge, ub_length);

    // host configure irq mode
	if (b_UseIrq)
	{
		if(SWI_BIFTransReadUint8(0, DEF_SCTRL_IRQ_EN, &reg)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		reg |= DEF_SCTRL_IRQ_AUTH | DEF_SCTRL_IRQ_GLOBAL;
		SWI_BIFTransWriteUint8(0, DEF_SCTRL_IRQ_EN, reg);

		if(SWI_BIFTransReadUint8(0, DEF_SCTRL_IRQ_CLR, &reg)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		reg |= DEF_SCTRL_IRQ_AUTH;
		SWI_BIFTransWriteUint8(0, DEF_SCTRL_IRQ_CLR, reg);
	}

	// host trigger challenge
	if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_TRIG, &reg)!=SWI_BIFRESULT_NO_ERROR)
		return FALSE;
	reg |= DEF_SCTRL_TASK_TRIG_AUTH;
	SWI_BIFTransWriteUint8(0, DEF_SCTRL_TASK_TRIG, reg);

	// host wait for the completion of ECC
	if (b_UseIrq)
	{
		if(SWI_BIFTrans_EINT(0, 1000*1000) != SWI_BIFRESULT_NO_ERROR)
		 	return FALSE;

		if(SWI_BIFTransReadUint8(0, DEF_SCTRL_IRQ_STS, &reg)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		if((reg & DEF_SCTRL_IRQ_AUTH) == 0)
		 	return FALSE;
	}
	else
	{
		if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_BUSY, &reg)!=SWI_BIFRESULT_NO_ERROR)
			return FALSE;
		while((reg & DEF_SCTRL_IRQ_AUTH))
		{
			SWI_BIFDelay(15);
			if(SWI_BIFTransReadUint8(0, DEF_SCTRL_TASK_BUSY, &reg)!=SWI_BIFRESULT_NO_ERROR)
				return FALSE;
		}
	}

    // host read response value
    if(Sls10ere_read_response(ubp_zResponse, ub_zResponseLength, ubp_macResponse, ub_macResponseLength) == FALSE)
		return FALSE;

	return TRUE;
}
