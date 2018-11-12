/**
 * @file xmc_spi_conf.h
 * @date 2016-11-18
 *
 * @cond
 */

#ifndef XMC_SPI_CONF_H
#define XMC_SPI_CONF_H

//****************************************************************************
// @Project Includes
//****************************************************************************
#include <xmc_spi.h>
#include <xmc_i2c_conf.h>
//****************************************************************************
// @Typedefs
//****************************************************************************

typedef struct XMC_SPI
{
    XMC_USIC_CH_t*         channel        ;
    XMC_SPI_CH_CONFIG_t   channel_config  ;
    XMC_PORT_PIN_t        mosi            ;
    XMC_GPIO_CONFIG_t     mosi_config     ;
    XMC_PORT_PIN_t        miso            ;
    XMC_GPIO_CONFIG_t     miso_config     ;
    XMC_USIC_INPUT_t      input_source    ;
    XMC_PORT_PIN_t        sclkout         ;
    XMC_GPIO_CONFIG_t     sclkout_config  ;
} XMC_SPI_t;

#define XMC_SPI_default 	XMC_SPI_0

#define NUM_SPI  			5
#define XMC_SPI_for_xmc_SD	XMC_SPI_1
extern XMC_SPI_t XMC_SPI_0;
extern XMC_SPI_t XMC_SPI_1;
extern XMC_SPI_t XMC_SPI_2;
extern XMC_SPI_t XMC_SPI_3;
extern XMC_SPI_t XMC_SPI_4;
/*#else
#error XMC Board not supported
#endif*/


#endif /* XMC_SPI_CONF_H */
