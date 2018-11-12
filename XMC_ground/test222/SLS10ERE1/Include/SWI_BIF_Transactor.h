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

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//
// This header file declare types and functions of SWI_BIF Hardware Abstraction Layer
// //
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

#ifndef SWI_BIF_HAL_H
#define SWI_BIF_HAL_H

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// Header file version
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

#define SWI_BIF_HAL_HEADER_VERSION	(10)

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// Basic types declaration
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

#ifndef SWI_BIF_BASIC_TYPES_DEFINED
#define SWI_BIF_BASIC_TYPES_DEFINED
#if 0
#include <stdint.h>

typedef int8_t	SWI_BIFint8;
typedef int16_t	SWI_BIFint16;
typedef int32_t	SWI_BIFint32;
typedef uint8_t	SWI_BIFuint8;
typedef uint16_t	SWI_BIFuint16;
typedef uint32_t	SWI_BIFuint32;

typedef uint8_t	SWI_BIFchar;

typedef uint8_t	SWI_BIFbool;

#else
typedef char	SWI_BIFint8;
typedef short	SWI_BIFint16;
typedef long	SWI_BIFint32;
typedef unsigned char	SWI_BIFuint8;
typedef unsigned short	SWI_BIFuint16;
typedef unsigned long	SWI_BIFuint32;

typedef unsigned char	SWI_BIFchar;

typedef unsigned char	SWI_BIFbool;
#endif

#define SWI_BIF_TRUE	(1)
#define SWI_BIF_FALSE	(0)

#endif

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// Default handle type define
//
// The handle exact definition is application specific. It should be redefined at
//   target implementation
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

#ifndef SWI_BIF_HANDLESWI_BIF_TYPE_DEFINED
#define SWI_BIF_HANDLESWI_BIF_TYPE_DEFINED

typedef SWI_BIFuint8 SWI_BIFhandle;

#endif

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// HAL specific types
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
#ifndef SWI_BIF_HAL_TYPES_DEFINED
#define SWI_BIF_HAL_TYPES_DEFINED

//...................................................................................
// SWI_BIFresult
//...................................................................................
#define	SWI_BIFRESULT_NO_ERROR		 (0x0000) // no error
#define	SWI_BIFRESULT_TIME_OUT		 (0x0001) // time-out occured
#define	SWI_BIFRESULT_ABORTED		 (0x0002) // command has been aborted
#define	SWI_BIFRESULT_CONTACT_BROKEN (0x0003) // BCL has been disconnected.
						 // This result can be reported only if
						 // contact break detector is embedded in
						 // the HW.
#define	SWI_BIFRESULT_NO_EVENT       (0x0004) // No interrupt or contact-broken is detected

#define	SWI_BIFRESULT_ERR_VENDOR_SPEC (0x4000) // vendor specific error

#define	SWI_BIFRESULT_ERR_HW_INIT	    (0x8000) // HW initialization error
#define	SWI_BIFRESULT_ERR_HW_FAIL	    (0x8001) // HW failure
#define	SWI_BIFRESULT_ERR_HW_BUSY	    (0x8002) // HW busy
#define	SWI_BIFRESULT_ERR_SLAVE_NO_RESP	(0x8003) // Slave has not responded to a read in
 // time
#define	SWI_BIFRESULT_ERR_SLAVE_NACK    (0x8004) // Slave reported an error at read (ACK is
 // 0)
#define	SWI_BIFRESULT_ERR_SLAVE_EOT	    (0x8005) // Slave terminated transmission earlier
						 // than burst length requested
#define	SWI_BIFRESULT_ERR_SLAVE_SIG	    (0x8006) // A signaling error is seen during Slave
									   // transmission (parity, inversion or BCF)
#define	SWI_BIFRESULT_ERR_NOT_SUPPORTED	(0x8007) // The called function is not implemented
#define	SWI_BIFRESULT_ERR_OUT_OF_RANGE	(0x8008) // Function parameter is out of range

