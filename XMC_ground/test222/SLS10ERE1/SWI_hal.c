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

#include "include/SWI_BIF_transactor.h"
#include "include/SLS10ERE_SWI_BIF.h"
#include "../../board.h"

static void SWI_BIFTransSetParityBits(U_SWI_BIF_WORD * word);
static SWI_BIFbool SWI_BIFTransCheckParityBit(U_SWI_BIF_WORD * word);
static SWI_BIFbool SWI_BIFTransCheckBitInversion(U_SWI_BIF_WORD * word);
static void SWI_BIFTransTreatInvertFlag(U_SWI_BIF_WORD * word);
static SWI_BIFbool SWI_BIFTransProcessReceivedWord(U_SWI_BIF_WORD * word);
static void SWI_BIFTransSendWord(SWI_BIFuint8 code, SWI_BIFuint8 data);
static SWI_BIFbool SWI_BIFTransReceiveWord(U_SWI_BIF_WORD * word);

static SWI_BIFuint16 tauHigh;
static SWI_BIFuint16 tauLow;
static SWI_BIFuint16 tauStop;

#define SWI_BIF_ONE_MICROSEC0			17      //0.5us now   
#define SWI_BIF_ONE_MICROSEC			35      //1us now                                            //#define SWI_BIF_ONE_MICROSEC			15
#define SWI_BIF_SLAVE_RESPONSE_TIME		(tauLow*20) 		                //10tau                      //  #define SWI_BIF_SLAVE_RESPONSE_TIME		(SWI_BIF_ONE_MICROSEC*30) 
#define SWI_BIF_SLAVE_POWER_UP_LOW		(SWI_BIF_ONE_MICROSEC*50)	 	   //30--50us              //#define SWI_BIF_SLAVE_POWER_UP_LOW		(SWI_BIF_ONE_MICROSEC*30)	
#define SWI_BIF_SLAVE_POWER_DOWN_LOW	(SWI_BIF_ONE_MICROSEC*2000)	 	// >=2000us              //#define SWI_BIF_SLAVE_POWER_DOWN_LOW	(SWI_BIF_ONE_MICROSEC*200)
#define SWI_BIF_SLAVE_POWER_UP_DELAY	(SWI_BIF_ONE_MICROSEC*2000) // 10ms MAX            //#define SWI_BIF_SLAVE_POWER_UP_DELAY	(SWI_BIF_ONE_MICROSEC*2000) 	// 2ms
#define SWI_BIF_SLAVE_ACTIVATION_DELAY	0			// instant                                     //#define SWI_BIF_SLAVE_ACTIVATION_DELAY	0			// instant


/*g_ulBaudLow 				= 50;		//1 * SWI_DEFAULT_TIMEBASE;	5us
	g_ulBaudHigh 				= 50*3+20;		//3 * SWI_DEFAULT_TIMEBASE; 15us
	g_ulBaudStop				= 50*5;		//5 * SWI_DEFAULT_TIMEBASE; 25us
	g_ulResponseTimeOut 		= 50*20;	//20 * SWI_DEFAULT_TIMEBASE; 100 us

	g_ulBaudPowerUpPulse 		= 450;		// 30us
	g_ulBaudPowerDownTime		= 40000;	// 2ms
	g_ulBaudPowerUpTime			= 15000;	// 1ms
	g_ulBaudResetTime			= 5000;

	g_ulIntTimeoutDefault     	= 300000; 	// 200ms
	g_ulImmediateIntTimeOut   	= 30000; 	// 20ms

	g_culNvmTimeout 			= 500000;
	*/
