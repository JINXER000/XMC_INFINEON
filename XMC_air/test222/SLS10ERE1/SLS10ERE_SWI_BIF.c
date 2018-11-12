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

/*************************** Include Files ***************************/

#include "include/SWI_BIF_transactor.h"
#include "include/SLS10ERE_include.h"


// Used in multiple slave uid search
uint8_t ub_Stack[80];
uint8_t ub_StackPointer = 0;

/********************************************************************
*
* Function Name: SWI_BIF_search_id
* Description: This function used perform search id
*
* \param[in]: number of bits to search
* \param[in]: pointer to holder for uid searched
*
* \return TRUE if no error found
*
********************************************************************/
BOOL Sls10ere_SWI_BIF_search_id ( uint8_t ub_BitsToSearch, S_SLS10ERE_PUID * stp_DetectedPuid )
{
/*************** local variables ***************/
	BOOL bFound_0;
  	BOOL bFound_1;
  	uint8_t ubBitIndex = 0u;
  	uint8_t ubByteIndex;
  	uint8_t * ubpBytes = (uint8_t*)stp_DetectedPuid;

/******************* codes *********************/

  	/* do parameter checks */
  	if( ( ub_BitsToSearch == 0u ) || ( ub_BitsToSearch > 80u ) )
  	{
   		return FALSE;
  	}

	/* start ID search */
	SWI_BIFTrans_DISS(0);

 	/* search loop */
  	for( ; ub_BitsToSearch != 0u; ub_BitsToSearch-- )
  	{
		ubByteIndex = ubBitIndex >> 3u;
    	ubpBytes[ubByteIndex] = ubpBytes[ubByteIndex] << 1u;

		/* take care that all two probes did perform correct */
		SWI_BIFTrans_DIP0(0, &bFound_0);

		SWI_BIFTrans_DIP1(0, &bFound_1);

		/* check result and exit on error! */
    	if( (bFound_1 == FALSE) && (bFound_0 == TRUE) )
    	{
      		ubpBytes[ubByteIndex] &= 0xFEu;
  			SWI_BIFTrans_DIE0(0);
    	}
    	else if( (bFound_1 == TRUE) && (bFound_0 == FALSE) )
    	{
      		ubpBytes[ubByteIndex] |= 0x01u;
 			SWI_BIFTrans_DIE1(0);
    	}
    	else
    	{
      		/* if nothing detected then break! */
      		return FALSE;
    	}

    	/* check next bit */
    	ubBitIndex++;
  	}

	 /* all done */
  	return TRUE;

}

/********************************************************************
*
* Function Name: b_UidSearch_GetDipDoneBit
* Description: This function is to be used in multiple UID search
*
* \param[in]  ub_BitInfo
*			  bit 0: 0: DIP needed	; 1: DIP not needed
*             bit 1: 0: DIE1 not needed;  1: DIE1 needed
*             bit 2: 0: DIE0 not needed;  1: DIE0 needed
*
* \return TRUE if no processing error
********************************************************************/

BOOL b_UidSearch_GetDipDoneBit( uint8_t ub_BitInfo )
{
    if (ub_BitInfo & 0x01)
		return TRUE;
	else
		return FALSE;
}

/********************************************************************
*
* Function Name: b_UidSearch_SetDipDoneBit
* Description: This function is to be used in multiple UID search
*
* \param[out]  ub_BitInfo
*			  bit 0: 0: DIP needed	; 1: DIP not needed
*             bit 1: 0: DIE1 not needed;  1: DIE1 needed
*             bit 2: 0: DIE0 not needed;  1: DIE0 needed
* \param[in]  b_Bit : TRUE: set
*                     FALSE: clear

* \return  -
********************************************************************/
void b_UidSearch_SetDipDoneBit( uint8_t * ub_BitInfo , BOOL b_Bit)
{
    if (b_Bit)
		*ub_BitInfo = (*ub_BitInfo) | 0x01; // set
	else
		*ub_BitInfo = (*ub_BitInfo) & 0xfe; // clear
}

/********************************************************************
*
* Function Name: b_UidSearch_GetDIE0Info
* Description: This function is to be used in multiple UID search
*
* \param[in]  ub_BitInfo
*			  bit 0: 0: DIP needed	; 1: DIP not needed
*             bit 1: 0: DIE1 not needed;  1: DIE1 needed
*             bit 2: 0: DIE0 not needed;  1: DIE0 needed
* \param[in]  b_Bit : TRUE: set
*                     FALSE: clear

* \return  TRUE if DIE0(bit2) is set
********************************************************************/
BOOL b_UidSearch_GetDIE0Info( uint8_t ub_BitInfo )
{
    if (ub_BitInfo & 0x04) return TRUE;
	else return FALSE;
}

