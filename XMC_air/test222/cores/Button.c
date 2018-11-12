/**
 * @file Buttons.c
 * @date 2016-10-19
 *
 * @cond
 *********************************************************************************************************************
 * Buttons interface for XMC4700 Relxa Kit v1.0.1
 *
 * Copyright (c) 2015-2016, Infineon Technologies AG
 * All rights reserved.                        
 *                                             
 * Redistribution and use in source and binary forms, with or without modification,are permitted provided that the 
 * following conditions are met:   
 *                                                                              
 * Redistributions of source code must retain the above copyright notice, this list of conditions and the following 
 * disclaimer.                        
 * 
 * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following 
 * disclaimer in the documentation and/or other materials provided with the distribution.                       
 * 
 * Neither the name of the copyright holders nor the names of its contributors may be used to endorse or promote 
 * products derived from this software without specific prior written permission.                                           
 *                                                                              
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE  
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR  
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.                                                  
 *                                                                              
 * To improve the quality of the software, users are encouraged to share modifications, enhancements or bug fixes with 
 * Infineon Technologies AG dave@infineon.com).                                                          
 *********************************************************************************************************************
 *
 * Change History
 * --------------
 *
 * 2016-10-19:
 *     - Fix initialization of input pins
 *
 * @endcond 
 *
 */

#include "xmc_gpio.h"
#include "Button.h"

/* GPIO Pin identifier */
typedef struct _GPIO_PIN 
{
  XMC_GPIO_PORT_t *port;
  uint8_t         pin;
} GPIO_PIN;

/* LED GPIO Pins */
static const GPIO_PIN BUTTON_PIN[] = 
{
  { XMC_GPIO_PORT15, 13 },
  { XMC_GPIO_PORT15, 12 }
};

#define BUTTON_COUNT (sizeof(BUTTON_PIN)/sizeof(GPIO_PIN))


/**
  \fn          int32_t Buttons_Initialize (void)
  \brief       Initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Initialize (void) 
{
  uint32_t n;
  XMC_GPIO_CONFIG_t gpio_config = 
	{
		.mode = XMC_GPIO_MODE_INPUT_INVERTED_TRISTATE
	};
	
  for (n = 0; n < BUTTON_COUNT; ++n) 
  {
    XMC_GPIO_Init(BUTTON_PIN[n].port, BUTTON_PIN[n].pin, &gpio_config);
  }

  return 0;
}

/**
  \fn          int32_t Buttons_Uninitialize (void)
  \brief       De-initialize buttons
  \returns
   - \b  0: function succeeded
   - \b -1: function failed
*/
int32_t Buttons_Uninitialize (void) 
{

  return 0;
}

/**
  \fn          uint32_t Buttons_GetState (void)
  \brief       Get buttons state
  \returns     Buttons state
*/
uint32_t Buttons_GetState (void) 
{
  uint32_t n;
  uint32_t val = 0;

  for (n = 0; n < BUTTON_COUNT; ++n) 
  {
    val |= XMC_GPIO_GetInput(BUTTON_PIN[n].port, BUTTON_PIN[n].pin) << n;
  }

  return val;
}

/**
  \fn          uint32_t Buttons_GetCount (void)
  \brief       Get number of available buttons
  \return      Number of available buttons
*/
uint32_t Buttons_GetCount (void) 
{
  return BUTTON_COUNT;
}