typedef SWI_BIFuint16 SWI_BIFresult; 	// Teach about success/error of function call.

//...................................................................................
// SWI_BIFslaveError
//...................................................................................
#define SWI_BIFSLAVEERROR_NO_ERROR		(0x00)	// no error
#define SWI_BIFSLAVEERROR_GEN_COM		(0x10)	// general communication error
#define SWI_BIFSLAVEERROR_PARITY		(0x11)	// parity error
#define SWI_BIFSLAVEERROR_INV			(0x12)	// Inversion error
#define SWI_BIFSLAVEERROR_LENGTH		(0x13)	// Invalid word length error
#define SWI_BIFSLAVEERROR_TIMING		(0x14)	// Timing error
#define SWI_BIFSLAVEERROR_UNKWN_CMD		(0x15)	// unknown command error
#define SWI_BIFSLAVEERROR_SEQ			(0x16)	// Wrong sequence error
#define SWI_BIFSLAVEERROR_BUS_COL		(0x1F)	// Bus collision error
#define SWI_BIFSLAVEERROR_BUSY			(0x20)	// Slave busy error
#define SWI_BIFSLAVEERROR_FATAL			(0x7F)	// Fatal error

#define SWI_BIFSLAVEERROR_VENDOR_BASE	(0x80)	// Slave vendor specific error

typedef SWI_BIFuint8	SWI_BIFslaveError; 	// Error reported from Slave

//...................................................................................
// Signaling layer capabilities
//...................................................................................
typedef struct
{
	SWI_BIFbool	LowPowerTauBifSupport;	// is SWI_BIF_TRUE if this mode is
// supported else SWI_BIF_FALSE
	SWI_BIFuint8	OptimizedReadBurstLength;	// Teach indirectly about the read HW
							// FIFO length of SWI_BIF Master. Making
							// read operation length shorter than
// this value ensures read operation
							// to be completed in a single read
							// burst.
	SWI_BIFuint16	ManufacturerId;		// Manufacturer identifier of SWI_BIF
// Master solution.
	SWI_BIFuint16	DeviceId;			// Device identifier of SWI_BIF Master
// solution.
	SWI_BIFuint16	ContactBreakDetTime;		// Informs about detection time
// guaranteed by the SWI_BIF Master
							// contact break sensor.
							// Time is expressed in 100ns unit
							// (for example, 134 means 13.4µs
// detection time).
							// If the Master solution doesn't
// embed contact break detector,
							// the reported time is 0.
	SWI_BIFbool	NonBlockingInterruptSupported; // is SWI_BIF_TRUE if the Master
// implementation support non blocking
							// interrupt mode.

} SWI_BIFsigCapabilities;

//...................................................................................
// Signaling layer state
//...................................................................................
typedef struct
{
	SWI_BIFbool	TauBifNormalMode;	// SWI_BIF_FALSE: low power TauBif mode selected,
						// SWI_BIF_TRUE: normal TauBif selected
	SWI_BIFuint16	TauBif; 		// TauBif for the normal mode
						// in 100ns unit (for example, 25 means
// 2.5µs)
	SWI_BIFuint16	LowPowerTauBif; 	// If low power TauBif is not supported,
						// value is 0 else value is TauBif for this
						// mode in 100ns unit
						// (for example, 25 means 2.5µs)
	SWI_BIFbool	BclPadState;		// reports the current BCL pad state
						// (SWI_BIF_FALSE: low, SWI_BIF_TRUE: high)
	SWI_BIFuint8	SlaveAddressSelected;// reports the latest Slave selection
// address (done with SDA command)
	SWI_BIFbool	SlaveSelectionCache; // If SWI_BIF_TRUE, Slave selection cache is
// enabled.
						// In such case, any SDA command selecting an
						// address equal to SlaveAddressSelected is
						// ignored. If SWI_BIF_FALSE, all SDA commands
						// are emitted even if they select again same
						// Slave.
} SWI_BIFsigState;


