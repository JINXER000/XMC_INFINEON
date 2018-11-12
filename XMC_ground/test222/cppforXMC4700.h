/*! @file cppforXMC4700.h
 *  @version 3.1.8
 *  @date Aug 05 2016
 *
 *  @brief Support for printf to PC_UART on XMC4700 platform
 *
 *  Copyright 2016 DJI. All right reserved.
 *
 * */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#ifdef DYNAMIC_MEMORY
void * operator new (size_t size);
void * operator new [](size_t size);
void operator delete (void * pointer);
void operator delete[](void * pointer);
#endif //DYNAMIC_MEMORY

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

