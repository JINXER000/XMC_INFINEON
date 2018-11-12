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
 * @file   SLS10ERE_crypto.c
 * @date   September, 2012
 * @brief  Crypto library interface to ECC engine.
 *
 */


#include "include/SLS10ERE_math.h"
#include "include/SLS10ERE_crypto.h"
#include "include/SLS10ERE_curve.h"
#include "string.h"

#define ECC_MAX_KEY_LENGTH_BIT			(193u)
#define ECC_KEY_LENGTH_BIT				(163u)
#define ECC_MAC_RESPONSE_LENGTH_BIT		(80u)
#define ECC_MAC_SOURCE_LENGTH_BYTE		(10)
#define ECC_KEY_LENGTH_BYTE 			((ECC_KEY_LENGTH_BIT+8)>>3)
#define ECC_MAX_KEY_LENGTH_BYTE 		((ECC_MAX_KEY_LENGTH_BIT+8)>>3)
#define ECC_MAX_KEY_LENGTH_DWORD		((ECC_MAX_KEY_LENGTH_BYTE+4)>>2)
#define ECC_MAC_RESPONSE_LENGTH_BYTE	(ECC_MAC_RESPONSE_LENGTH_BIT>>3)
#define ECC_MAC_RESPONSE_LENGTH_DOWRD	((ECC_MAC_RESPONSE_LENGTH_BYTE+4)>>2)
#define ECC_MAC_SOURCE_LENGTH_DOWRD	((ECC_MAC_SOURCE_LENGTH_BYTE+4)>>2)
#define ODC_LENGTH_BYTE		        (78u)
#define UID_LENGTH_BYTE		        (10u)
#define ODC_PUBLICKEY_OFFSET	     	(56u)

#ifdef	OPTIGA_FINAL
	#define ODC_S_VALUE_OFFSET		(24u)
	#define ODC_R_VALUE_OFFSET		(24u)
#else
	#define ODC_S_VALUE_OFFSET		(26u)
	#define ODC_R_VALUE_OFFSET		(26u)
#endif

#define HASH_IN		(32u)
#define HASH_OUT	(32u)

/** Data structure for authentication library interface */
static SLS10ERE_AUTH sls10ere_ecc_auth;

/** Sls10ere unique ID */
static uint8_t 	sls10ereUid[UID_LENGTH_BYTE];

/**	Challenge data */
static uint32_t 	sls10ereChallenge[ECC_MAX_KEY_LENGTH_DWORD];
/**	Checkvalue data */
static uint32_t 	sls10ereCheckvalue[ECC_MAX_KEY_LENGTH_DWORD];
/**	Z-Response data */
static uint32_t 	sls10ereZResponse[ECC_MAX_KEY_LENGTH_DWORD];
/**	MAC-Response data */
static uint32_t 	sls10ereMacResponse[ECC_MAC_RESPONSE_LENGTH_DOWRD];
/**	MAC-Source data */
static uint32_t 	sls10ereMacSource[ECC_MAC_SOURCE_LENGTH_DOWRD];
/**	OPTIGA Trust digital certificate */
static uint8_t 	sls10ereOdc[ODC_LENGTH_BYTE];
/** Public key */
static uint32_t	sls10erePublicKey[ECC_MAX_KEY_LENGTH_DWORD];
/** Public key in 163 bits */
static uint32_t	sls10erePublicKey163[ECC_MAX_KEY_LENGTH_DWORD];
/** ECC curve parameter */
static curve_parameter_t *sls10ereECCCurve;
/** ODC curve parameter */
static curve_parameter_t *sls10ereODCCurve;
/** ODC public key */
static eccpoint_t *sls10ereSignaturePubKey;

static dwordvec_t lambda;

