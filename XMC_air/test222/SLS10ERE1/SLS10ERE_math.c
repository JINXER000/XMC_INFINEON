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
 * @file   SLS10ERE_math.c
 * @date   August, 2012
 * @brief  ECC crypto engine
 *
 */
/*************************** Include Files ***************************/
#include "include/SLS10ERE_math.h"

/************************* Global Variables *************************/
/** pointers to parameters of base point */
eccpoint_t actual_base_point[1];   /* coordinates of base point */
uint32_t *actual_base_point_order; /* order of the base point */

/** pointers to actual parameters of the elliptic curve */
uint32_t *actual_coeff_a;      /* curve parameter a */
uint32_t *actual_coeff_sqrt_b; /* square root of curve parameter b */

/** pointers to actual parameters of the finite field */
uint32_t *actual_irred_polynomial;
unsigned int actual_degree;

/** pointers to actual arithmetic functions for GF(2^n) */
static func2_pt actual_dwordvec_l_shift;

func2_pt actual_gf2n_sum, actual_gf2n_square, actual_dwordvec_copy;
func3_pt actual_gf2n_add, actual_gf2n_mul;

/** irreducible polynomials for different field sizes */
static const uint32_t irred_polynomial_163[ARRAY_LEN(GF2_163)] =
  {0xc9, 0x0, 0x0, 0x0, 0x0, 0x8};
static const uint32_t irred_polynomial_193[ARRAY_LEN(GF2_193)] =
  {0x8001, 0x0, 0x0, 0x0, 0x0, 0x0, 0x2};

/**************************** CONSTANT *********************************/
static const uint32_t K[64] = {
  0x428a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1, 0x923f82a4, 0xab1c5ed5,
  0xd807aa98, 0x12835b01, 0x243185be, 0x550c7dc3, 0x72be5d74, 0x80deb1fe, 0x9bdc06a7, 0xc19bf174,
  0xe49b69c1, 0xefbe4786, 0x0fc19dc6, 0x240ca1cc, 0x2de92c6f, 0x4a7484aa, 0x5cb0a9dc, 0x76f988da,
  0x983e5152, 0xa831c66d, 0xb00327c8, 0xbf597fc7, 0xc6e00bf3, 0xd5a79147, 0x06ca6351, 0x14292967,
  0x27b70a85, 0x2e1b2138, 0x4d2c6dfc, 0x53380d13, 0x650a7354, 0x766a0abb, 0x81c2c92e, 0x92722c85,
  0xa2bfe8a1, 0xa81a664b, 0xc24b8b70, 0xc76c51a3, 0xd192e819, 0xd6990624, 0xf40e3585, 0x106aa070,
  0x19a4c116, 0x1e376c08, 0x2748774c, 0x34b0bcb5, 0x391c0cb3, 0x4ed8aa4a, 0x5b9cca4f, 0x682e6ff3,
  0x748f82ee, 0x78a5636f, 0x84c87814, 0x8cc70208, 0x90befffa, 0xa4506ceb, 0xbef9a3f7, 0xc67178f2};

/** precomputed lookup table for faster squaring in GF(2^n) */
static const uint16_t square_tab[256] =
{
  0x0, 0x1, 0x4, 0x5, 0x10, 0x11, 0x14, 0x15,
  0x40, 0x41, 0x44, 0x45, 0x50, 0x51, 0x54, 0x55,
  0x100, 0x101, 0x104, 0x105, 0x110, 0x111, 0x114, 0x115,
  0x140, 0x141, 0x144, 0x145, 0x150, 0x151, 0x154, 0x155,
  0x400, 0x401, 0x404, 0x405, 0x410, 0x411, 0x414, 0x415,
  0x440, 0x441, 0x444, 0x445, 0x450, 0x451, 0x454, 0x455,
  0x500, 0x501, 0x504, 0x505, 0x510, 0x511, 0x514, 0x515,
  0x540, 0x541, 0x544, 0x545, 0x550, 0x551, 0x554, 0x555,
  0x1000, 0x1001, 0x1004, 0x1005, 0x1010, 0x1011, 0x1014, 0x1015,
  0x1040, 0x1041, 0x1044, 0x1045, 0x1050, 0x1051, 0x1054, 0x1055,
  0x1100, 0x1101, 0x1104, 0x1105, 0x1110, 0x1111, 0x1114, 0x1115,
  0x1140, 0x1141, 0x1144, 0x1145, 0x1150, 0x1151, 0x1154, 0x1155,
  0x1400, 0x1401, 0x1404, 0x1405, 0x1410, 0x1411, 0x1414, 0x1415,
  0x1440, 0x1441, 0x1444, 0x1445, 0x1450, 0x1451, 0x1454, 0x1455,
  0x1500, 0x1501, 0x1504, 0x1505, 0x1510, 0x1511, 0x1514, 0x1515,
  0x1540, 0x1541, 0x1544, 0x1545, 0x1550, 0x1551, 0x1554, 0x1555,
  0x4000, 0x4001, 0x4004, 0x4005, 0x4010, 0x4011, 0x4014, 0x4015,
  0x4040, 0x4041, 0x4044, 0x4045, 0x4050, 0x4051, 0x4054, 0x4055,
  0x4100, 0x4101, 0x4104, 0x4105, 0x4110, 0x4111, 0x4114, 0x4115,
  0x4140, 0x4141, 0x4144, 0x4145, 0x4150, 0x4151, 0x4154, 0x4155,
  0x4400, 0x4401, 0x4404, 0x4405, 0x4410, 0x4411, 0x4414, 0x4415,
  0x4440, 0x4441, 0x4444, 0x4445, 0x4450, 0x4451, 0x4454, 0x4455,
  0x4500, 0x4501, 0x4504, 0x4505, 0x4510, 0x4511, 0x4514, 0x4515,
  0x4540, 0x4541, 0x4544, 0x4545, 0x4550, 0x4551, 0x4554, 0x4555,
  0x5000, 0x5001, 0x5004, 0x5005, 0x5010, 0x5011, 0x5014, 0x5015,
  0x5040, 0x5041, 0x5044, 0x5045, 0x5050, 0x5051, 0x5054, 0x5055,
  0x5100, 0x5101, 0x5104, 0x5105, 0x5110, 0x5111, 0x5114, 0x5115,
  0x5140, 0x5141, 0x5144, 0x5145, 0x5150, 0x5151, 0x5154, 0x5155,
  0x5400, 0x5401, 0x5404, 0x5405, 0x5410, 0x5411, 0x5414, 0x5415,
  0x5440, 0x5441, 0x5444, 0x5445, 0x5450, 0x5451, 0x5454, 0x5455,
  0x5500, 0x5501, 0x5504, 0x5505, 0x5510, 0x5511, 0x5514, 0x5515,
  0x5540, 0x5541, 0x5544, 0x5545, 0x5550, 0x5551, 0x5554, 0x5555
};