//...................................................................................
// Signaling layer reported strings
//...................................................................................

#define SWI_BIF_STRING_MAX_LENGTH		(0x40) // Max length of string (64 chars
							// including null terminating char)
#define SWI_BIF_STRING_ID_MANUFACTURER_NAME	(0x00) // Manufacturer name
#define SWI_BIF_STRING_ID_DEVICE_NAME		(0x01) // Device name

#endif

//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// Signaling layer function declaration
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

//...................................................................................
// Getting implemented version of interface.
// This function informs about the interface version implemented. This should match
// the SWI_BIF_HAL_HEADER_VERSION version of this header file.
//...................................................................................
SWI_BIFresult SWI_BIFHALGetVersion(SWI_BIFhandle handle, SWI_BIFuint16 * version);

//...................................................................................
// Getting information string.
// This function populates the "string" pointer with the corresponding constant
//  "stringID".
// "stringID" is one of defined SWI_BIF_STRING_ID_...
// The string reported has length below or equal SWI_BIF_STRING_MAX_LENGTH
//...................................................................................
SWI_BIFresult SWI_BIFHALGetString(SWI_BIFhandle handle, SWI_BIFuint8 stringID, SWI_BIFchar * string);

//...................................................................................
// Getting error code reported by Slave.
// If SWI_BIFresult is SWI_BIFRESULT_ERR_SLAVE_NACK, it means that transaction went well at
// signaling level but the salve was not understanding it correctly. With NACK
// reporting from Slave, an error code is provided which can be retrieved with the
// following function.
//...................................................................................
SWI_BIFresult SWI_BIFHALGetLastError(SWI_BIFhandle handle, SWI_BIFslaveError * error);

//...................................................................................
// Retrieving the signaling layer capabilities
//...................................................................................
SWI_BIFresult SWI_BIFHALGetCapabilities(SWI_BIFhandle handle, SWI_BIFsigCapabilities * capabilities);

//...................................................................................
// Signaling layer initialization.
// Calling this function initializes the signaling layer to a default inactive
// state.
//...................................................................................
SWI_BIFresult SWI_BIFHALInit(SWI_BIFhandle handle);

//...................................................................................
// Signaling layer enabling.
// Calling this function with “enable” at SWI_BIF_FALSE place the signaling layer in
// inactive state (BCL is not driven). If “enable” is SWI_BIF_TRUE, the
// signaling layer Masters the BCL.
//...................................................................................
SWI_BIFresult SWI_BIFHALEnabling(SWI_BIFhandle handle, SWI_BIFbool enable);

//...................................................................................
// TauBif setting.
// This function configures and selects the normal TauSWI_BIF timing.
// The “tauBif” parameter is expressed in 100ns unit (2.3µs corresponds to
//  tauBif = 23).
//
// The effective TauBif configured is reported in field TauBif of SWI_BIFsigState.
//...................................................................................
SWI_BIFresult SWI_BIFHALSetTauSWI_BIF(SWI_BIFhandle handle, SWI_BIFuint16 tauBif);

//...................................................................................
// Set and select TauBif to the most power efficient value supported by the HW.
//
// If LowPowerTauBifSupport is supported in the SWI_BIFsigCapabilities, calling the
// SWI_BIFSigSetLowPowerTauBif configures the HW to run with the best compromise of
// transmission speed vs power consumption.
//
// Typically, SWI_BIFHALSetLowPowerTauBif configures the HW to use a low power clock tree
// which is generally at low frequency. By consequence, TauBif would be clamped to a
// long value.
//
// The “tauBif” parameter is expressed in 100ns unit (100µs corresponds to
// tauBif = 1000). The effective TauBif for this mode is reported in field
// LowPowerTauBif of SWI_BIFsigState.
//...................................................................................
SWI_BIFresult SWI_BIFHALSetLowPowerTauSWI_BIF(SWI_BIFhandle handle, SWI_BIFuint16 tauBif);

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
SWI_BIFresult SWI_BIFHALSlaveSelectionCache(SWI_BIFhandle handle, SWI_BIFbool enable);