/** Implementations for crypto APIs */
static void Sls10ere_unregister(INOUT SLS10ERE_AUTH ** auth);
static enum SLS10ERE_AUTH_RESULT Sls10ere_initialize_crypto(IN SLS10ERE_AUTH * auth, IN uint8_t * certificate);
static void Sls10ere_generate_challenge(IN SLS10ERE_AUTH * auth, OUT uint8_t * challenge);
static void Sls10ere_generate_checkvalue(IN SLS10ERE_AUTH * auth, OUT uint8_t * checkvalue);
static enum SLS10ERE_AUTH_RESULT Sls10ere_authenticate(IN SLS10ERE_AUTH * auth, IN uint8_t * z_response, uint8_t * mac_response);
static enum SLS10ERE_AUTH_RESULT Sls10ere_mac_verify(IN SLS10ERE_AUTH * auth, IN uint8_t * z_response, IN uint8_t * mac_response, IN uint8_t * mac_data, OUT uint8_t * mac_output);

/** Helper routines, to be replaced by customer APIs */
static void Sls10ere_memcpy(uint8_t * dst, uint8_t * src, uint16_t len);
static void Sls10ere_memset(uint8_t * dst, uint8_t val, uint16_t len);
static void Sls10ere_rand(dwordvec_t erg);

/**
 * Register the authentication library and acquire a pointer to the interface.
 *
 * \param[in] uid Slave UID.
 * \return A pointer to the authentication interface.
 */
SLS10ERE_AUTH * Sls10ere_RegisterCrypto(IN uint8_t * uid)
{
	// Initialize the crypto data structure
	sls10ere_ecc_auth.unregister = Sls10ere_unregister;
	sls10ere_ecc_auth.initialize_crypto = Sls10ere_initialize_crypto;
	sls10ere_ecc_auth.generate_challenge = Sls10ere_generate_challenge;
	sls10ere_ecc_auth.generate_checkvalue = Sls10ere_generate_checkvalue;
	sls10ere_ecc_auth.authenticate = Sls10ere_authenticate;
	sls10ere_ecc_auth.mac_verify = Sls10ere_mac_verify;

	// Reset memory
	Sls10ere_memset(sls10ereUid, 0, sizeof(sls10ereUid));
	Sls10ere_memset((uint8_t*)sls10ereChallenge, 0, sizeof(sls10ereChallenge));
	Sls10ere_memset((uint8_t*)sls10ereCheckvalue, 0, sizeof(sls10ereCheckvalue));
	Sls10ere_memset((uint8_t*)sls10ereZResponse, 0, sizeof(sls10ereZResponse));
	Sls10ere_memset((uint8_t*)sls10ereMacResponse, 0, sizeof(sls10ereMacResponse));
	Sls10ere_memset(sls10ereOdc, 0, sizeof(sls10ereOdc));
	Sls10ere_memset((uint8_t*)sls10erePublicKey, 0, sizeof(sls10erePublicKey));

	// Store UID
	if(uid != NULL)
		Sls10ere_memcpy(sls10ereUid, (uint8_t*)uid, UID_LENGTH_BYTE);

	// Select curve parameter
#ifdef OPTIGA_FINAL
	sls10ereECCCurve = (curve_parameter_t*)&curve_163[sls10ereUid[1] & 0x01];
	sls10ereODCCurve = (curve_parameter_t*)&curve_193;
	sls10ereSignaturePubKey = (eccpoint_t*)&eccpntSignaturePubKey[sls10ereUid[1] & 0x01];
#else
	sls10ereECCCurve = (curve_parameter_t*)&curve_163[0];
	sls10ereODCCurve = (curve_parameter_t*)&curve_193;
	sls10ereSignaturePubKey = (eccpoint_t*)&eccpntSignaturePubKey[0];
#endif
	
	return &sls10ere_ecc_auth;
}

/**
 * Release the interface.
 *
 * All resources related to this interface are released.
 * The interface structure is considered freed after this call.
 *
 * \param auth Pointer to the pointer to the interface. Will be set to NULL on return.
 */
static void Sls10ere_unregister(INOUT SLS10ERE_AUTH ** auth)
{
 	return;
}


/**
 * Store the certificate for the crypto.
 *
 * \param[in] auth Pointer to the interface.
 * \param[in] certificate Data read from the Certificate List.
 */

static enum SLS10ERE_AUTH_RESULT Sls10ere_initialize_crypto(IN SLS10ERE_AUTH * auth, IN uint8_t * certificate)
{
	// Store certificate in the local copy
	if(certificate != NULL)
	{
		Sls10ere_memcpy(sls10ereOdc, certificate, ODC_LENGTH_BYTE);
	}

