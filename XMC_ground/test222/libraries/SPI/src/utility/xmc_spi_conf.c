/**
 * @file xmc_spi_conf.c
 * @date 2016-11-18
 *
 * @cond
 */

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "xmc_spi_conf.h"

XMC_SPI_t XMC_SPI_0 =
{
    .channel          = XMC_SPI2_CH0,
    .channel_config   = {
        .baudrate = 20003906U,
        .bus_mode = (XMC_SPI_CH_BUS_MODE_t)XMC_SPI_CH_BUS_MODE_MASTER,
        .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
        .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
    },
    .mosi             = {
        .port = (XMC_GPIO_PORT_t*)PORT3_BASE,
        .pin  = (uint8_t)8
    },
    .mosi_config      = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
    .miso             = {
        .port = (XMC_GPIO_PORT_t*)PORT3_BASE,
        .pin  = (uint8_t)7
    },
    .miso_config      = {
        .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
    },
    .input_source     = XMC_INPUT_C,
    .sclkout = {
        .port = (XMC_GPIO_PORT_t*)PORT3_BASE,
        .pin  = (uint8_t)9
    },
    .sclkout_config   = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
};

XMC_SPI_t XMC_SPI_1 =
{
    .channel          = XMC_SPI0_CH1,
    .channel_config   = {
        .baudrate = 20003906U,
        .bus_mode = (XMC_SPI_CH_BUS_MODE_t)XMC_SPI_CH_BUS_MODE_MASTER,
        .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
        .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
    },
    .mosi = {
        .port   = (XMC_GPIO_PORT_t*)PORT3_BASE,
        .pin    = (uint8_t)5
    },
    .mosi_config      = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
    .miso             = {
        .port = (XMC_GPIO_PORT_t*)PORT4_BASE,
        .pin  = (uint8_t)0
    },
    .miso_config      = {
        .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
    },
    .input_source     = XMC_INPUT_E,
    .sclkout = {
        .port = (XMC_GPIO_PORT_t*)PORT3_BASE,
        .pin  = (uint8_t)6
    },
    .sclkout_config   = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT4,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
};

XMC_SPI_t XMC_SPI_2 =
{
    .channel          = XMC_SPI2_CH1,
    .channel_config   = {
        .baudrate = 20003906U,
        .bus_mode = (XMC_SPI_CH_BUS_MODE_t)XMC_SPI_CH_BUS_MODE_MASTER,
        .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
        .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
    },
    .mosi             = {
        .port = (XMC_GPIO_PORT_t*)PORT3_BASE,
        .pin  = (uint8_t)11
    },
    .mosi_config      = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
    .miso             = {
        .port = (XMC_GPIO_PORT_t*)PORT3_BASE,
        .pin  = (uint8_t)12
    },
    .miso_config      = {
        .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
    },
    .input_source     = XMC_INPUT_D,
    .sclkout = {
        .port = (XMC_GPIO_PORT_t*)PORT3_BASE,
        .pin  = (uint8_t)13
    },
    .sclkout_config   = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
};

XMC_SPI_t XMC_SPI_3 =
{
    .channel          = XMC_SPI0_CH0,
    .channel_config   = {
        .baudrate = 20003906U,
        .bus_mode = (XMC_SPI_CH_BUS_MODE_t)XMC_SPI_CH_BUS_MODE_MASTER,
        .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
        .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
    },
    .mosi             = {
        .port = (XMC_GPIO_PORT_t*)PORT5_BASE,
        .pin  = (uint8_t)1
    },
    .mosi_config      = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT1,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
    .miso             = {
        .port = (XMC_GPIO_PORT_t*)PORT5_BASE,
        .pin  = (uint8_t)0
    },
    .miso_config      = {
        .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
    },
    .input_source     = XMC_INPUT_D,
    .sclkout = {
        .port = (XMC_GPIO_PORT_t*)PORT0_BASE,
        .pin  = (uint8_t)8
    },
    .sclkout_config   = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
};

XMC_SPI_t XMC_SPI_4 =
{
    .channel          = XMC_SPI1_CH0,
    .channel_config   = {
        .baudrate = 20003906U,
        .bus_mode = (XMC_SPI_CH_BUS_MODE_t)XMC_SPI_CH_BUS_MODE_MASTER,
        .selo_inversion = XMC_SPI_CH_SLAVE_SEL_INV_TO_MSLS,
        .parity_mode = XMC_USIC_CH_PARITY_MODE_NONE
    },
    .mosi             = {
        .port = (XMC_GPIO_PORT_t*)PORT0_BASE,
        .pin  = (uint8_t)5
    },
    .mosi_config      = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
    .miso             = {
        .port = (XMC_GPIO_PORT_t*)PORT0_BASE,
        .pin  = (uint8_t)4
    },
    .miso_config      = {
        .mode = XMC_GPIO_MODE_INPUT_TRISTATE,
    },
    .input_source     = XMC_INPUT_A,
    .sclkout = {
        .port = (XMC_GPIO_PORT_t*)PORT0_BASE,
        .pin  = (uint8_t)11
    },
    .sclkout_config   = {
        .mode = XMC_GPIO_MODE_OUTPUT_PUSH_PULL_ALT2,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH,
        .output_strength = XMC_GPIO_OUTPUT_STRENGTH_MEDIUM
    },
};

//****************************************************************************
//                                 END OF FILE
//****************************************************************************
