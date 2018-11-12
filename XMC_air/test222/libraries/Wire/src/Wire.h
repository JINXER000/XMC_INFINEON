/*
  TwoWire.h - TWI/I2C library for Arduino & Wiring
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated starts
*/

#ifndef _WIRE_H
#define _WIRE_H
//#define TwoWire_h

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "xmc_i2c_conf.h"
#include "Stream.h"
//****************************************************************************
// @Defines
//****************************************************************************
#define BUFFER_LENGTH 32
// WIRE_HAS_END means Wire has end()
#define WIRE_HAS_END 1

#define WIRE_COMMUNICATION_TIMEOUT 5000u

//****************************************************************************
// @Class Definitions
//****************************************************************************
class TwoWire : public Stream
{
private:
    XMC_I2C_t* XMC_I2C_config;

    static bool isMaster;
    static bool inRepStart;

    static uint8_t rxBuffer[];
    static uint8_t rxBufferIndex;
    static uint8_t rxBufferLength;

    static uint8_t slaveAddress;
    static uint8_t txAddress;
    static uint8_t txBuffer[];
    static uint8_t txBufferIndex;
    static uint8_t txBufferLength;

    static uint8_t pre_rxBuffer[];
    static uint8_t pre_rxBufferCount;

    static uint8_t transmitting;
    static uint16_t timeout;
    static void (*user_onRequest)(void);
    static void (*user_onReceive)(int);
    void OnRequestService(void);
    void OnReceiveService(uint8_t*, uint8_t);
public:
	bool volatile hasError;
    TwoWire();
	TwoWire(XMC_I2C_t *conf);
    void begin();
    void begin(uint8_t);
    void begin(int);
    void end();
    void setClock(uint32_t);
    void beginTransmission(uint8_t);
    void beginTransmission(int);
    uint8_t endTransmission(void);
    uint8_t endTransmission(uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint8_t);
    uint8_t requestFrom(uint8_t, uint8_t, uint32_t, uint8_t, uint8_t);
    uint8_t requestFrom(int, int);
    uint8_t requestFrom(int, int, int);
    virtual size_t write(uint8_t);
    virtual size_t write(const uint8_t*, size_t);
    virtual int available(void);
    virtual int read(void);
    virtual int peek(void);
    virtual void flush(void);
    void onReceive( void (*)(int) );
    void onRequest( void (*)(void) );
    void ReceiveHandler(void);
    void ProtocolHandler(void);

    inline size_t write(unsigned long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(long n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(unsigned int n)
    {
        return write((uint8_t)n);
    }
    inline size_t write(int n)
    {
        return write((uint8_t)n);
    }
    //using Print::write;
};


//****************************************************************************
// @Typedefs
//****************************************************************************
extern TwoWire Wire;
extern TwoWire Wire1;



#endif /* Wire_h */