//...................................................................................
// Getting implemented version of interface.
// This function informs about the interface version implemented. This should match
// the SWI_BIF_HAL_HEADER_VERSION version of this header file.
//...................................................................................
SWI_BIFresult SWI_BIFHALGetVersion(SWI_BIFhandle handle, SWI_BIFuint16 * version)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Getting information string.
// This function populates the "string" pointer with the corresponding constant
//  "stringID".
// "stringID" is one of defined SWI_BIF_STRING_ID_...
// The string reported has length below or equal SWI_BIF_STRING_MAX_LENGTH
//...................................................................................
SWI_BIFresult SWI_BIFHALGetString(SWI_BIFhandle handle, SWI_BIFuint8 stringID, SWI_BIFchar * string)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Getting error code reported by Slave.
// If SWI_BIFresult is SWI_BIFRESULT_ERR_SLAVE_NACK, it means that transaction went well at
// signaling level but the salve was not understanding it correctly. With NACK
// reporting from Slave, an error code is provided which can be retrieved with the
// following function.
//...................................................................................
SWI_BIFresult SWI_BIFHALGetLastError(SWI_BIFhandle handle, SWI_BIFslaveError * error)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Retrieving the signaling layer capabilities
//...................................................................................
SWI_BIFresult SWI_BIFHALGetCapabilities(SWI_BIFhandle handle, SWI_BIFsigCapabilities * capabilities)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Signaling layer initialization.
// Calling this function initializes the signaling layer to a default inactive
// state.
//...................................................................................
SWI_BIFresult SWI_BIFHALInit(SWI_BIFhandle handle)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Signaling layer enabling.
// Calling this function with “enable” at SWI_BIF_FALSE place the signaling layer in
// inactive state (BCL is not driven). If “enable” is SWI_BIF_TRUE, the
// signaling layer Masters the BCL.
//...................................................................................
SWI_BIFresult SWI_BIFHALEnabling(SWI_BIFhandle handle, SWI_BIFbool enable)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// TauSWI_BIF setting.
// This function configures and selects the normal TauSWI_BIF timing.
// The “tauSWI_BIF” parameter is expressed in 100ns unit (2.3µs corresponds to
//  tauSWI_BIF = 23).
//
// The effective TauSWI_BIF configured is reported in field TauSWI_BIF of SWI_BIFsigState.
//...................................................................................
SWI_BIFresult SWI_BIFHALSetTauSWI_BIF(SWI_BIFhandle handle, SWI_BIFuint16 tauSWI_BIF)
{
	tauLow = SWI_BIF_ONE_MICROSEC*10; //tauSWI_BIF/10*16;
	tauHigh = SWI_BIF_ONE_MICROSEC*30; //* tauSWI_BIF;
	tauStop = SWI_BIF_ONE_MICROSEC*50;//* tauSWI_BIF;
	return SWI_BIFRESULT_NO_ERROR;
	/*	tauLow = tauSWI_BIF/10*15;
	tauHigh = 5 * tauSWI_BIF;
	tauStop = 7 * tauSWI_BIF;
	return SWI_BIFRESULT_NO_ERROR;*/
}

//...................................................................................
// Set and select TauSWI_BIF to the most power efficient value supported by the HW.
//
// If LowPowerTauSWI_BIFSupport is supported in the SWI_BIFsigCapabilities, calling the
// SWI_BIFSigSetLowPowerTauSWI_BIF configures the HW to run with the best compromise of
// transmission speed vs power consumption.
//
// Typically, SWI_BIFHALSetLowPowerTauSWI_BIF configures the HW to use a low power clock tree
// which is generally at low frequency. By consequence, TauSWI_BIF would be clamped to a
// long value.
//
// The “tauSWI_BIF” parameter is expressed in 100ns unit (100µs corresponds to
// tauSWI_BIF = 1000). The effective TauSWI_BIF for this mode is reported in field
// LowPowerTauSWI_BIF of SWI_BIFsigState.
//...................................................................................
SWI_BIFresult SWI_BIFHALSetLowPowerTauSWI_BIF(SWI_BIFhandle handle, SWI_BIFuint16 tauSWI_BIF)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Slave selection cache control.
//
// If enable is SWI_BIF_TRUE, Slave selection cache is enabled. In such case, any SDA
// command selecting a Slave which has already been selected with a previous SDA
// command are ignored and not emitted. If enable SWI_BIF_FALSE, all SDA commands are
// emitted even if they select again same Slave.
//
// The known current selected Slave and state of selection cache are mentioned inside
//  the SWI_BIFsigState structure reported with SWI_BIFHALGetState function.
//...................................................................................
SWI_BIFresult SWI_BIFHALSlaveSelectionCache(SWI_BIFhandle handle, SWI_BIFbool enable)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Get state of signaling layer.
// The function set-up the data pointed by “state” pointer.
//...................................................................................
SWI_BIFresult SWI_BIFHALGetState(SWI_BIFhandle handle, SWI_BIFsigState * state)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Generic access point to vendor specific signaling layer functionalities
//...................................................................................
SWI_BIFuint16 SWI_BIFHALVendorSpecific(SWI_BIFhandle handle, SWI_BIFuint32 command, void * data_in,
	void * data_out)
{
	return SWI_BIFRESULT_NO_ERROR;
}


//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// Transaction Elements layer function declaration
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------