/** constant field element with value 1*/
static const dwordvec_t one_element = {0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

/** constant field element with value 0*/
static const dwordvec_t zero_element = {0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0};

//static dwordvec_t lambda;


/* ------------------------------------------------------------------------- */
/** challenge generation
  * \param[out] xA challenge, which is the affine x-coordinate
  *             of \a A = \a lambda * \a P
  * \param[in] curve parameters of the elliptic curve
  * \return FALSE if everything is ok, TRUE otherwise
  */
int generate_challenge (dwordvec_t xA, const curve_parameter_t *curve, dwordvec_t lambda)
{
	dwordvec_t A, B, C, D;
    /* initialize finite field and curve parameters */
    ecc_init(curve);
    /* compute challenge */
 //   gf2n_rand(lambda);

    mont_ecc_mul(A, B, C, D, actual_base_point->x_coord, lambda);
    /* compute affine result */
    return gf2n_divide(xA, A, B);
}

/* ------------------------------------------------------------------------- */
/** generate check value
  * \param[out] xC check value for the response, which is the affine x-coordinate
  *             of \a C = \a lambda * \a xT
  * \param[in] xT public key, which is the affine x-coordinate of \a T = \a xiT * \a P
  * \param[in] curve parameters of the elliptic curve
  * \return FALSE if everything is ok, TRUE otherwise
  */
int generate_checkvalue (dwordvec_t xC, const dwordvec_t xT, const curve_parameter_t *curve, dwordvec_t lambda)
{
  	dwordvec_t A, B, C, D;

  	/* initialize finite field and curve parameters */
  	ecc_init(curve);
  	/* compute check value */
  	mont_ecc_mul(A, B, C, D, xT, lambda);
  	/* compute affine result */
  	return gf2n_divide(xC, A, B);
}

/* ------------------------------------------------------------------------- */
/** verify MAC value
  * param[in] mac_value MAC computed by the tag
  * param[in] Z projective z-coordinate of the response of the tag
  * param[in] xC expected response (i.e. check value), which is the affine
  *            x-coordinate of \a C = \a lambda * \a xT
  * param[in] data data to be auhenticated (i.e. counter value)
  * param[in] curve parameters of the elliptic curve
  * return TRUE if mac/response is valid, FALSE otherwise
  */
int verify_mac(const mac_t mac_value, const dwordvec_t Z, const dwordvec_t xC, const mac_t data, const curve_parameter_t *curve, mac_t host_mac_value)
{
  dwordvec_t host_session_key;
//  mac_t host_mac_value;
  uint32_t t;

  /* initialize finite field */
  gf2n_init(curve->degree);

  /* verify MAC */
  if (dwordvec_iszero(Z))
  {
    return FALSE;
  }

  gf2n_mul(host_session_key, xC, Z);


  mac_algorithm_80(host_mac_value, data, host_session_key);


  t = host_mac_value[0] ^ mac_value[0];
  t |= host_mac_value[1] ^ mac_value[1];

  if (actual_degree == GF2_163)
  {
    t |= host_mac_value[2] ^ mac_value[2];
  }

  if (t)
  {
    return FALSE;
  }

  return TRUE;
}


/**************************** Functions ****************************/

/** multiply an element in GF(2^163) by x without reduction
  * param[in] in GF(2^n) element
  * param[out] out GF(2^n) element
  */
void dwordvec_l_shift_163(dwordvec_t out, const dwordvec_t in)
{
  out[5] = in[5]<<1 | in[4]>>31;
  out[4] = in[4]<<1 | in[3]>>31;
  out[3] = in[3]<<1 | in[2]>>31;
  out[2] = in[2]<<1 | in[1]>>31;
  out[1] = in[1]<<1 | in[0]>>31;
  out[0] = in[0]<<1;
}

/** multiply an element in GF(2^193) by x without reduction
  * param[in] in GF(2^n) element
  * param[out] out GF(2^n) element
  */
void dwordvec_l_shift_193(dwordvec_t out, const dwordvec_t in)
{
  out[6] = in[6]<<1 | in[5]>>31;
  out[5] = in[5]<<1 | in[4]>>31;
  out[4] = in[4]<<1 | in[3]>>31;
  out[3] = in[3]<<1 | in[2]>>31;
  out[2] = in[2]<<1 | in[1]>>31;
  out[1] = in[1]<<1 | in[0]>>31;
  out[0] = in[0]<<1;
}

/** addition in the finite field GF(2^163)
  * param[in,out] io output of addition and first operand
  * param[in] op second operand
  */
void gf2_163_sum(dwordvec_t io, const dwordvec_t op)
{
  io[0] ^= op[0];
  io[1] ^= op[1];
  io[2] ^= op[2];
  io[3] ^= op[3];
  io[4] ^= op[4];
  io[5] ^= op[5];
}

/** addition in the finite field GF(2^193)
  * param[in,out] io output of addition and first operand
  * param[in] op second operand
  */
void gf2_193_sum(dwordvec_t io, const dwordvec_t op)
{
  io[0] ^= op[0];
  io[1] ^= op[1];
  io[2] ^= op[2];
  io[3] ^= op[3];
  io[4] ^= op[4];
  io[5] ^= op[5];
  io[6] ^= op[6];
}

/** reduction in the finite field GF(2^163) modulo x^163+x^7+x^6+x^3+1
  * param[out] out reduced field element
  * param[in] temp vector of uint32_t
  */
void gf2_163_reduction(dwordvec_t out, const double_dwordvec_t temp)
{
  int i;
  uint32_t t, t1, t2, t3;
  /* reduction modulo x^163+x^7+x^6+x^3+1 */
  /* x^192 = x^36+x^35+x^32+x^29 */

  t1 = t2 = 0UL;

  for (i = 0; i < DOUBLE_ARRAY_LEN(GF2_163)-ARRAY_LEN(GF2_163); i++)
  {
    t = temp[i+ARRAY_LEN(GF2_163)];
    t1 ^= t<<29; /* x^29 */
    t2 ^= t>>3 ^ t ^ t<<3 ^ t<<4; /* x^29+x^32+x^35+x^36 */
    t3 = t>>29 ^ t>>28; /* x^35+x^36 */
    out[i] = temp[i] ^ t1;
    t1 = t2;
    t2 = t3;
  }

  t2 = temp[ARRAY_LEN(GF2_163)-1] ^ t1;
  t = t2 & 0xfffffff8;
  out[ARRAY_LEN(GF2_163)-1] = t2 & 7UL;
  out[0] ^= t>>3 ^ t ^ t<<3 ^ t<<4; /* 1+x^3+x^6+x^7 */
  out[1] ^= t>>29 ^ t>>28; /* x^6+x^7 */
}

/** reduction in the finite field GF(2^193) modulo x^193+x^15+1
  * param[out] out reduced field element
  * param[in] temp vector of uint32_t
  */
void gf2_193_reduction(dwordvec_t out, const double_dwordvec_t temp)
{
  int i;
  uint32_t t, t1, t2, t3;
  /* reduction modulo x^193+x^15+1 */
  /* x^224 = x^46+x^31 */

  t1 = t2 = 0UL;

  for (i = 0; i < DOUBLE_ARRAY_LEN(GF2_193)-ARRAY_LEN(GF2_193); i++)
  {
    t = temp[i+ARRAY_LEN(GF2_193)];
    t1 ^= t<<31; /* x^31 */
    t2 ^= t>>1 ^ t<<14; /* x^31+x^46 */
    t3 = t>>18; /* x^46 */
    out[i] = temp[i] ^ t1;
    t1 = t2;
    t2 = t3;
  }

  t2 = temp[ARRAY_LEN(GF2_193)-1] ^ t1;
  t = t2 & 0xfffffffe;
  out[ARRAY_LEN(GF2_193)-1] = t2 & 1UL;
  out[0] ^= t>>1 ^ t<<14; /* 1+x^15 */
  out[1] ^= t>>18; /* x^15 */
}

/** squaring in the finite field GF(2^163) modulo x^163+x^7+x^6+x^3+1
  * param[out] out result of squaring
  * param[in] op operand
  */
void gf2_163_square(dwordvec_t out, const dwordvec_t op)
{
  uint32_t t;
  double_dwordvec_t temp;

  /* spreading by table lookup*/
  t = op[0];
  temp[0] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[1] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[1];
  temp[2] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[3] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[2];
  temp[4] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[5] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[3];
  temp[6] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[7] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[4];
  temp[8] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[9] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[5];
  temp[10] = (uint32_t)(square_tab[t]);
  /* reduction */
  gf2_163_reduction(out, temp);
}

/** squaring in the finite field GF(2^193) modulo x^193+x^15+1
  * param[out] out result of squaring
  * param[in] op operand
  */
void gf2_193_square(dwordvec_t out, const dwordvec_t op)
{
  uint32_t t;
  double_dwordvec_t temp;

  /* spreading by table lookup*/
  t = op[0];
  temp[0] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[1] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[1];
  temp[2] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[3] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[2];
  temp[4] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[5] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[3];
  temp[6] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[7] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[4];
  temp[8] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[9] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[5];
  temp[10] = ((uint32_t)(square_tab[t & 0xff]) ^ ((uint32_t)(square_tab[t>>8 & 0xff])<<16));
  temp[11] = ((uint32_t)(square_tab[t>>16 & 0xff]) ^ ((uint32_t)(square_tab[t>>24])<<16));
  t = op[6];
  temp[12] = (uint32_t)(square_tab[t]);
  /* reduction */
  gf2_193_reduction(out, temp);
}

/** copy an element of GF(2^163)
  * param[out] copy copy of element
  * param[in] in element
  */
void dwordvec_163_copy(dwordvec_t copy, const dwordvec_t in)
{
  copy[0] = in[0];
  copy[1] = in[1];
  copy[2] = in[2];
  copy[3] = in[3];
  copy[4] = in[4];
  copy[5] = in[5];
}

/** copy an element of GF(2^193)
  * param[out] copy copy of element
  * param[in] in element
  */
void dwordvec_193_copy(dwordvec_t copy, const dwordvec_t in)
{
  copy[0] = in[0];
  copy[1] = in[1];
  copy[2] = in[2];
  copy[3] = in[3];
  copy[4] = in[4];
  copy[5] = in[5];
  copy[6] = in[6];
}

/** addition in the finite field GF(2^163)
  * param[out] out output of addition
  * param[in] op1 first operand
  * param[in] op2 second operand
  */
void gf2_163_add(dwordvec_t out, const dwordvec_t op1, const dwordvec_t op2)
{
  out[0] = op1[0] ^ op2[0];
  out[1] = op1[1] ^ op2[1];
  out[2] = op1[2] ^ op2[2];
  out[3] = op1[3] ^ op2[3];
  out[4] = op1[4] ^ op2[4];
  out[5] = op1[5] ^ op2[5];
}

/** addition in the finite field GF(2^193)
  * param[out] out output of addition
  * param[in] op1 first operand
  * param[in] op2 second operand
  */
void gf2_193_add(dwordvec_t out, const dwordvec_t op1, const dwordvec_t op2)
{
  out[0] = op1[0] ^ op2[0];
  out[1] = op1[1] ^ op2[1];
  out[2] = op1[2] ^ op2[2];
  out[3] = op1[3] ^ op2[3];
  out[4] = op1[4] ^ op2[4];
  out[5] = op1[5] ^ op2[5];
  out[6] = op1[6] ^ op2[6];
}

/* ------------------------------------------------------------------------- */
/** precomputation for multiplication
  * param[out] table table of precomputed field elements
  * param[in] el field element
  */
void precompute(dwordvec_t table[], const dwordvec_t el)
{
  dwordvec_copy(table[0], zero_element);
  dwordvec_copy(table[1], el);
  dwordvec_l_shift(table[2], table[1]);
  dwordvec_l_shift(table[4], table[2]);
  dwordvec_l_shift(table[8], table[4]);
  gf2n_add(table[3], table[2], table[1]);
  gf2n_add(table[5], table[4], table[1]);
  gf2n_add(table[6], table[4], table[2]);
  gf2n_add(table[7], table[4], table[3]);
  gf2n_add(table[9], table[8], table[1]);
  gf2n_add(table[10], table[8], table[2]);
  gf2n_add(table[11], table[8], table[3]);
  gf2n_add(table[12], table[8], table[4]);
  gf2n_add(table[13], table[8], table[5]);
  gf2n_add(table[14], table[8], table[6]);
  gf2n_add(table[15], table[8], table[7]);
}

/** multiplication in the finite field GF(2^163) modulo x^163+x^7+x^6+x^3+1
  * \param[out] out result of multiplication
  * \param[in] op1 first operand
  * \param[in] op2 second operand
  */
void gf2_163_mul(dwordvec_t out, const dwordvec_t op1, const dwordvec_t op2)
{
  dwordvec_t table[16];
  double_dwordvec_t temp;
  int i, j, t;

  /* precomputation */
  precompute(table, op2);

  for (i = 0; i < DOUBLE_ARRAY_LEN(GF2_163); i++)
  {
    temp[i] = 0;
  }

  /* multiplication */
  for (j = 28; j > 0; j -= 4)
  {
    for (i = 0; i < ARRAY_LEN(GF2_163)-1; i++)
	{
      t = op1[i]>>j & 0xf;
      gf2_163_sum(temp+i, table[t]);
    }

    for (i = DOUBLE_ARRAY_LEN(GF2_163)-1; i > 0; i--)
	{
      temp[i] = temp[i]<<4 | temp[i-1]>>28;
	}

    temp[0] <<= 4;
  }

  for (i = 0; i < ARRAY_LEN(GF2_163); i++)
  {
    t = op1[i] & 0xf;
    gf2_163_sum(temp+i, table[t]);
  }

  /* reduction */
  gf2_163_reduction(out, temp);
}

/** multiplication in the finite field GF(2^193) modulo x^193+x^15+1
  * \param[out] out result of multiplication
  * \param[in] op1 first operand
  * \param[in] op2 second operand
  */
void gf2_193_mul(dwordvec_t out, const dwordvec_t op1, const dwordvec_t op2)
{
  dwordvec_t table[16];
  double_dwordvec_t temp;
  int i, j, t;

  /* precomputation */
  precompute(table, op2);

  for (i = 0; i < DOUBLE_ARRAY_LEN(GF2_193); i++)
  {
    temp[i] = 0;
  }

  /* multiplication */
  for (j = 28; j > 0; j -= 4)
  {
    for (i = 0; i < ARRAY_LEN(GF2_193)-1; i++)
	{
      t = op1[i]>>j & 0xf;
      gf2_193_sum(temp+i, table[t]);
    }

    for (i = DOUBLE_ARRAY_LEN(GF2_193)-1; i > 0; i--)
	{
      temp[i] = temp[i]<<4 | temp[i-1]>>28;
	}

    temp[0] <<= 4;
  }

  for (i = 0; i < ARRAY_LEN(GF2_193); i++)
  {
    t = op1[i] & 0xf;
    gf2_193_sum(temp+i, table[t]);
  }

  /* reduction */
  gf2_193_reduction(out, temp);
}

/* ------------------------------------------------------------------------- */
/** swapping of two finite field elements
  * \param[in,out] el1 finite field element
  * \param[in,out] el2 finite field element
  */
void dwordvec_swap(dwordvec_t el1, dwordvec_t el2)
{
  uint32_t t;

  t = el1[0];
  el1[0] = el2[0];
  el2[0] = t;
  t = el1[1];
  el1[1] = el2[1];
  el2[1] = t;
  t = el1[2];
  el1[2] = el2[2];
  el2[2] = t;
  t = el1[3];
  el1[3] = el2[3];
  el2[3] = t;
  t = el1[4];
  el1[4] = el2[4];
  el2[4] = t;

  if (actual_degree == GF2_131)
  {
	return;
  }

  t = el1[5];
  el1[5] = el2[5];
  el2[5] = t;

  if (actual_degree == GF2_163)
  {
	return;
  }

  t = el1[6];
  el1[6] = el2[6];
  el2[6] = t;
}

/* ------------------------------------------------------------------------- */
/** brief comparison of two uint32 vectors
  *[in] a uint32 vector
  * param[in] b uint32 vector
  * return 1 if \a a > \a b, -1 if \a a < \a b, or 0 if \a a == \a b
  */
int dwordvec_cmp(const dwordvec_t a, const dwordvec_t b)
{
  int i;

  for (i = ARRAY_LEN(actual_degree)-1; i > 0; i--)
  {
    if (a[i] != b[i])
	{
	  break;
	}
  }

  if (a[i] > b[i])
  {
    return 1;
  }

  if (a[i] < b[i])
  {
	return -1;
  }

  return 0;
}

/* ------------------------------------------------------------------------- */
/** right shift of a finite field element
  * param[in,out] el finite field element
  */
void r_shift(dwordvec_t el)
{
  el[0] = el[0]>>1 | el[1]<<31;
  el[1] = el[1]>>1 | el[2]<<31;
  el[2] = el[2]>>1 | el[3]<<31;
  el[3] = el[3]>>1 | el[4]<<31;

  if (actual_degree == GF2_131)
  {
    el[4] >>= 1;
    return;
  }

  el[4] = el[4]>>1 | el[5]<<31;

  if (actual_degree == GF2_163)
  {
    el[5] >>= 1;
    return;
  }

  /* actual_degree == GF2_193 */
  el[5] = el[5]>>1 | el[6]<<31;
  el[6] >>= 1;
}

/* ------------------------------------------------------------------------- */
/** local function is part of gf2n_divide for GF(2^n)
  * param[in,out] a element in GF(2^n)
  * param[in,out] b element in GF(2^n)
  */
void remove_x_power(dwordvec_t a, dwordvec_t b)
{
  while(!(a[0] & 1UL))
  {
    r_shift(a);
    if (b[0] & 1UL) gf2n_sum(b, actual_irred_polynomial);
    r_shift(b);
  }
}

/* ------------------------------------------------------------------------- */
/** check if GF(2^n)-variable is zero
  * param[in] el GF(2^n)-element
  * return TRUE if el == 0, FALSE otherwise
  */
int dwordvec_iszero(const dwordvec_t el)
{
  uint32_t t;

  t = (el[0] | el[1]);
  t |= el[2];
  t |= el[3];
  t |= el[4];

  if (actual_degree != GF2_131)
  {
	t |= el[5];
  }

  if (actual_degree == GF2_193)
  {
	t |= el[6];
  }

  if (t)
  {
    return FALSE;
  }

  return TRUE;
}

/* ------------------------------------------------------------------------- */
/** division in GF(2^n)
  * param[out] t1 result of division
  * param[in] op1 nominator GF(2^n) field element
  * param[in] op2 denominator GF(2^n) field element
  * return TRUE if op2 is 0, FALSE otherwise
  */
BOOL gf2n_divide(dwordvec_t t1, const dwordvec_t op1, const dwordvec_t op2)
{
  dwordvec_t r0, r1, t0;

  /* check division by zero */
  if (dwordvec_iszero(op2))
  {
	return TRUE;//FALSE;
  }

  /* initialization */
  dwordvec_copy(r0, actual_irred_polynomial);
  dwordvec_copy(r1, op2);
  dwordvec_copy(t0, zero_element);
  dwordvec_copy(t1, op1);
  /* main loop */
  remove_x_power(r1, t1);

  while (!dwordvec_iszero(r0))
  {
    remove_x_power(r0, t0);

    if (dwordvec_cmp(r1, r0) > 0)
	{
      dwordvec_swap(r1, r0);
      dwordvec_swap(t1, t0);
    }

    gf2n_sum(r0, r1);
    gf2n_sum(t0, t1);
  }
  return FALSE;//TRUE;
}

/* ------------------------------------------------------------------------- */
/** brief addition of two vectors of uint32_t
  * param[in] a vector of uint32_t
  * param[in] b vector of uint32_t
  * param[out] c vector of uint32_t
  * return none
  */
void dwordvec_add(dwordvec_t c, const dwordvec_t a, const dwordvec_t b)
{
  unsigned int i;
  uint32_t carry, t;

  t = a[0];                        /* safe the least significat limb of a (needed if c == a) */
  c[0] = a[0] + b[0];              /* add least significant limbs */
  carry = c[0] < t;                /* save carry of first addition */

  for (i = 1; i < ARRAY_LEN(actual_degree); i++)
  {
    t = a[i];                      /* save limb of a (needed if c == a) */
    c[i] = b[i] + carry;           /* add carry of preceeding loop to b */
    carry = c[i] < carry;          /* check carry */
    c[i] += t;                     /* add limb of a */
    carry |= c[i] < t;             /* check carry */
  }
}

/* ------------------------------------------------------------------------- */
/** brief subtraction of two vectors of uint32_t
  * param[in] a vector of uint32_t
  * param[in] b vector of uint32_t
  * param[out] c vector of uint32_t
  * return borrow
  */
uint32_t dwordvec_sub(dwordvec_t c, const dwordvec_t a, const dwordvec_t b)
{
  unsigned int i;
  uint32_t borrow, t1, t2;

  t1 = a[0];                       /* safe the least significat limb of a (needed if c == a) */
  c[0] = a[0] - b[0];              /* add least significant limbs */
  borrow = c[0] > t1;              /* save borrow of first subtraction */

  for (i = 1; i < ARRAY_LEN(actual_degree); i++)
  {
    t1 = a[i];                     /* save limb of a (needed if c == a) */
    t2 = b[i];                     /* save limb of b (needed if c == b) */
    c[i] = a[i] - borrow;          /* subtract borrow of preceeding loop from a */
    borrow = c[i] > t1;            /* check borrow */
    t1 = c[i];
    c[i] -= t2;                    /* subtract limb of a */
    borrow |= c[i] > t1;           /* check borrow */
  }

  return borrow;
}

/* ------------------------------------------------------------------------- */
/** \brief local function is part of gfp_divide for GF(p)
  * \param[in,out] a finite field element
  * \param[in,out] b finite field element
  * \return none
  */
void remove_2_power(dwordvec_t a, dwordvec_t b)
{
  while(!(a[0] & 1UL))
  {
    r_shift(a);
    if (b[0] & 1) dwordvec_add(b, b, actual_base_point_order);
    r_shift(b);
  }
}

/* ------------------------------------------------------------------------- */
/** \brief division of two finite field elements in GF(p)
  * \param[in] a finite field element
  * \param[in] b finite field element != 0
  * \param[out] x finite field element a/b mod p
  * \return none
  */
void gfp_divide(dwordvec_t x, const dwordvec_t a, const dwordvec_t b)
{
  uint32_t t;
  dwordvec_t u, v, y;

  /* initialization */
  dwordvec_copy(u, b);
  dwordvec_copy(v, actual_base_point_order);
  dwordvec_copy(x, a);
  dwordvec_copy(y, zero_element);
  /* main loop */
  remove_2_power(u, x); /* make u odd */

  while (!dwordvec_iszero(v))
  {
    remove_2_power(v, y); /* make v odd */

    /* sort u and v */
    if (dwordvec_cmp(u, v) > 0)
	{
      dwordvec_swap(u, v);
      dwordvec_swap(x, y);
    }

    /* subtract v = v-u and y = y-x mod p */
    dwordvec_sub(v, v, u);
    t = dwordvec_sub(y, y, x);
    if (t)
	{
	  dwordvec_add(y, y, actual_base_point_order);
	}
  }
}

/**************************** FUNCTION *********************************/

/** choose finite field GF(2^n) and initialize global variables
  * param[in] degree degree of the field
  */
void gf2n_init(const unsigned int degree)
{

  if (degree == GF2_163)
  {
    actual_degree = GF2_163;
    actual_irred_polynomial = (uint32_t *)irred_polynomial_163;
    actual_dwordvec_l_shift = dwordvec_l_shift_163;
    actual_gf2n_sum = gf2_163_sum;
    actual_gf2n_square = gf2_163_square;
    actual_dwordvec_copy = dwordvec_163_copy;
    actual_gf2n_add = gf2_163_add;
    actual_gf2n_mul = gf2_163_mul;
    return;
  }
  actual_degree = GF2_193;
  actual_irred_polynomial = (uint32_t *)irred_polynomial_193;
  actual_dwordvec_l_shift = dwordvec_l_shift_193;
  actual_gf2n_sum = gf2_193_sum;
  actual_gf2n_square = gf2_193_square;
  actual_dwordvec_copy = dwordvec_193_copy;
  actual_gf2n_add = gf2_193_add;
  actual_gf2n_mul = gf2_193_mul;
}

/** brief choose elliptic curve and initialize global variables
  * param[in] p pointer to structure with parameters
  */
void ecc_init(const curve_parameter_t *p)
{
	gf2n_init(p->degree);
	actual_coeff_a = p->coeff_a;
	actual_coeff_sqrt_b = p->coeff_sqrt_b;
	dwordvec_copy(actual_base_point->x_coord, p->base_point_x);

	if (p->base_point_y)
	{
		dwordvec_copy(actual_base_point->y_coord, p->base_point_y);
	}

	actual_base_point_order = p->order;
}

/** elliptic curve point addition */
void mont_ecc_add(dwordvec_t E, dwordvec_t F, const dwordvec_t A, const dwordvec_t B, const dwordvec_t C,
                         const dwordvec_t D, const dwordvec_t point_x)
{
  dwordvec_t G;

  gf2n_mul(G, A, D);
  gf2n_mul(F, B, C);
  gf2n_mul(E, G, F);
  gf2n_sum(F, G);
  gf2n_square(F, F);
  gf2n_mul(G, point_x, F);
  gf2n_sum(E, G);
}

/** elliptic curve point doubling */
void mont_ecc_double(dwordvec_t E, dwordvec_t F, const dwordvec_t A, const dwordvec_t B)
{
  dwordvec_t C;

  gf2n_square(E, A);
  gf2n_square(F, B);
  gf2n_mul(C, actual_coeff_sqrt_b, F);
  gf2n_mul(F, E, F);
  gf2n_sum(E, C);
  gf2n_square(E, E);
}

/** scalar elliptic curve muliplication
  * param[out] A, B, C, D projective x-coordinates of scalar*G and (scalar+1)*G
  * param[in]  point_x affine x-coordinate of the point G
  * param[in]  scalar scalar
  */
void mont_ecc_mul(dwordvec_t A, dwordvec_t B, dwordvec_t C, dwordvec_t D,
                  const dwordvec_t point_x, const dwordvec_t scalar)
{
  dwordvec_t E, F;
  int i;

  /* initialization of coordinates */
  dwordvec_copy(A, one_element);
  dwordvec_copy(B, zero_element);
  dwordvec_copy(C, point_x);
  dwordvec_copy(D, one_element);

  /* main loop */
  for (i = actual_degree-1; i >= 0; i--)
  {
    if (scalar[i/32] & 1UL<<i%32)
	{
	  break;
	}
  }

  while (i >= 0)
  {
    if (scalar[i/32] & 1UL<<i%32)
	{
      if (i & 0x1)
	  {
        mont_ecc_add(E, F, A, B, C, D, point_x);
        mont_ecc_double(C, D, C, D);
        dwordvec_copy(A, E);
        dwordvec_copy(B, F);
      }
	  else
	  {
        mont_ecc_double(E, F, C, D);
        mont_ecc_add(A, B, A, B, C, D, point_x);
        dwordvec_copy(C, E);
        dwordvec_copy(D, F);
      }
    }
	else
	{
      if (i & 0x1)
	  {
        mont_ecc_add(E, F, A, B, C, D, point_x);
        mont_ecc_double(A, B, A, B);
        dwordvec_copy(C, E);
        dwordvec_copy(D, F);
      }
	  else
	  {
        mont_ecc_double(E, F, A, B);
        mont_ecc_add(C, D, A, B, C, D, point_x);
        dwordvec_copy(A, E);
        dwordvec_copy(B, F);
      }
    }

    i--;
  }
}



/** MAC algorithm for data authentication
  * param[out] mac_value result of MAC computation
  * param[in]  data data to be authenticated
  * param[in]  session_key key to be used for data authentication
  */
void mac_algorithm_80(mac_t mac_value, const mac_t data, const mac_t session_key)
{
  uint32_t a0, a1, a2, b0, b1, b2;
  uint32_t i, t;
  uint32_t x0, x1, x2, x3, y0, y1, y2, y3, k0l, k0h, k1;

  a0 = data[0];
  a1 = data[1];
  a2 = data[2] & 0xffff;
  b0 = session_key[0];
  b1 = session_key[1];
  b2 = session_key[2] & 0xffff;

  for (i = 0; i < 150; i++)
  {
    k0l = (b1<<31 | b0>>1) & 0xffffe000;
    k0h = b1>>1 & 0x7f;
    k1 = b2<<10 | b1>>22;

    b2 = b1>>6 & 0xffff;
    b1 = (b1<<26 | b0>>6) ^ k1>>21 ^ ((i > 49 && i < 101) ? k1>>23 : 0UL) ^ k1>>24;
    b0 = b0<<26 ^ k1<<11 ^ ((i > 49 && i < 101) ? k1<<9 : k1<<2) ^ k1<<8 ^ k1;

    x0 = a1<<19 | a0>>13;
    x1 = a2<<21 | a1>>11;
    x2 = a2<<16 | a1>>16;
    x3 = a2<<12 | a1>>20;

    y0 = ~(((x1 ^ x3) & x0) ^ (~x3 & x1) ^ x2 ^ x3);
    y1 = ((x3 | ~x2) & x0) ^ (((~x3 & x2) ^ x3) & x1) ^ (x3 & x2) ^ x3;
    y2 = (((x2 ^ x3) | ~x1) & x0) ^ (((~x3 & x2) ^ x3) & x1) ^ (~x3 & x2);
    y3 = (~((x3 & x1) ^ x3) & x0) ^ (~(x2 ^ x3) & x1) ^ x2;

    y0 &= 0x03ffffff;
    y1 &= 0x03ffffff;
    y2 &= 0x03ffffff;
    y3 &= 0x03ffffff;

    t = a2<<10 | a1>>22;
    a2 = a1>>6 & 0xffff;
    a1 = (a1<<26 | a0>>6) ^ t>>23;
    a0 = a0<<26 ^ t<<9 ^ t<<4 ^ t<<2 ^ t;

    a0 ^= y0<<5 ^ y1<<11 ^ y2<<13;
    a1 ^= y1>>21 ^ y2>>19 ^ y3<<10;
    a2 ^= y3>>22;

    a0 ^= k0l;
    a1 ^= k0h ^ k1<<10;
    a2 ^= k1>>22;
  }

  mac_value[0] = a0 ^ session_key[0];
  mac_value[1] = a1 ^ session_key[1];
  mac_value[2] = (a2 ^ session_key[2]) & 0xffff;
}


/** \brief check the integrity of an affine point
  * \param[in] p point of an elliptic curve
  * \return TRUE if P is on the elliptic curve, FALSE otherwise
  */
int ecc_point_on_curve(const eccpoint_t *p)
{
  dwordvec_t t1, t2;

  gf2n_square(t1, p->x_coord);              /* x^2 */
  gf2n_add(t2, p->x_coord, actual_coeff_a); /* x+a */
  gf2n_mul(t1, t1, t2);                     /* x^3+a*x^2 */
  gf2n_square(t2, actual_coeff_sqrt_b);     /* b */
  gf2n_sum(t1, t2);                         /* x^3+ax^2+b */
  gf2n_add(t2, p->x_coord, p->y_coord);     /* x*y */
  gf2n_mul(t2, t2, p->y_coord);             /* y^2+xy */
  gf2n_sum(t1, t2);                         /* x^3+ax^2+b+y^2+xy */
  return dwordvec_iszero(t1);
}


/** scalar elliptic curve muliplication with projective result
  * \param[out] X x-coordinate of projective point
  * \param[out] Y y-coordinate of projective point
  * \param[out] Z z-coordinate of projective point
  * \param[in] point_x x-coordinate of affine point
  * \param[in] point_y y-coordinate of affine point
  * \param[in] scalar scalar
  */
void ecc_mul_projective(dwordvec_t X, dwordvec_t Y, dwordvec_t Z, const eccpoint_t *p, const dwordvec_t scalar)
{
  dwordvec_t A, B, C, D, t1, t2, t3, t4;

  mont_ecc_mul(A, B, C, D, p->x_coord, scalar);

  if (dwordvec_iszero(B))
  {
    /* result is point at infinity */
    dwordvec_copy(X, zero_element);
    dwordvec_copy(Y, one_element);
    dwordvec_copy(Z, zero_element);
    return;
  }

  if (dwordvec_iszero(D))
  {
    /* result is -P */
    dwordvec_copy(X, p->x_coord);
    gf2n_add(Y, p->x_coord, p->y_coord);
    dwordvec_copy(Z, one_element);
    return;
  }

  gf2n_mul(t1, B, D);           /* Z1*Z2 */
  gf2n_mul(t2, p->x_coord, B);  /* x*Z1 */
  gf2n_mul(Z, t1, t2);          /* x*Z1^2*Z2 */
  gf2n_mul(t3, p->x_coord, t1); /* x*Z1*Z2 */
  gf2n_mul(X, A, t3);           /* x*X1*Z1*Z2 */
  gf2n_sum(t2, A);              /* X1+x*Z1 */
  gf2n_mul(t3, p->x_coord, D);  /* x*Z2 */
  gf2n_sum(t3, C);              /* X2+x*Z2 */
  gf2n_mul(t3, t3, t2);         /* (X1+x*Z1)*(X2+x*Z2) */
  gf2n_square(t4, p->x_coord);  /* x^2 */
  gf2n_sum(t4, p->y_coord);     /* x^2+y */
  gf2n_mul(t1, t1, t4);         /* (x^2+y)*Z1*Z2 */
  gf2n_sum(t3, t1);             /* (X1+x*Z1)*(X2+x*Z2)+(x^2+y)*Z1*Z2 */
  gf2n_mul(t2, t2, t3);         /* (X1+x*Z1)*((X1+x*Z1)*(X2+x*Z2)+(x^2+y)*Z1*Z2) */
  gf2n_mul(t1, p->y_coord, Z);  /* x*y*Z1^2*Z2 */
  gf2n_add(Y, t1, t2);          /* (X1+x*Z1)*((X1+x*Z1)*(X2+x*Z2)+(x^2+y)*Z1*Z2)+x*y*Z1^2*Z2 */
}

/** \brief compute affine x-coordinate of sum of two projective points
  * \param[out] point_x affine x-coordinate of sum of the points
  * \param[in] X1 projective x-coordinate of first point
  * \param[in] Y1 projective y-coordinate of first point
  * \param[in] Z1 projective z-coordinate of first point
  * \param[in] X2 projective x-coordinate of second point
  * \param[in] Y2 projective y-coordinate of second point
  * \param[in] Z2 projective z-coordinate of second point
  * \return TRUE if sum is point at infinity or an error occurs, FALSE otherwise
  */
int ecc_add_point_x(dwordvec_t point_x, const dwordvec_t X1, const dwordvec_t Y1, const dwordvec_t Z1, const dwordvec_t X2, const dwordvec_t Y2, const dwordvec_t Z2)
{
  dwordvec_t t1, t2, t3, t4, t5;

  /* cases with point at infinity */
  if (dwordvec_iszero(Z1))
  {
    if (dwordvec_iszero(Z2))
    {

      return TRUE;
    }

    gf2n_divide(point_x, X2, Z2);
    return FALSE;
  }

  if (dwordvec_iszero(Z2))
  {
    gf2n_divide(point_x, X1, Z1);
    return FALSE;
  }

  /* compare affine x- and y-coordinates */
  gf2n_mul(t1, Y1, Z2);
  gf2n_mul(t2, Y2, Z1);
  gf2n_sum(t1, t2);     /* Y1*Z2+Y2*Z1 */
  gf2n_mul(t2, X1, Z2);
  gf2n_mul(t3, X2, Z1);
  gf2n_sum(t2, t3);     /* X1*Z2+X2*Z1 */

  if (dwordvec_iszero(t2))
  {
    /* x-ccordinates are equal */
    if (!dwordvec_iszero(t1))
    {
      return TRUE; /* case P + (-P) */
    }

    /* case doubling point */
    gf2n_square(t2, Z1);
    gf2n_square(t3, X1);
    gf2n_mul(t1, t2, actual_coeff_sqrt_b);
    gf2n_sum(t1, t3);
    gf2n_square(t1, t1);   /* (X1^2+coeff_sqrt_b*Z1^2)^2 */
    gf2n_mul(t2, t2, t3);  /* X1^2*Z1^2 */
    return gf2n_divide(point_x, t1, t2); /* TRUE, when doubling point of order 2 */
  }

  gf2n_add(t3, t1, t2);
  gf2n_mul(t3, t3, t1);             /* (Y1*Z2+Y2*Z1)^2+(Y1*Z2+Y2*Z1)*(X1*Z2+X2*Z1) */
  gf2n_square(t4, t2);              /* (X1*Z2+X2*Z1)^2 */
  gf2n_mul(t2, t2, t4);             /* (X1*Z2+X2*Z1)^3 */
  gf2n_mul(t5, actual_coeff_a, t4); /* a*(X1*Z2+X2*Z1)^2 */
  gf2n_sum(t3, t5);                 /* (Y1*Z2+Y2*Z1)^2+(Y1*Z2+Y2*Z1)*(X1*Z2+X2*Z1)+a*(X1*Z2+X2*Z1)^2 */
  gf2n_mul(t5, Z1, Z2);             /* Z1*Z2 */
  gf2n_mul(t3, t3, t5);             /* Z1*Z2*((Y1*Z2+Y2*Z1)^2+(Y1*Z2+Y2*Z1)*(X1*Z2+X2*Z1)+a*(X1*Z2+X2*Z1)^2) */
  gf2n_sum(t3, t2);                 /* Z1*Z2*((Y1*Z2+Y2*Z1)^2+(Y1*Z2+Y2*Z1)*(X1*Z2+X2*Z1)+a*(X1*Z2+X2*Z1)^2)+(X1*Z2+X2*Z1)^3 */
  gf2n_mul(t4, t5, t4);             /* Z1*Z2*(X1*Z2+X2*Z1)^2 */
  gf2n_divide(point_x, t3, t4);
  return FALSE;
}



/** brief perform ecdsa signature verification
  * param[in] sig pointer to signature
  * param[in] hash_val pointer to SHA-256 hash value
  * param[in] pub_key pointer to public ecdsa key
  * param[in] curve parameters of the elliptic curve
  * return TRUE is signature is valid, FALSE otherwise
  */
int ECDSA_verify(const signature_t *sig, const uint8_t *hash_data, const eccpoint_t *pub_key,
                 const curve_parameter_t *curve)
{
  int i;
  dwordvec_t hash, temp;
  dwordvec_t X1, Y1, Z1, X2, Y2, Z2;
  uint32_t t1, t2;
  uint8_t *ptr;

  /* initialize finite field and curve parameters */
  ecc_init(curve);

  /* check input parameters */
  if (dwordvec_iszero(sig->r_value))
  {
	return FALSE;
  }

  if (dwordvec_cmp(sig->r_value, actual_base_point_order) >= 0)
  {
    return FALSE;
  }

  if (dwordvec_iszero(sig->s_value))
  {
    return FALSE;
  }

  if (dwordvec_cmp(sig->s_value, actual_base_point_order) >= 0)
  {
    return FALSE;
  }

  if (!ecc_point_on_curve(pub_key))
  {
    return FALSE;
  }

  if (!ecc_point_on_curve(actual_base_point))
  {
	return FALSE;
  }

  /* convert hash value to dwordvec_t according to ANS X9.62-2005 7.4 Verifying Process */
  ptr = (uint8_t *)hash_data;
  t1 = 0UL;

  for (i = ARRAY_LEN(actual_degree)-1; i >= 0; i--)
  {
    t2 = BIG_U8TO32(ptr);
    hash[i] = t1<<(actual_degree%32) | t2>>(32-actual_degree%32);
    t1 = t2;
    ptr += 4;
  }

  /* reduce hash mod order */
  while (dwordvec_cmp(hash, actual_base_point_order) >= 0)
  {
    dwordvec_sub(hash, hash, actual_base_point_order);
  }

  /* verify signature */
  gfp_divide(temp, hash, sig->s_value);
  ecc_mul_projective(X1, Y1, Z1, actual_base_point, temp);
  gfp_divide(temp, sig->r_value, sig->s_value);
  ecc_mul_projective(X2, Y2, Z2, pub_key, temp);

  if (ecc_add_point_x(temp, X1, Y1, Z1, X2, Y2, Z2))
  {
    return FALSE;
  }

  /* reduce temp mod order */
  while (dwordvec_cmp(temp, actual_base_point_order) >= 0)
  {
    dwordvec_sub(temp, temp, actual_base_point_order);
  }

  /* compare result with sig->r */
  gf2n_sum(temp, sig->r_value);

  return dwordvec_iszero(temp);
}

/*---------------------------------------------------------------------------*/
/** brief sha256 compression function
  * param[in,out] context pointer to sha256 context
  */
void sha256_compress(sha256_context_t *context)
{
  uint32_t a, b, c, d, e, f, g, h, i, t1, t2;
  uint32_t W[16];

  for (i = 0; i < 16; i++)
  {
    W[i] = BIG_U8TO32(&(context->M[i<<2]));
  }

  a = context->H[0];
  b = context->H[1];
  c = context->H[2];
  d = context->H[3];
  e = context->H[4];
  f = context->H[5];
  g = context->H[6];
  h = context->H[7];

  for (i = 0; i < 64; i++)
  {
    if (i > 15)
	{
	  W[i & 15] = LIT_SIG_1(W[(i-2) & 15])+W[(i-7) & 15]+LIT_SIG_0(W[(i-15) & 15])+W[(i-16) & 15];
	}

    t1 = h+BIG_SIG_1(e)+CH(e, f, g)+K[i]+W[i & 15];
    t2 = BIG_SIG_0(a)+MAJ(a, b, c);
    h = g;
    g = f;
    f = e;
    e = d+t1;
    d = c;
    c = b;
    b = a;
    a = t1+t2;
  }

  context->H[0] += a;
  context->H[1] += b;
  context->H[2] += c;
  context->H[3] += d;
  context->H[4] += e;
  context->H[5] += f;
  context->H[6] += g;
  context->H[7] += h;

  context->next = 0;
}

/*---------------------------------------------------------------------------*/
/** brief finalize sha256 hashing
  * param[out] hash_value pointer to buffer
  * param[in,out] context pointer to sha256 context
  */
void sha256_final(uint8_t *hash_value, sha256_context_t *context)
{
  uint8_t i;

  i = context->next;
  context->M[i] = 128;
  i++;

  if (i > 56)
  {
    for (; i < 64; i++)
	{
	  context->M[i] = 0;
	}

    sha256_compress(context);
    i = 0;
  }

  for (; i < 60; i++)
  {
    context->M[i] = 0;
  }

  BIG_U32TO8((context->M)+60, context->length);
  sha256_compress(context);

  for (i = 0; i < 8; i++)
  {
    BIG_U32TO8(hash_value+4*i, context->H[i]);
  }
}

/*---------------------------------------------------------------------------*/
/** brief sha256 update function
  * param[in] input_data pointer to input data
  * param[in] input_length byte length of input data
  * param[in,out] context pointer to sha256 context
  */
void sha256_update(const uint8_t *input_data, const uint32_t input_length, sha256_context_t *context)
{
  uint32_t i;

  for (i = 0; i < input_length; i++)
  {
    context->M[context->next] = input_data[i];
    context->next++;
    if (context->next == 64)
	{
	  sha256_compress(context);
	}
  }
  context->length += (input_length<<3);
}

/*---------------------------------------------------------------------------*/
/** brief sha256 context initialization
  * param[in,out] context pointer to sha256 context
  */
void sha256_init(sha256_context_t *context)
{
  context->length = 0;
  context->next = 0;
  context->H[0] = 0x6a09e667;
  context->H[1] = 0xbb67ae85;
  context->H[2] = 0x3c6ef372;
  context->H[3] = 0xa54ff53a;
  context->H[4] = 0x510e527f;
  context->H[5] = 0x9b05688c;
  context->H[6] = 0x1f83d9ab;
  context->H[7] = 0x5be0cd19;
}

/*---------------------------------------------------------------------------*/
/** brief sha256 hashing in one call
  * param[out] hash_value pointer to hash output
  * param[in] input_data pointer to data to hash
  * param[in] input_length byte length of hash data
  */
void sha256(uint8_t *hash_value, const uint8_t *input_data, const uint32_t input_length)
{
  sha256_context_t ctx;

  sha256_init(&ctx);
  sha256_update(input_data, input_length, &ctx);
  sha256_final(hash_value, &ctx);
}