//...................................................................................
// Get state of signaling layer.
// The function set-up the data pointed by “state” pointer.
//...................................................................................
SWI_BIFresult SWI_BIFHALGetState(SWI_BIFhandle handle, SWI_BIFsigState * state);

//...................................................................................
// Generic access point to vendor specific signaling layer functionalities
//...................................................................................
SWI_BIFuint16 SWI_BIFHALVendorSpecific(SWI_BIFhandle handle, SWI_BIFuint32 command, void * data_in,
	void * data_out);


//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// Transaction Elements layer function declaration
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------


//...................................................................................
// Sending command BRES (Bus Reset)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_BRES(SWI_BIFhandle handle);

//...................................................................................
// Driving BCL low for tPDL (Hard Reset)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_HardRST(SWI_BIFhandle handle);

//...................................................................................
// Sending command PWDN (Power down)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_PWDN(SWI_BIFhandle handle);

//...................................................................................
// Sending command STBY (standby)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_STBY(SWI_BIFhandle handle);

//...................................................................................
// Produces an activation signal only
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WakeUpPulse(SWI_BIFhandle handle);

//...................................................................................
// Produces an activation signal and wait for tPUP delays in order to go out of
// power-down
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WakeUpFromPWDN(SWI_BIFhandle handle);

//...................................................................................
// Produces an activation signal and wait for tACT delays in order to go out of
// standby mode
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WakeUpFromSTBY(SWI_BIFhandle handle);

//...................................................................................
// Sending command ISTS (interrupt status).
// The function reports SWI_BIF_TRUE if Slave reports interrupt pending else it report
// SWI_BIF_FALSE.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_ISTS(SWI_BIFhandle handle, SWI_BIFbool * intStatus);

//...................................................................................
// Sending command RBL (read burst length) with the command parameter
//...................................................................................
SWI_BIFresult SWI_BIFTrans_RBL(SWI_BIFhandle handle, SWI_BIFuint8 length);

//...................................................................................
// Sending command RBE (read burst extended) with the command parameter
//...................................................................................
SWI_BIFresult SWI_BIFTrans_RBE(SWI_BIFhandle handle, SWI_BIFuint8 length);

//...................................................................................
// Sending command DASM (device multiple select)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DASM(SWI_BIFhandle handle);

//...................................................................................
// Sending command DISS
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DISS(SWI_BIFhandle handle);

//...................................................................................
// Sending command DILC.
// Report the DILC result.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DILC(SWI_BIFhandle handle, SWI_BIFbool * dilc);

//...................................................................................
// Sending command DIE0
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DIE0(SWI_BIFhandle handle);

//...................................................................................
// Sending command DIE1
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DIE1(SWI_BIFhandle handle);

//...................................................................................
// Sending command DIP0.
// Reports SWI_BIF_TRUE if a Slave responds to probing else SWI_BIF_FALSE.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DIP0(SWI_BIFhandle handle, SWI_BIFbool * dip0);

//...................................................................................
// Sending command DIP1.
// Reports SWI_BIF_TRUE if a Slave responds to probing else SWI_BIF_FALSE.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DIP1(SWI_BIFhandle handle, SWI_BIFbool * dip1);

//...................................................................................
// Sending command DRES (device reset)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_DRES(SWI_BIFhandle handle);

//...................................................................................
// Sending command TQ (Transaction query).
// If no error, reports transaction “dataCount” value (“errorCode” will be
// SWI_BIFSLAVEERROR_NO_ERROR).
// If error, returns “errorCode” from Slave (“dataCount” will be 0).
//...................................................................................
SWI_BIFresult SWI_BIFTrans_TQ(SWI_BIFhandle handle, SWI_BIFslaveError * errorCode,
	SWI_BIFuint8 * dataCount);

