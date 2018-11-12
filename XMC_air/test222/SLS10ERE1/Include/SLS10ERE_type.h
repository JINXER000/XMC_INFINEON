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

#ifndef _SLS10ERE_TYPE_H_
#define _SLS10ERE_TYPE_H_

#include <stdlib.h>

typedef signed char BYTE;
typedef signed char int8_t;
typedef unsigned char UBYTE;
typedef unsigned char uint8_t;

typedef short WORD;
typedef short int16_t;
typedef unsigned short UWORD;
typedef unsigned short uint16_t;

typedef long LONG;
typedef long int32_t;
typedef unsigned long ULONG;
typedef unsigned long uint32_t;

typedef unsigned char BOOL;

#define  BYTE_MAX (0xFF)
#define UBYTE_MAX (0xFFu)
#define  WORD_MAX (0xFFFF)
#define UWORD_MAX (0xFFFFu)
#define  LONG_MAX (0xFFFFFFFF)
#define ULONG_MAX (0xFFFFFFFFu)
#define  QUAD_MAX (0xFFFFFFFFFFFFFFFF)
#define UQUAD_MAX (0xFFFFFFFFFFFFFFFFu)


typedef UWORD gf2n_t[9];

#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

//*** MACROS ***
#define LOBYTE( uwData )	( (UBYTE) ((uwData) & 0xFFu) )
#define HIBYTE( uwData )	( (UBYTE)(((uwData) >> 8) & 0xFFu) )


#endif  /* _SLS10ERE_TYPE_H_  */
