/**
 * @file xmc_i2c_conf.h
 * @date 2016-11-18
 *
 * @cond
 */

#ifndef XMC_I2C_CONF_H
#define XMC_I2C_CONF_H

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "common.h"
#include <xmc_i2c.h>
#include <xmc_ccu4.h>
//****************************************************************************
// @Typedefs
//****************************************************************************

  /*
    * XMC USIC channel input selection types
    */
typedef enum XMC_USIC_INPUT
{
        XMC_INPUT_A = 0U, /*< Input-A */
        XMC_INPUT_B,      /*< Input-B */
        XMC_INPUT_C,      /*< Input-C */
        XMC_INPUT_D,      /*< Input-D */
        XMC_INPUT_E,      /*< Input-E */
        XMC_INPUT_F,      /*< Input-F */
        XMC_INPUT_G,      /*< Input-G */
        XMC_INPUT_INVALID /*< This is to check during mode switch */
}XMC_USIC_INPUT_t;

    /*
    * XMC port pin type
    */
typedef struct XMC_PORT_PIN
{
        XMC_GPIO_PORT_t* port;
        unsigned          pin;
}XMC_PORT_PIN_t;

    /*
    * XMC pin interrupt type
    */
typedef struct XMC_PIN_INTERRUPT
{
        XMC_CCU4_MODULE_t*      ccu         ;
        XMC_CCU4_SLICE_t*       slice       ;
        uint8_t                 slice_num   ;
        uint8_t                 irq_num     ;
        XMC_CCU4_SLICE_INPUT_t  input       ;
}XMC_PIN_INTERRUPT_t;

typedef struct XMC_I2C
{
    XMC_USIC_CH_t* channel;
    XMC_I2C_CH_CONFIG_t   channel_config;
    XMC_PORT_PIN_t        sda;
    XMC_GPIO_CONFIG_t     sda_config;
    XMC_PORT_PIN_t        scl;
    XMC_GPIO_CONFIG_t     scl_config;
    XMC_USIC_INPUT_t      input_source_dx0;
    XMC_USIC_INPUT_t      input_source_dx1;
    IRQn_Type             slave_receive_irq_num;
    uint32_t              slave_receive_irq_service_request;
    IRQn_Type             protocol_irq_num;
    uint32_t              protocol_irq_service_request;
} XMC_I2C_t;

#define XMC_I2C_default 	XMC_I2C_0

#define NUM_I2C  2
extern XMC_I2C_t XMC_I2C_0;
extern XMC_I2C_t XMC_I2C_1;

extern const XMC_PORT_PIN_t mapping_port_pin[];
extern const XMC_PIN_INTERRUPT_t mapping_interrupt[];

#endif /* XMC_I2C_CONF_H */