//...................................................................................
// Sending command BRES (Bus Reset)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_BRES(SWI_BIFhandle handle)
{
	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_BRES);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Driving BCL low for tPDL (Hard Reset)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_HardRST(SWI_BIFhandle handle)
{
	set_pin_dir(1);
    set_pin(0);
	SWI_BIFDelay(SWI_BIF_SLAVE_POWER_DOWN_LOW);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command PWDN (Power down)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_PWDN(SWI_BIFhandle handle)
{
	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_PDWN);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command STBY (standby)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_STBY(SWI_BIFhandle handle)
{
	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_STBY);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Produces an activation signal only
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WakeUpPulse(SWI_BIFhandle handle)
{
	set_pin_dir(1);
	set_pin(0);
	SWI_BIFDelay(SWI_BIF_SLAVE_POWER_UP_LOW);
	set_pin(1);
	set_pin_dir(0);
	SWI_BIFDelay(SWI_BIF_SLAVE_ACTIVATION_DELAY);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Produces an activation signal and wait for tPUP delays in order to go out of
// power-down
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WakeUpFromPWDN(SWI_BIFhandle handle)
{
	set_pin_dir(1);
	set_pin(1);
	set_pin_dir(0);
	SWI_BIFDelay(SWI_BIF_SLAVE_POWER_UP_DELAY);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Produces an activation signal and wait for tACT delays in order to go out of
// standby mode
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WakeUpFromSTBY(SWI_BIFhandle handle)
{
	set_pin_dir(1);
	set_pin(0);
	SWI_BIFDelay(SWI_BIF_SLAVE_POWER_UP_LOW);
	set_pin(1);
	set_pin_dir(0);
	SWI_BIFDelay(SWI_BIF_SLAVE_ACTIVATION_DELAY);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command ISTS (interrupt status).
// The function reports SWI_BIF_TRUE if Slave reports interrupt pending else it report
// SWI_BIF_FALSE.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_ISTS(SWI_BIFhandle handle, SWI_BIFbool * intStatus)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command RBL (read burst length) with the command parameter
//...................................................................................
SWI_BIFresult SWI_BIFTrans_RBL(SWI_BIFhandle handle, SWI_BIFuint8 length)
{
	SWI_BIFTransSendWord(SWI_BIF_BC, length+0x20);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command RBE (read burst extended) with the command parameter
//...................................................................................
SWI_BIFresult SWI_BIFTrans_RBE(SWI_BIFhandle handle, SWI_BIFuint8 length)
{
	SWI_BIFTransSendWord(SWI_BIF_BC, length+0x30);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command DASM (device multiple select)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DASM(SWI_BIFhandle handle)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command DISS
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DISS(SWI_BIFhandle handle)
{
	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_DISS);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command DILC.
// Report the DILC result.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DILC(SWI_BIFhandle handle, SWI_BIFbool * dilc)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command DIE0
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DIE0(SWI_BIFhandle handle)
{
	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_DIE0);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command DIE1
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DIE1(SWI_BIFhandle handle)
{
	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_DIE1);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command DIP0.
// Reports SWI_BIF_TRUE if a Slave responds to probing else SWI_BIF_FALSE.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DIP0(SWI_BIFhandle handle, SWI_BIFbool * dip0)
{
	SWI_BIFuint32 timeout = tauLow * 30;

	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_DIP0);
	while(timeout--)
	{
		if(get_pin() == 0)
		{
			*dip0 = SWI_BIF_TRUE;
			SWI_BIFDelay(SWI_BIF_SLAVE_RESPONSE_TIME*10);
			return SWI_BIFRESULT_NO_ERROR;
		}
		SWI_BIFDelay(SWI_BIF_ONE_MICROSEC);
	}
	*dip0 = SWI_BIF_FALSE;
	SWI_BIFTrans_Abort_EINT(handle);
	SWI_BIFDelay(SWI_BIF_SLAVE_RESPONSE_TIME);
	return SWI_BIFRESULT_TIME_OUT;
}

//...................................................................................
// Sending command DIP1.
// Reports SWI_BIF_TRUE if a Slave responds to probing else SWI_BIF_FALSE.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DIP1(SWI_BIFhandle handle, SWI_BIFbool * dip1)
{
	SWI_BIFuint32 timeout = tauLow * 30;

	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_DIP1);
	while(timeout--)
	{
		if(get_pin() == 0)
		{
			*dip1 = SWI_BIF_TRUE;
			SWI_BIFDelay(SWI_BIF_SLAVE_RESPONSE_TIME*10);
			return SWI_BIFRESULT_NO_ERROR;
		}
		SWI_BIFDelay(SWI_BIF_ONE_MICROSEC);
	}
	*dip1 = SWI_BIF_FALSE;
	SWI_BIFTrans_Abort_EINT(handle);
	SWI_BIFDelay(SWI_BIF_SLAVE_RESPONSE_TIME);
	return SWI_BIFRESULT_TIME_OUT;
}

//...................................................................................
// Sending command DRES (device reset)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DRES(SWI_BIFhandle handle)
{

	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command TQ (Transaction query).
// If no error, reports transaction “dataCount” value (“errorCode” will be
// SWI_BIFSLAVEERROR_NO_ERROR).
// If error, returns “errorCode” from Slave (“dataCount” will be 0).
//...................................................................................
SWI_BIFresult SWI_BIFTrans_TQ(SWI_BIFhandle handle, SWI_BIFslaveError * errorCode,
	SWI_BIFuint8 * dataCount)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command AIO
//...................................................................................
SWI_BIFresult SWI_BIFTrans_AIO(SWI_BIFhandle handle)
{
	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_AIO);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command EDA.
// Function takes the EDA command parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_EDA(SWI_BIFhandle handle, SWI_BIFuint8 add)
{
	SWI_BIFTransSendWord(SWI_BIF_EDA, add);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command SDA (Slave select by address).
// Function takes the Slave address as parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_SDA(SWI_BIFhandle handle, SWI_BIFuint8 add)
{
	SWI_BIFTransSendWord(SWI_BIF_SDA, add);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command ERA.
// Function takes the command parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_ERA(SWI_BIFhandle handle, SWI_BIFuint8 add)
{
	SWI_BIFTransSendWord(SWI_BIF_ERA, add);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command WRA.
// Function takes the command parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WRA(SWI_BIFhandle handle, SWI_BIFuint8 add)
{
	SWI_BIFTransSendWord(SWI_BIF_WRA, add);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command WD.
// Function takes the command parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WD(SWI_BIFhandle handle, SWI_BIFuint8 data)
{
	SWI_BIFTransSendWord(SWI_BIF_WD, data);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command RRA.
// read data are placed in data array with specified length
//
// * It is not recommended to use RRA directly as data count transmitted by Slave	*
// * depends directly on previous command sent to it. Prefer using high-level read	*
// * functions instead.									*
//
//...................................................................................
SWI_BIFresult SWI_BIFTrans_RRA(SWI_BIFhandle handle, SWI_BIFuint8 add, SWI_BIFuint8 * data,
	SWI_BIFuint8 length)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending command EINT (enable interrupt mode) and wait for completion.
//
// Completion can be:
// - interrupt signalled by Slave (function returns SWI_BIFRESULT_NO_ERROR)
// - time-out (function returns SWI_BIFRESULT_TIME_OUT)
// - abort (function returns SWI_BIFRESULT_ABORTED)
// - BCL contact broken if such detection is supported by Master (function returns
//	SWI_BIFRESULT_CONTACT_BROKEN)
//
// Function takes timeOut data as parameter. timeOut is expressed in ms. If timeout
// ellapses before interrupt signalled by Slave, the function returns
// SWI_BIFRESULT_TIME_OUT as SWI_BIFresult.
//
// !! In case of time-out, the function aborts the interrupt mode on SWI_BIF bus so that
// !! any command can be send to the Slave
//
// The interrupt mode can be aborted by the SWI_BIFTrans_Abort_EINT function called from
// another thread. In such case, the function returns SWI_BIFRESULT_ABORTED.
//
// If timeOut is 0, the function waits forever (infinite time-out)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_EINT(SWI_BIFhandle handle, SWI_BIFuint32 timeOut)
{
	SWI_BIFuint32 timeout = timeOut;

	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_EINT);

	while(timeout--)
	{
		if(get_pin() == 0)
		{
			SWI_BIFDelay(SWI_BIF_SLAVE_RESPONSE_TIME);
			return SWI_BIFRESULT_NO_ERROR ;
		}

		SWI_BIFDelay(SWI_BIF_ONE_MICROSEC);
	}
	SWI_BIFTrans_Abort_EINT(handle);
	SWI_BIFDelay(SWI_BIF_SLAVE_RESPONSE_TIME);
	return SWI_BIFRESULT_TIME_OUT;
}

//..................................................................................
// Sending command EINT (enable interrupt mode) and don't wait for completion.
//
// !! this function is effectively implemented only if non-blocking interrupt mode
// !! is supported by the Master. This support is reported in the field
// !! NonBlockingInterruptSupported of SWI_BIFsigCapabilities structure after calling
// !! SWI_BIFHALGetCapabilities function.
//
// After sending the EINT command to Slaves, the function engages BCL monitoring on a // background process (HW cell commonly) and returns. The SWI_BIF bus is no more available // for any transactions. It's up to function caller to manage resuming from interrupt
// mode.
//
// The function returns SWI_BIFRESULT_NO_ERROR if background process is correctly started. // Else, it returns appropriate error information.
//
// The interrupt survey engaged on background process is resumed when:
// - the SWI_BIFTrans_Abort_EINT function is called.
// - Slaves produce interrupt.
//
// A BCL contact break detection (if such detection is supported by the Master)
// doesn't stop the interrupt survey background process. A call to SWI_BIFTrans_Abort_EINT
// function is required.
//
// By using SWI_BIFTrans_Query_EINT function, it is possible to get the state of the
// background interrupt survey process.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_Async_EINT(SWI_BIFhandle handle)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Probe the background interrupt survey process state.
//
// !! this function is effectively implemented only if non-blocking interrupt mode
// !! is supported by the Master. This support is reported in the field
// !! NonBlockingInterruptSupported of SWI_BIFsigCapabilities structure after calling
// !! SWI_BIFHALGetCapabilities function.
//
// When interrupt survey background process is engaged after a sucessfull call of
// SWI_BIFTrans_Async_EINT, this function returns the following:
// - SWI_BIFRESULT_NO_ERROR if a slave interrupt has been detected.
// - SWI_BIFRESULT_CONTACT_BROKEN if a BCL contact break has been detected (if supported
// by Master).
// - SWI_BIFRESULT_BG_RUNNING if background process is running and has not detected
// anything.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_Query_EINT(SWI_BIFhandle handle)
{
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Abort the interrupt mode engaged by SWI_BIFTrans_EINT command or SWI_BIFTrans_EINT_Engage
//
// This function emits the interrupt mode abort pulse on the SWI_BIF bus.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_Abort_EINT(SWI_BIFhandle handle)
{
	set_pin_dir(1);
	set_pin(0);
	udelay(tauLow);
	set_pin(1);
	set_pin_dir(0);
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Sending Slave vendor specific command with commandNibble code (from 0x00 to 0x0F)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_SlaveVendorSpecific(SWI_BIFhandle handle, SWI_BIFuint8 commandNibble)
{
	return SWI_BIFRESULT_NO_ERROR;
}


//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// Transaction function declaration
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

//...................................................................................
// Writing a 8bits "data" at a 16bits "address“ into the Slave.
// After write done, a TQ command is sent and its result is reported.
//...................................................................................
SWI_BIFresult SWI_BIFTransWriteUint8(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint8 data)
{
	SWI_BIFTransSendWord(SWI_BIF_ERA, ((address >> 8) & 0xFF));
	SWI_BIFTransSendWord(SWI_BIF_WRA, (address & 0xFF));

	SWI_BIFTransSendWord(SWI_BIF_WD, data);

	// TQ to be implemented

	return SWI_BIFRESULT_NO_ERROR;
}

//..................................................................................
// Writing a 16bits "data" at a 16bits "address“ into the Slave.
// After write done, a TQ command is sent and its result is reported.
//...................................................................................
SWI_BIFresult SWI_BIFTransWriteUint16(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint16 data)
{
	SWI_BIFTransSendWord(SWI_BIF_ERA, ((address >> 8) & 0xFF));
	SWI_BIFTransSendWord(SWI_BIF_WRA, (address & 0xFF));

	SWI_BIFTransSendWord(SWI_BIF_WD, ((data >> 8) & 0xFF));
	SWI_BIFTransSendWord(SWI_BIF_WD, (data & 0xFF));

	// TQ to be implemented
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Read SWI_BIFuint8 “data” at a 16 bits "address“ into the Slave.
//...................................................................................
SWI_BIFresult SWI_BIFTransReadUint8(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint8 * data)
{
	U_SWI_BIF_WORD word;

	SWI_BIFTransSendWord(SWI_BIF_ERA, ((address >> 8) & 0xFF));
	SWI_BIFTransSendWord(SWI_BIF_RRA, (address & 0xFF));

	if(SWI_BIFTransReceiveWord(&word) == SWI_BIF_TRUE)
	{
		*data = word.sSWI_BIFAbstact.PAYLOAD;
	}
	else
	{
		*data = 0xff;
		return SWI_BIFRESULT_ERR_SLAVE_NO_RESP;
	}
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Read SWI_BIFuint16 “data” at a 16 bits "address“ into the Slave.
//...................................................................................
SWI_BIFresult SWI_BIFTransReadUint16(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint16 * data)
{
	U_SWI_BIF_WORD word;

	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_RBE0);
	SWI_BIFTransSendWord(SWI_BIF_BC, SWI_BIF_RBL2);

	SWI_BIFTransSendWord(SWI_BIF_ERA, ((address >> 8) & 0xFF));
	SWI_BIFTransSendWord(SWI_BIF_RRA, (address & 0xFF));

	if(SWI_BIFTransReceiveWord(&word) == SWI_BIF_TRUE)
	{
		*data = (word.sSWI_BIFAbstact.PAYLOAD<<8);
		if(SWI_BIFTransReceiveWord(&word) == SWI_BIF_TRUE)
		{
			*data |= word.sSWI_BIFAbstact.PAYLOAD;
		}
		else
		{
			return SWI_BIFRESULT_ERR_SLAVE_NO_RESP;
		}
	}
	else
	{
		return SWI_BIFRESULT_ERR_SLAVE_NO_RESP;
	}
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Writing multiple 8bits "data" from a 16bits "address“ into the Slave.
// After write done, a TQ command is sent and its result is reported.
//...................................................................................
SWI_BIFresult SWI_BIFTransWriteMultUint8(SWI_BIFhandle handle, SWI_BIFuint16 address,
	SWI_BIFuint8 * data, SWI_BIFuint8 size)
{
	SWI_BIFuint8 cnt = 0;

	SWI_BIFTransSendWord(SWI_BIF_ERA, ((address >> 8) & 0xFF));
	SWI_BIFTransSendWord(SWI_BIF_WRA, (address & 0xFF));

	for(cnt=0; cnt < size; cnt++)
		SWI_BIFTransSendWord(SWI_BIF_WD, *(data+cnt));

	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Reading multiple 8bits "data" from a 16bits "address“ into the Slave.
//...................................................................................
SWI_BIFresult SWI_BIFTransReadMultUint8(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint8 * data,
	SWI_BIFuint16 size)
{
	SWI_BIFuint16 cnt = 0;
	U_SWI_BIF_WORD word;

	if(size != 256)//size=4e--lily
	{
		SWI_BIFTransSendWord(SWI_BIF_BC, (size/16)+0x30);
		SWI_BIFTransSendWord(SWI_BIF_BC, (size%16)+0x20);
	}
	else
	{
		SWI_BIFTransSendWord(SWI_BIF_BC, 0x30);
		SWI_BIFTransSendWord(SWI_BIF_BC, 0x20);
	}
	SWI_BIFTransSendWord(SWI_BIF_ERA, ((address >> 8) & 0xFF));
	SWI_BIFTransSendWord(SWI_BIF_RRA, (address & 0xFF));

	for(cnt=0; cnt<size; cnt++)
	{
		if(SWI_BIFTransReceiveWord(&word) == SWI_BIF_TRUE)
		{
			*(data+cnt) = word.sSWI_BIFAbstact.PAYLOAD;
		}
		else
		{
			return SWI_BIFRESULT_ERR_SLAVE_NO_RESP;
		}
	}
	return SWI_BIFRESULT_NO_ERROR;
}

//...................................................................................
// Select a Slave by its « UID » passed in parameter as an array of 8bits data
// (10 bytes).
//...................................................................................
SWI_BIFresult SWI_BIFTransSelUid(SWI_BIFhandle handle, SWI_BIFuint8 * uid)
{
	SWI_BIFint8 i;

	// loop through uid array to send out all uid.
	for (i = 0; i < 9; i++)
	{
		SWI_BIFTransSendWord(SWI_BIF_EDA, uid[i]);
	}

	SWI_BIFTransSendWord(SWI_BIF_SDA, uid[9]);

	return SWI_BIFRESULT_NO_ERROR;
}

// SWI_BIF protocol
static void SWI_BIFTransSetParityBits(U_SWI_BIF_WORD * word)
{
	SWI_BIFuint8 cnt = 0;

	cnt += (word->sSWI_BIFBits._BCF) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D8) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D6) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D4) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D3) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D1) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D0) ? 1 : 0;

	// total bit set that covered by parity bit 0 is even number
	if (cnt % 2 == 0)
	{
		word->sSWI_BIFBits.P0 = 0;
	}
	// total bit set that covered by parity bit 0 is odd number
	else
	{
		word->sSWI_BIFBits.P0 = 1;
	}

	// parity bit 1 setting
	// reset cnt counter
	cnt = 0;

	// check all bits that covered by parity bit 1
	// if total bit set is even number then parity bit 1 shall be set as zero,
	// otherwise set parity bit 1 as one
	cnt += (word->sSWI_BIFBits._BCF) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D9) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D6) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D5) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D3) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D2) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D0) ? 1 : 0;

	// total bit set that covered by parity bit 1 is even number
	if (cnt % 2 == 0)
	{
		word->sSWI_BIFBits.P1 = 0;
	}
	else // total bit set that covered by parity bit 1 is odd number
	{
		word->sSWI_BIFBits.P1 = 1;
	}

	// parity bit 2 setting
	// reset cnt counter
	cnt = 0;

	// check all bits that covered by parity bit 2
	// if total bit set is even number then parity bit 2 shall be set as zero,
	// otherwise set parity bit 2 as one
	cnt += (word->sSWI_BIFBits._BCF) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D9) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D8) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D7) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D3) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D2) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D1) ? 1 : 0;

	// total bit set that covered by parity bit 2 is even number
	if (cnt % 2 == 0)
	{
		word->sSWI_BIFBits.P2 = 0;
	}
	else // total bit set that covered by parity bit 2 is odd number
	{
		word->sSWI_BIFBits.P2 = 1;
	}

	// parity bit 3 setting.
	// reset cnt counter
	cnt = 0;

	// check all bits that covered by parity bit 3.
	// if total bit set is even number then parity bit 3 shall be set as zero,
	// otherwise set parity bit 3 as one.
	cnt += (word->sSWI_BIFBits._BCF) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D9) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D8) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D7) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D6) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D5) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D4) ? 1 : 0;

	// total bit set that covered by parity bit 3 is even number
	if (cnt % 2 == 0)
	{
		word->sSWI_BIFBits.P3 = 0;
	}
	else // total bit set that covered by parity bit 3 is even number
	{
		word->sSWI_BIFBits.P3 = 1;
	}
}


