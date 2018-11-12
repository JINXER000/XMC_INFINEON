/*! @file driver.h
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief
 *  Implementation of HardDriver for the XMC4700Discovery board.
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

//#include "DJI_HardDriver.h"
#include "XMC4700.h"
#include <time.h>
#include "xmc_gpio.h"
#include "xmc_uart.h"

extern uint32_t tick;

//class XMC_4700 : public DJI::onboardSDK::HardDriver
//{
//  public:
//    virtual size_t send(const uint8_t* buf, size_t len);
//    virtual DJI::time_ms getTimeStamp();
//    virtual bool getDeviceStatus() { return true; }

//  public:
//    virtual void init() { ; }
//    virtual size_t readall(uint8_t* buf, size_t maxlen)
//    {
//      return 8;
//    }
//    virtual void lockMemory() { ; }
//    virtual void freeMemory() { ; }

//    virtual void lockMSG() { ; }
//    virtual void freeMSG() { ; }

//    virtual void lockACK() { ; }
//    virtual void freeACK() { ; }

//  virtual void notify() { ; }
//  virtual void wait(int timeout) { ; }

//};
extern volatile uint8_t UART1_rx_buffer;
extern volatile  uint32_t baro_h_air;
extern volatile  uint32_t baro_h_ground;
void send(XMC_USIC_CH_t *const channel,const uint8_t* buf, size_t len);
void send2xmc_air(uint8_t send_flag);
void send2stm(uint8_t send_flag);
void UART1_Prepare_Attitude(void) ;
void UART1_Prepare_AGM(void) ;
void UART1_Report_States(void);
