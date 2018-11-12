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

#ifndef _SLS10ERE_CRYPTO_H_
#define _SLS10ERE_CRYPTO_H_

#include "SLS10ERE_type.h"

/**
 * \file SLS10ERE_auth.h
 *
 * Sls10ere Authentication lirbary interface.
 *
 * \note Memory allocation is restricted. Further details to be decided later.
 * \note Any data that needs to persist between function calls needs to be copied by the library.
 */

#define IN		///< Macro indentifying input arguments.
#define OUT		///< Macro indentifying output arguments.
#define INOUT	///< Macro indentifying arguments that are both input and output.

struct SLS10ERE_AUTH_;
typedef struct SLS10ERE_AUTH_ SLS10ERE_AUTH;

/// Authentication result.
enum SLS10ERE_AUTH_RESULT
{
	SLS10ERE_AUTH_RESULT_FAILED, ///< Authentication failed
	SLS10ERE_AUTH_RESULT_SUCCESS, ///< Authentication succeeded
	SLS10ERE_AUTH_RESULT_SUCCESS_WITHOUT_UID ///< Authentication succeeded, but tying the UID to the certificate is not supported.
};

/**
 * SLS10ERE authentication library interface.
 */
typedef struct SLS10ERE_AUTH_ {

	/**
	 * Unregister the interface.
	 *
	 * \param auth Pointer to the pointer to the interface. Will be set to NULL on return.
	 */
	void (*unregister)(INOUT SLS10ERE_AUTH ** auth);


	/**
	 * Store the certificate of the crypto.
	 *
	 * \param[in] auth Pointer to the interface.
	 * \param[in] certificate Data read from the Certificate List.
	 */
	enum SLS10ERE_AUTH_RESULT (*initialize_crypto)(IN SLS10ERE_AUTH * auth, IN uint8_t * certificate);


	/**
	 * Generate challenge.
         *
	 * \param[in] auth Pointer to the interface.
	 * \param[OUT] challenge to be used for generating the response
	 */
	void (*generate_challenge)(IN SLS10ERE_AUTH * auth, OUT uint8_t * challenge);

	/**
	 * Generate checkvalue.
	 * \param[in] auth Pointer to the interface.
	 * \param[OUT] checkvalue to be used for generating the response
	 */
	void (*generate_checkvalue)(IN SLS10ERE_AUTH * auth, OUT uint8_t * checkvalue);


	/**
	 * Perform authentication.
	 * \param[in] auth Pointer to the interface.
	 * \param[in] z response
	 * \param[in] mac response
	 */
	enum SLS10ERE_AUTH_RESULT (*authenticate)(IN SLS10ERE_AUTH * auth, IN uint8_t * z_response, IN uint8_t * mac_response);


	/**
	 * Perform MAC verification.
	 * \param[in] auth Pointer to the interface.
	 * \param[in] z response
	 * \param[in] mac response
	 * \param[in] mac data
	 */
	enum SLS10ERE_AUTH_RESULT (*mac_verify)(IN SLS10ERE_AUTH * auth, IN uint8_t * z_response, IN uint8_t * mac_response, IN uint8_t * mac_data, OUT uint8_t * mac_output);
} SLS10ERE_AUTH;

/**
 * Initialize theSLS10ERE authentication library and acquire a pointer to the interface.
 *
 * \param[in] uid Slave UID.
 * \return A pointer to the authentication interface, or NULL if manufacturer and product IDs do not match this library.
 */
SLS10ERE_AUTH * Sls10ere_RegisterCrypto(IN uint8_t * uid);

#endif /* _SLS10ERE_CRYPTO_H_ */