//...................................................................................
// Sending command AIO
//...................................................................................
SWI_BIFresult SWI_BIFTrans_AIO(SWI_BIFhandle handle);

//...................................................................................
// Sending command EDA.
// Function takes the EDA command parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_EDA(SWI_BIFhandle handle, SWI_BIFuint8 add);

//...................................................................................
// Sending command SDA (Slave select by address).
// Function takes the Slave address as parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_SDA(SWI_BIFhandle handle, SWI_BIFuint8 add);

//...................................................................................
// Sending command ERA.
// Function takes the command parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_ERA(SWI_BIFhandle handle, SWI_BIFuint8 add);

//...................................................................................
// Sending command WRA.
// Function takes the command parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WRA(SWI_BIFhandle handle, SWI_BIFuint8 add);

//...................................................................................
// Sending command WD.
// Function takes the command parameter.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_WD(SWI_BIFhandle handle, SWI_BIFuint8 data);

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
	SWI_BIFuint8 length);

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
SWI_BIFresult SWI_BIFTrans_EINT(SWI_BIFhandle handle, SWI_BIFuint32 timeOut);

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
SWI_BIFresult SWI_BIFTrans_Async_EINT(SWI_BIFhandle handle);

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
SWI_BIFresult SWI_BIFTrans_Query_EINT(SWI_BIFhandle handle);

//...................................................................................
// Abort the interrupt mode engaged by SWI_BIFTrans_EINT command or SWI_BIFTrans_EINT_Engage
//
// This function emits the interrupt mode abort pulse on the SWI_BIF bus.
//...................................................................................
SWI_BIFresult SWI_BIFTrans_Abort_EINT(SWI_BIFhandle handle);

//...................................................................................
// Sending Slave vendor specific command with commandNibble code (from 0x00 to 0x0F)
//...................................................................................
SWI_BIFresult SWI_BIFTrans_SlaveVendorSpecific(SWI_BIFhandle handle, SWI_BIFuint8 commandNibble);


//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
// Transaction function declaration
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------

//...................................................................................
// Writing a 8bits "data" at a 16bits "address“ into the Slave.
// After write done, a TQ command is sent and its result is reported.
//...................................................................................
SWI_BIFresult SWI_BIFTransWriteUint8(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint8 data);

//..................................................................................
// Writing a 16bits "data" at a 16bits "address“ into the Slave.
// After write done, a TQ command is sent and its result is reported.
//...................................................................................
SWI_BIFresult SWI_BIFTransWriteUint16(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint16 data);

//...................................................................................
// Read SWI_BIFuint8 “data” at a 16 bits "address“ into the Slave.
//...................................................................................
SWI_BIFresult SWI_BIFTransReadUint8(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint8 * data);

//...................................................................................
// Read SWI_BIFuint16 “data” at a 16 bits "address“ into the Slave.
//...................................................................................
SWI_BIFresult SWI_BIFTransReadUint16(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint16 * data);

//...................................................................................
// Writing multiple 8bits "data" from a 16bits "address“ into the Slave.
// After write done, a TQ command is sent and its result is reported.
//...................................................................................
SWI_BIFresult SWI_BIFTransWriteMultUint8(SWI_BIFhandle handle, SWI_BIFuint16 address,
	SWI_BIFuint8 * data, SWI_BIFuint8 size);

//...................................................................................
// Reading multiple 8bits "data" from a 16bits "address“ into the Slave.
//...................................................................................
SWI_BIFresult SWI_BIFTransReadMultUint8(SWI_BIFhandle handle, SWI_BIFuint16 address, SWI_BIFuint8 * data,
	SWI_BIFuint16 size);

//...................................................................................
// Select a Slave by its « UID » passed in parameter as an array of 8bits data
// (10 bytes).
//...................................................................................
SWI_BIFresult SWI_BIFTransSelUid(SWI_BIFhandle handle, SWI_BIFuint8 * uid);



#endif