	// Extract public key from ODC
	// 176 bits PK used for ODC
#ifdef OPTIGA_FINAL
	Sls10ere_memcpy((uint8_t*)sls10erePublicKey, certificate+ODC_PUBLICKEY_OFFSET, ECC_KEY_LENGTH_BYTE+1);
#else
	Sls10ere_memcpy((uint8_t*)sls10erePublicKey, certificate+ODC_PUBLICKEY_OFFSET, ECC_KEY_LENGTH_BYTE);
#endif
	// 163 bits PK used for ECC
	Sls10ere_memcpy((uint8_t*)sls10erePublicKey163, certificate+ODC_PUBLICKEY_OFFSET, ECC_KEY_LENGTH_BYTE);

	// Mask off the remining bits to keep it 163 bits
	sls10erePublicKey163[5] &= 0x7; 

	return SLS10ERE_AUTH_RESULT_SUCCESS;
}

/**
 * Generate challenge.
 *
 * \param[in] auth Pointer to the interface.
 * \param[OUT] challenge to be used for generating the response
 */

static void Sls10ere_generate_challenge(IN SLS10ERE_AUTH * auth, OUT uint8_t * challenge)
{
	// Generate the random number
	Sls10ere_rand(lambda);

	// Generate challenge
	generate_challenge(sls10ereChallenge, sls10ereECCCurve, lambda);

	// Copy challenge to the caller
	if(challenge != NULL)
		Sls10ere_memcpy(challenge, (uint8_t*)sls10ereChallenge, ECC_KEY_LENGTH_BYTE);

	return;
}

/**
 * Generate checkvalue.
 *
 * \param[in] auth Pointer to the interface.
 * \param[OUT] challenge to be used for generating the response
 */
static void Sls10ere_generate_checkvalue(IN SLS10ERE_AUTH * auth, OUT uint8_t * checkvalue)
{
	// Generate checkvalue, which is hidden from the caller
	generate_checkvalue(sls10ereCheckvalue, sls10erePublicKey163, sls10ereECCCurve, lambda);

	// Copy challenge to the caller
	if(checkvalue != NULL)
		Sls10ere_memcpy(checkvalue, (uint8_t*)sls10ereCheckvalue, ECC_KEY_LENGTH_BYTE);

	return;
}

/**
 * Perform authentication.
 *
 * \param[in] auth Pointer to the interface.
 * \param[in] z response
 * \param[in] mac response
 */
static enum SLS10ERE_AUTH_RESULT Sls10ere_authenticate(IN SLS10ERE_AUTH * auth, IN uint8_t * z_response, IN uint8_t * mac_response)
{
	BOOL result;
	mac_t sls10ereMacOutput;

	uint32_t 	ubHashOut[HASH_OUT>>2];
	uint32_t 	ubHashIn[HASH_IN>>2];
	signature_t 	sigSignature[1];

	if((z_response == NULL)||(mac_response == NULL))
		return SLS10ERE_AUTH_RESULT_FAILED;

	// Verify ODC
	// Extract R and S value of the ODC
//	Sls10ere_memset((uint8_t*)ubHashOut, 0, sizeof(ubHashOut));
	Sls10ere_memset((uint8_t*)(sigSignature[0].r_value), 0, sizeof(sigSignature[0].r_value));
	Sls10ere_memset((uint8_t*)(sigSignature[0].s_value), 0, sizeof(sigSignature[0].s_value));
	Sls10ere_memcpy((uint8_t*)(sigSignature[0].r_value), sls10ereOdc, ODC_R_VALUE_OFFSET);
#ifdef OPTIGA_FINAL
	Sls10ere_memcpy((uint8_t*)(sigSignature[0].s_value), sls10ereOdc+ODC_S_VALUE_OFFSET+2, ODC_S_VALUE_OFFSET);
#else
	Sls10ere_memcpy((uint8_t*)(sigSignature[0].s_value), sls10ereOdc+ODC_S_VALUE_OFFSET, ODC_S_VALUE_OFFSET);
#endif
	