static SWI_BIFbool SWI_BIFTransCheckParityBit(U_SWI_BIF_WORD * word)
{
	SWI_BIFuint8 cnt = 0;
	SWI_BIFbool res = SWI_BIF_TRUE;

	// parity bit 0 setting
	// check all bits that covered by parity bit 0
	// if total bit set is even number then parity bit 0 shall be zero,
	// otherwise parity bit 0 shall be one
	cnt += (word->sSWI_BIFBits._BCF) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D8) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D6) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D4) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D3) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D1) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D0) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.P0) ? 1 : 0;
	if(cnt % 2 != 0)
	{
		res = SWI_BIF_FALSE;
	}

	// parity bit 1 setting
	// reset cnt counter
	cnt = 0;

	// check all bits that covered by parity bit 1
	// if total bit set is even number then parity bit 1 shall be set as zero,
	// otherwise set parity bit 1 as one
	cnt += (word->sSWI_BIFBits._BCF) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D9) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D6) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D5) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D3) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D2) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D0) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.P1) ? 1 : 0;
	if(cnt % 2 != 0)
	{
		res = SWI_BIF_FALSE;
	}

	// parity bit 2 setting
	// reset cnt counter
	cnt = 0;

	// check all bits that covered by parity bit 2
	// if total bit set is even number then parity bit 2 shall be set as zero,
	// otherwise set parity bit 2 as one
	cnt += (word->sSWI_BIFBits._BCF) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D9) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D8) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D7) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D3) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D2) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D1) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.P2) ? 1 : 0;
	if(cnt % 2 != 0)
	{
		res = SWI_BIF_FALSE;
	}

	// parity bit 3 setting.
	// reset cnt counter
	cnt = 0;

	// check all bits that covered by parity bit 3.
	// if total bit set is even number then parity bit 3 shall be set as zero,
	// otherwise set parity bit 3 as one.
	cnt += (word->sSWI_BIFBits._BCF) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D9) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D8) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D7) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D6) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D5) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D4) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.P3) ? 1 : 0;
	if(cnt % 2 != 0)
	{
		res = SWI_BIF_FALSE;
	}

	return res;
}