/********************************************************************
*
* Function Name: b_UidSearch_SetDIE0Info
* Description: This function is to be used in multiple UID search
*
* \param[out]  ub_BitInfo
*			  bit 0: 0: DIP needed	; 1: DIP not needed
*             bit 1: 0: DIE1 not needed;  1: DIE1 needed
*             bit 2: 0: DIE0 not needed;  1: DIE0 needed
* \param[in]  b_Bit : TRUE: set
*                     FALSE: clear

* \return  -
********************************************************************/
void b_UidSearch_SetDIE0Info( uint8_t * ub_BitInfo , BOOL b_Data)
{
    if (b_Data)
		*ub_BitInfo = (*ub_BitInfo) | 0x04;	 // set
	else
		*ub_BitInfo  = (*ub_BitInfo) & 0xfb; // clear
}

/********************************************************************
*
* Function Name: b_UidSearch_GetDIE1Info
* Description: This function is to be used in multiple UID search
*
* \param[in]  ub_BitInfo
*			  bit 0: 0: DIP needed	; 1: DIP not needed
*             bit 1: 0: DIE1 not needed;  1: DIE1 needed
*             bit 2: 0: DIE0 not needed;  1: DIE0 needed
* \param[in]  b_Bit : TRUE: set
*                     FALSE: clear

* \return  TRUE if DIE1(bit1) is set
********************************************************************/
BOOL b_UidSearch_GetDIE1Info( uint8_t ub_BitInfo )
{
    if (ub_BitInfo & 0x02)
		return TRUE;
	else
		return FALSE;
}

/********************************************************************
*
* Function Name: b_UidSearch_SetDIE1Info
* Description: This function is to be used in multiple UID search
*
* \param[out]  ub_BitInfo
*			  bit 0: 0: DIP needed	; 1: DIP not needed
*             bit 1: 0: DIE1 not needed;  1: DIE1 needed
*             bit 2: 0: DIE0 not needed;  1: DIE0 needed

* \return  -
********************************************************************/
void b_UidSearch_SetDIE1Info( uint8_t * ub_BitInfo )
{
    *ub_BitInfo = (*ub_BitInfo) | 0x02;
}

/********************************************************************
*
* Function Name: b_pop
* Description: This function is to be used in multiple UID search
*
* \param[out]  ub_Data: read out last data in ub_Stack

* \return  TRUE if no error
********************************************************************/
BOOL b_pop(uint8_t * ub_Data)
{
    if(ub_StackPointer == 0 )
		return FALSE;	// stack empty
	*ub_Data = ub_Stack[--ub_StackPointer];
	return TRUE;
}

/********************************************************************
*
* Function Name: b_push
* Description: This function is to be used in multiple UID search
*
* \param[out]  ub_Data: push data at the end of  ub_Stack

* \return  TRUE if no error
********************************************************************/
BOOL b_push( uint8_t ub_Data )
{
    if (ub_StackPointer == 96 ) return FALSE; // stack full
	ub_Stack[ub_StackPointer++] = ub_Data;
	return TRUE;
}

/********************************************************************
*
* Function Name: ub_SizeOfStack
* Description: This function is to be used in multiple UID search
*
* \return  Size of ub_Stack
********************************************************************/
uint8_t ub_SizeOfStack()
{
    return ub_StackPointer;
}