	// SHA256 of R/S value of the certificate and public key
	// UID to be included in the next release
#ifdef OPTIGA_FINAL
	Sls10ere_memcpy((uint8_t*)ubHashIn, (uint8_t*)sls10erePublicKey, 22);
	Sls10ere_memcpy(((uint8_t*)ubHashIn)+22, sls10ereUid, UID_LENGTH_BYTE);
	sha256((uint8_t*)ubHashOut, (uint8_t*)ubHashIn, HASH_IN);
#else
  	sha256((uint8_t*)ubHashOut, (uint8_t*)sls10erePublicKey, 22);
#endif

	// ECDSA verification
	if( !ECDSA_verify( sigSignature, (uint8_t*)ubHashOut, sls10ereSignaturePubKey, sls10ereODCCurve))
	{
		return SLS10ERE_AUTH_RESULT_FAILED;
	}

	// Copy Z-response and MAC-response from the caller to the local data structure
	Sls10ere_memcpy((uint8_t*)sls10ereZResponse, z_response, ECC_KEY_LENGTH_BYTE);
	Sls10ere_memcpy((uint8_t*)sls10ereMacResponse, mac_response, 10);

	// ECC verification
	result = verify_mac(sls10ereMacResponse, sls10ereZResponse, sls10ereCheckvalue, sls10erePublicKey163, sls10ereECCCurve, sls10ereMacOutput);

	if(result == TRUE)
		return SLS10ERE_AUTH_RESULT_SUCCESS;

	else
		return SLS10ERE_AUTH_RESULT_FAILED;
}

/**
 * Perform MAC verification.
 *
 * \param[in] auth Pointer to the interface.
 * \param[in] z response
 * \param[in] mac response
 */
static enum SLS10ERE_AUTH_RESULT Sls10ere_mac_verify(IN SLS10ERE_AUTH * auth, IN uint8_t * z_response, IN uint8_t * mac_response, IN uint8_t * mac_data, OUT uint8_t * mac_output)
{
	BOOL result;
	mac_t	sls10ereMacOutput;

	if((z_response == NULL)||(mac_response == NULL) || (mac_data == NULL))
		return SLS10ERE_AUTH_RESULT_FAILED;


	// Copy Z-response and MAC-response from the caller to the local data structure
	Sls10ere_memcpy((uint8_t*)sls10ereZResponse, z_response, ECC_KEY_LENGTH_BYTE);
	Sls10ere_memcpy((uint8_t*)sls10ereMacResponse, mac_response, 10);
	Sls10ere_memcpy((uint8_t*)sls10ereMacSource, mac_data, 10);


	// ECC verification
	result = verify_mac(sls10ereMacResponse, sls10ereZResponse, sls10ereCheckvalue, sls10ereMacSource, sls10ereECCCurve, sls10ereMacOutput);
	Sls10ere_memcpy(mac_output, (uint8_t*)sls10ereMacOutput, 10);
	if(result == TRUE)
		return SLS10ERE_AUTH_RESULT_SUCCESS;
	else
		return SLS10ERE_AUTH_RESULT_FAILED;
}

/**
 * Memory copy routine
 * \param[in] destination pointer.
 * \param[in] source pointer
 * \param[in] length in bytes
 */
static void Sls10ere_memcpy(uint8_t * dst, uint8_t * src, uint16_t len)
{
	memcpy(dst, src, len);
}

/**
 * Memory set routine
 * \param[in] destination pointer.
 * \param[in] value to be set
 * \param[in] length in bytes
 */
static void Sls10ere_memset(uint8_t * dst, uint8_t val, uint16_t len)
{
	memset(dst, val, len);
}

/** !!!!!!!!!! W A R N I N G !!!!!!!!!!
  *
  * this implementation calls the system function rand() for random number generation.
  * rand() is not a cryptographically strong random number generator.
  *
  * !!!!! this implementation must be replaced by a program using an unpredictable
  * true or pseudo random number generator with good statistical properties !!!!!
  */

/** generates a 160 bit random number
  * \param[out] erg generated random number
  * \return void
  */
static void Sls10ere_rand(dwordvec_t erg)
{
	uint8_t i;
	for (i = 0; i < ARRAY_LEN(GF2_163)-1; i++)
		erg[i] = (uint32_t)rand();
	while (i < ARRAY_LEN(MAX_DEGREE))
	{
		erg[i] = 0;
		i++;
	}
}