static SWI_BIFbool SWI_BIFTransCheckBitInversion(U_SWI_BIF_WORD * word)
{
	SWI_BIFuint8 cnt = 0;
	SWI_BIFbool res = TRUE;

	cnt += (word->sSWI_BIFBits.D9) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D8) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D7) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D6) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D5) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D4) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.P3) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D3) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D2) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D1) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.P2) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.D0) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.P1) ? 1 : 0;
	cnt += (word->sSWI_BIFBits.P0) ? 1 : 0;

	// check to make sure logical ones are less than half.
	if (cnt > 7)
	{
		res = SWI_BIF_FALSE;
	}

	return res;
}

static void SWI_BIFTransTreatInvertFlag(U_SWI_BIF_WORD * word)
{
	SWI_BIFuint8 bits;
	SWI_BIFuint8 cnt = 0;
	SWI_BIFuint16 data = word->sSWI_BIFInvRelevant.DATA;

	// loop through data bits to count for number of one.
	for(bits = 0; bits < 14; bits++)
	{
		if(data & 0x1)
		{
			cnt++;
		}
		data >>= 1;
	}

	// if number of one more than half then carry out bit
	// inversion handling.
	if(cnt > 7)
	{
		word->sSWI_BIFInvRelevant.DATA = ~word->sSWI_BIFInvRelevant.DATA;
		word->sSWI_BIFInvRelevant.INV = 1;
	}
}

