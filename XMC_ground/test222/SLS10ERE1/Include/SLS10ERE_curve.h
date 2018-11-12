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
 * @file   SLS10ERE_curve.h
 * @date   September, 2012
 * @brief  Definition of the curve parameters for ECC and ECDSA.
 * @last update May 2013
 *
 */
#ifndef SLS10ERE_CURVE_H_
#define SLS10ERE_CURVE_H_

#define OPTIGA_FINAL

/* CURVE #1: */

/** square root of curve parameter b */
const uint32_t coeff_sqrt_b_163_ES[ARRAY_LEN(GF2_163)] =
  {0x3153d194u, 0xca99ad8fu, 0xcecc452du, 0x5cf77251u, 0xab8e0812u, 0x0u};

	/** affine x-coordinate of the base point */
const uint32_t xP_163_ES[ARRAY_LEN(GF2_163)] =
  {0x40f7702cu, 0x6fa72ca5u, 0x2aa84157u, 0x58a0df74u, 0x474a0364u, 0x4u};

const curve_parameter_t curve_163[2] =
{
	{
    /* degree       */ GF2_163,
    /* coeff_a      */ NULL, /* not needed for authentication protocol */
    /* coeff_sqrt_b */ (uint32_t *)coeff_sqrt_b_163_ES,
    /* base_point_x */ (uint32_t *)xP_163_ES,
    /* base_point_y */ NULL, /* not needed for authentication protocol */
    /* order        */ NULL /* not needed for authentication protocol */
	},
	{
    /* degree       */ GF2_163,
    /* coeff_a      */ NULL, /* not needed for authentication protocol */
    /* coeff_sqrt_b */ (uint32_t *)coeff_sqrt_b_163_ES,
    /* base_point_x */ (uint32_t *)xP_163_ES,
    /* base_point_y */ NULL, /* not needed for authentication protocol */
    /* order        */ NULL
	}
}; /* not needed for authentication protocol */


/** affine x-coordinate of the base point */
const uint32_t xP_193[ARRAY_LEN(GF2_193)] =
  {0xd8c0c5e1, 0x79625372, 0xdef4bf61, 0xad6cdf6f, 0xff84a74, 0xf481bc5f, 0x1};

/** affine y-coordinate of the base point */
const uint32_t yP_193[ARRAY_LEN(GF2_193)] =
  {0xf7ce1b05, 0xb3201b6a, 0x1ad17fb0, 0xf3ea9e3a, 0x903712cc, 0x25e399f2, 0x0};

/** curve parameter a */
const uint32_t coeff_a_193[ARRAY_LEN(GF2_193)] =
  {0x11df7b01, 0x98ac8a9, 0x7b4087de, 0x69e171f7, 0x7a989751, 0x17858feb, 0x0};

/** square root of curve parameter b */
const uint32_t coeff_sqrt_b_193[ARRAY_LEN(GF2_193)] =
  {0x52fdfb06, 0xd43f8be7, 0xd24e42e9, 0x139483af, 0xddee67cd, 0xde5fb3d7, 0x1};

/** order of base point 6277101735386680763835789423269548053691575186051040197193 */
const uint32_t order_193[ARRAY_LEN(GF2_193)] =
  {0x920eba49, 0x8f443acc, 0xc7f34a77, 0x0, 0x0, 0x0, 0x1};

const curve_parameter_t curve_193 =
{
    /* degree       */ GF2_193,
    /* coeff_a      */ (uint32_t *)coeff_a_193,
    /* coeff_sqrt_b */ (uint32_t *)coeff_sqrt_b_193,
    /* base_point_x */ (uint32_t *)xP_193,
    /* base_point_y */ (uint32_t *)yP_193,
    /* order        */ (uint32_t *)order_193
};

/** public key for ECDSA */

#ifdef OPTIGA_FINAL
const eccpoint_t eccpntSignaturePubKey[2] = {
	{
		 {0x354D265B, 0x0AF31FB1, 0x493A6120, 0xD0496678, 0xA41654E1, 0x55DCB1A5, 0x0},
		 {0x446BB75A, 0xB1423C04, 0x75C4AE44, 0x40DA7281, 0xAF13FF1C, 0x0A3BB28A, 0x0}
	},
	{
		 {0x354D265B, 0x0AF31FB1, 0x493A6120, 0xD0496678, 0xA41654E1, 0x55DCB1A5, 0x0},
		 {0x446BB75A, 0xB1423C04, 0x75C4AE44, 0x40DA7281, 0xAF13FF1C, 0x0A3BB28A, 0x0}
	}
};

#else
const eccpoint_t eccpntSignaturePubKey[2] = {
	{
		 {0xecc6e969u, 0x0193cfa3u, 0xbac0f412u, 0x877df6dcu, 0x7bc00744u, 0x514b368fu, 0x00000001u},
		 {0x7dd30e74u, 0x13ae8dbau, 0x9540f5f2u, 0x5e56a9a3u, 0x3ff6b027u, 0x476b848cu, 0x00000000u}
	},
	{
		 {0xecc6e969u, 0x0193cfa3u, 0xbac0f412u, 0x877df6dcu, 0x7bc00744u, 0x514b368fu, 0x00000001u},
		 {0x7dd30e74u, 0x13ae8dbau, 0x9540f5f2u, 0x5e56a9a3u, 0x3ff6b027u, 0x476b848cu, 0x00000000u}
	}
};
#endif	// OPTIGA_FINAL

#endif				// SLS10ERE_CURVE_H_