/********************************************************************
*
* Function Name: SWI_BIF_MultiUidSearch
* Description: This function is to be used in multiple UID search
* devAddr is assigned sequentially from 1 to N for each searched device
* \param[out]  stp_DetectedPuid
* \param[out]  ubp_DevCnt: slave amount in system

* \return  TRUE if no error
********************************************************************/
BOOL Sls10ere_SWI_BIF_search_ids(uint8_t ub_BitsToSearch, S_SLS10ERE_PUID * stp_DetectedPuid, uint8_t * ubp_DevCnt )
{
    uint8_t ubBitInfo[80];
	uint8_t ubCurrentIdPtr, ubLastIdPtr;
	uint8_t ubSlaveCnt = 0;
	uint8_t ub_DevAddr = 0;
	BOOL bFound_0;
  	BOOL bFound_1;
	uint8_t ubBitCnt = ub_BitsToSearch;
  	uint8_t ubByteIndex;
	BOOL bSearchDone = FALSE;
	uint8_t i;
  	uint8_t ubpBytes[10];
	uint8_t *ptr=(uint8_t*)stp_DetectedPuid;

	ub_StackPointer = 0;  // clear stack first
	for (i = 0;i < ubBitCnt;i ++)ubBitInfo[i] = 0; // clear all info for all 80 bits
	do
	{
		/* Restore device state */
		if(ubSlaveCnt > 0)
			 SWI_BIFTrans_DIE0(0);

		/* start ID search */
		SWI_BIFTrans_DISS(0);
		ubCurrentIdPtr = ub_BitsToSearch;
		ubBitCnt = ub_BitsToSearch;
		for ( i = 0;i < 10;i ++)ubpBytes[i] = 0;
		for ( ; ubBitCnt > 0; ubBitCnt --)
		{
		    ubCurrentIdPtr --;
			ubByteIndex = 9 - (ubCurrentIdPtr >> 3u);
		    ubpBytes[ubByteIndex] = ubpBytes[ubByteIndex] << 1u;
		    if (b_UidSearch_GetDipDoneBit(ubBitInfo[ubCurrentIdPtr]) == FALSE )	  // need to do DIP
			{
 				SWI_BIFTrans_DIP0(0, &bFound_0);

 				SWI_BIFTrans_DIP1(0, &bFound_1);

				if( (bFound_0 == TRUE) && (bFound_1 == TRUE)) // both DIP0 and DIP1 has positive response
				{
					ubLastIdPtr = ubCurrentIdPtr;
					if( b_push( ubLastIdPtr ) == FALSE)
						return FALSE;
		      		ubpBytes[ubByteIndex] &= 0xFEu;	// DIE0
		  			SWI_BIFTrans_DIE0(0);
					b_UidSearch_SetDIE0Info( &ubBitInfo[ubLastIdPtr] , TRUE);
					b_UidSearch_SetDIE1Info( &ubBitInfo[ubCurrentIdPtr] );
				}
				else if ((bFound_0 == TRUE) && (bFound_1 == FALSE))	  // DIE0
				{
		      		ubpBytes[ubByteIndex] &= 0xFEu;
		  			SWI_BIFTrans_DIE0(0);
					b_UidSearch_SetDIE0Info( &ubBitInfo[ubCurrentIdPtr], TRUE );
				}
				else if ((bFound_0 == FALSE) && (bFound_1 == TRUE)) // DIE1
				{
		      		ubpBytes[ubByteIndex] |= 0x01u;
		  			SWI_BIFTrans_DIE1(0);
					b_UidSearch_SetDIE1Info( &ubBitInfo[ubCurrentIdPtr] );
				}
				else
					return FALSE;
				b_UidSearch_SetDipDoneBit( &ubBitInfo[ubCurrentIdPtr] , TRUE);


			}
			else  // DIPDone == TRUE
			{
			    if (ubCurrentIdPtr == ubLastIdPtr)
				{
					b_pop( &ubCurrentIdPtr );
				}

				if ( b_UidSearch_GetDIE0Info( ubBitInfo[ubCurrentIdPtr] ))
				{
		      		ubpBytes[ubByteIndex] &= 0xFEu;
		  			SWI_BIFTrans_DIE0(0);
				}
				else if (b_UidSearch_GetDIE1Info( ubBitInfo[ubCurrentIdPtr]))
				{
		      		ubpBytes[ubByteIndex] |= 0x01u;
		  			SWI_BIFTrans_DIE1(0);
				}

			}

			if (ubCurrentIdPtr == 0) // clear dip done from last bit index that has two positive response
			{
			    if ( ub_SizeOfStack() == 0)
					bSearchDone = TRUE;
				else
				{
					// refresh ubLastIdPtr
				    b_pop( &ubLastIdPtr );
					b_push( ubLastIdPtr );
				}
				// clear DIP done bit since last id pointer
			    for  (i = 0;i < ubLastIdPtr;i ++)
					b_UidSearch_SetDipDoneBit( &ubBitInfo[i] , FALSE);

				b_UidSearch_SetDIE0Info( &ubBitInfo[ubLastIdPtr] , FALSE);	// clear

			}  // if (ubCurrentIdPtr == 0)
		}   //for ( ; ubBitCnt > 0; ubBitCnt --)
		for(i=0;i<10;i++)
	    	ptr[i] = ubpBytes[i];

		ubSlaveCnt ++;
		ptr += sizeof(S_SLS10ERE_PUID);
		/* Configure device address from 1 to N for each found device */
		ub_DevAddr = ubSlaveCnt+1;
		SWI_BIFTransWriteUint8(0, DEF_PROT_DEV_ADR, ub_DevAddr);
	}while(bSearchDone == FALSE);
	*ubp_DevCnt = ubSlaveCnt;
    return TRUE;
}