static SWI_BIFbool SWI_BIFTransProcessReceivedWord(U_SWI_BIF_WORD * word)
{
	// check for bit inversion.
	if (!SWI_BIFTransCheckBitInversion(word))
	{
		return SWI_BIF_FALSE;
	}

	// handle inverted bit if it was set.
	if(word->sSWI_BIFBits.INV)
	{
		word->sSWI_BIFBits.INV = 0;
		word->sSWI_BIFInvRelevant.DATA = ~word->sSWI_BIFInvRelevant.DATA;
	}

	// check for errors
	if( (word->sSWI_BIFBits.BCF == word->sSWI_BIFBits._BCF) || SWI_BIFTransCheckParityBit(word) == FALSE)
	{
		return SWI_BIF_FALSE;
	}

	return SWI_BIF_TRUE;
}

static void SWI_BIFTransSendWord(SWI_BIFuint8 code, SWI_BIFuint8 data)
{
	U_SWI_BIF_WORD word;
	S_SWI_BIF_WORDBITS * SWI_BIFBits = &word.sSWI_BIFBits;

	word.uwWord = 0;
	word.sSWI_BIFAbstact.CODE = code;
	word.sSWI_BIFAbstact.PAYLOAD = data;

	// set parity bit base on SWI_BIF word
	SWI_BIFTransSetParityBits(&word);

	// handle bit inversion if necessary
	SWI_BIFTransTreatInvertFlag(&word);

	// set SWI_BIF gpio as output
	set_pin_dir(1);

  	// send a stop command first.
	set_pin(1);
	SWI_BIFDelay(tauStop);

	// send BCF
	set_pin(0);
	SWI_BIFBits->BCF ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send _BCF
	set_pin(1);
	SWI_BIFBits->_BCF ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 9
	set_pin(0);
	SWI_BIFBits->D9 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 8
	set_pin(1);
	SWI_BIFBits->D8 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 7
	set_pin(0);
	SWI_BIFBits->D7 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 6
	set_pin(1);
	SWI_BIFBits->D6 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 5
	set_pin(0);
	SWI_BIFBits->D5 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 4
	set_pin(1);
	SWI_BIFBits->D4 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send parity bit 3
	set_pin(0);
	SWI_BIFBits->P3 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 3
	set_pin(1);
	SWI_BIFBits->D3 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 2
	set_pin(0);
	SWI_BIFBits->D2 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 1
	set_pin(1);
	SWI_BIFBits->D1 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send parity bit 2
	set_pin(0);
	SWI_BIFBits->P2 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send payload bit 0
	//gpio_set_value(SWI_BIF_GPIO_PIN, 1);
	set_pin(1);
	SWI_BIFBits->D0 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send parity bit 1
	set_pin(0);
	SWI_BIFBits->P1 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send parity bit 0
	set_pin(1);
	SWI_BIFBits->P0 ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);
	// send inversion bit
	set_pin(0);
	SWI_BIFBits->INV ? SWI_BIFDelay(tauHigh): SWI_BIFDelay(tauLow);

	// Send Stop Command
	set_pin(1);

	SWI_BIFDelay(tauStop);

	// set SWI_BIF gpio as input
	set_pin_dir(0);
}

