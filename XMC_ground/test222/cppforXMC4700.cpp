/*! @file cppforXMC4700.cpp
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief Support for printf to PC_UART on XMC platform
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */
#ifndef CPPFORXMC4700_H
#define CPPFORXMC4700_H

#include <XMC4700.h>
#include "cppforXMC4700.h"
#include "BspUsart.h"
#include "xmc_gpio.h"
#include "xmc_uart.h"

extern XMC_USIC_CH_t           *pc_uart;


#ifdef DYNAMIC_MEMORY
void * operator new (size_t size)
{
  if(NULL == size)
  {
#ifdef DEBUG
    printf("Error! Size is zero");
#endif//DEBUG
    return NULL;
  }
  void *p = malloc(size);
#ifdef DEBUG
  if(p == 0)
  printf("Lack Memory!");
#endif//DEBUG
  return p;
}

void * operator new [](size_t size)
{
  return operator new(size);
}

void operator delete (void * pointer)
{
  if(NULL != pointer)
  {
    free(pointer);
  }
}

void operator delete[](void * pointer)
{
  operator delete(pointer);
}
#endif //DYNAMIC_MEMORY

//!@code printf link functions
#ifdef __cplusplus
extern "C"
{
#endif //__cplusplus
//int fputc(int ch, FILE *f)
PUTCHAR_PROTOTYPE
{

  XMC_UART_CH_Transmit(pc_uart, (unsigned char) ch);

  return (ch);
} 
#ifdef __cplusplus
}
#endif //__cplusplus
//!@endcode printf link fuctions.
#endif 