static SWI_BIFbool SWI_BIFTransReceiveWord(U_SWI_BIF_WORD * word)
{
	SWI_BIFuint8 prevSWI_BIFState;
	SWI_BIFuint8 index = 16;
	SWI_BIFuint8 bitsToCapture;
	SWI_BIFuint32 timeout = SWI_BIF_SLAVE_RESPONSE_TIME;
	SWI_BIFuint32 samples[17];
	SWI_BIFuint32 maxSamples = 0;
	SWI_BIFuint32 minSamples = ~maxSamples;
 	SWI_BIFuint32 cnt;
	SWI_BIFuint32 threshold;

	// quit the while loop when SWI_BIF gpio change to low
	// or time out.
	while(get_pin() && timeout)
	{
		timeout--;
		SWI_BIFDelay(3);//
	}

	// return false if time out and yet SWI_BIF gpio doesn't
	// get toggled.
	if(!timeout)
	{
		return SWI_BIF_FALSE;
	}

	// store previous SWI_BIF state.ok
	
	prevSWI_BIFState = get_pin();

	// loop through all SWI_BIF word bits to sample the timing for each bit.
	for( bitsToCapture = 17; bitsToCapture != 0; bitsToCapture-- )
	{
		cnt = 0;

		while(get_pin() == prevSWI_BIFState)
		{
			cnt++;
		}

		samples[index--] = cnt;
		prevSWI_BIFState = get_pin();
	}

	// loop through the stored timing to locate the min and max timing.
	for(index = 0; index < 17; index++)
	{
		if(samples[index] < minSamples)
		{
			minSamples = samples[index];
		}
		else if(samples[index] > maxSamples)
		{
			maxSamples = samples[index];
		}
	}

	// find the threshold between zero and one.
	threshold = ((maxSamples - minSamples) >> 1);
	threshold += minSamples;

	word->sSWI_BIFBits.BCF 	= (samples[16] > threshold) ? 1 : 0;
	word->sSWI_BIFBits._BCF = (samples[15] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D9 	= (samples[14] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D8 	= (samples[13] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D7 	= (samples[12] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D6 	= (samples[11] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D5 	= (samples[10] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D4 	= (samples[9] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.P3 	= (samples[8] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D3 	= (samples[7] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D2 	= (samples[6] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D1 	= (samples[5] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.P2 	= (samples[4] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.D0 	= (samples[3] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.P1 	= (samples[2] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.P0 	= (samples[1] > threshold) ? 1 : 0;
	word->sSWI_BIFBits.INV 	= (samples[0] > threshold) ? 1 : 0;

	if (!SWI_BIFTransProcessReceivedWord(word))
	{
		word = 0;
		return SWI_BIF_FALSE;
	}

	return SWI_BIF_TRUE;
}


