/**
 * @file xmc_i2c_conf.c
 * @date 2016-11-18
 *
 * @cond
 */

//****************************************************************************
// @Project Includes
//****************************************************************************
#include "xmc_i2c_conf.h"

const XMC_PORT_PIN_t mapping_port_pin[] =
{
    /* 0  */    {XMC_GPIO_PORT2, 15}, // PIN_RX                         P2.15
    /* 1  */    {XMC_GPIO_PORT2 , 14}, // PIN_TX                         P2.14
    /* 2  */    {XMC_GPIO_PORT1 , 0}, // GPIO / INT0                    P1.0
    /* 3  */    {XMC_GPIO_PORT1 , 1}, // PWM output / INT1              P1.1
    /* 4  */    {XMC_GPIO_PORT1 , 8}, // GPIO                           P1.8
    /* 5  */    {XMC_GPIO_PORT2 , 12}, // PWM output                     P2.12
    /* 6  */    {XMC_GPIO_PORT2 , 11}, // PWM output                     P2.11
    /* 7  */    {XMC_GPIO_PORT1 , 9}, // GPIO                           P1.9
    /* 8  */    {XMC_GPIO_PORT1 , 10}, // GPIO                           P1.10
    /* 9  */    {XMC_GPIO_PORT1 , 11}, // PWM output                     P1.11
    /* 10  */   {XMC_GPIO_PORT3 , 10}, // SPI-SS / PWM output            P3.10
    /* 11  */   {XMC_GPIO_PORT3 , 8}, // SPI-MOSI / PWM output          P3.8
    /* 12  */   {XMC_GPIO_PORT3 , 7}, // SPI-MISO                       P3.7
    /* 13  */   {XMC_GPIO_PORT3 , 9}, // SPI-SCK                        P3.9
    /* 14  */   {XMC_GPIO_PORT2 , 3}, // AREF TODO:                     P2.3
    /* 15  */   {XMC_GPIO_PORT3 , 15}, // I2C Data / Address SDA         P3.15
    /* 16  */   {XMC_GPIO_PORT0 , 13}, // I2C Clock SCL                  P0.13
    /* 17  */   {XMC_GPIO_PORT14 , 0}, // A0 / ADC Input                 P14.0
    /* 18  */   {XMC_GPIO_PORT14 , 1}, // A1 / ADC Input                 P14.1
    /* 19  */   {XMC_GPIO_PORT14 , 2}, // A2 / ADC Input                 P14.2
    /* 20  */   {XMC_GPIO_PORT14 , 3}, // A3 / ADC Input                 P14.3
    /* 21  */   {XMC_GPIO_PORT14 , 4}, // A4 / ADC Input                 P14.4
    /* 22  */   {XMC_GPIO_PORT14 , 5}, // A5 / ADC Input                 P14.5
    /* 23  */   {XMC_GPIO_PORT3 , 12}, // SPI_SS_2                       P3.12
    /* 24  */   {XMC_GPIO_PORT5 , 9}, // Additional LED1                P5.9
    /* 25  */   {XMC_GPIO_PORT5 , 8}, // Additional LED2                P5.8
    /* 26  */   {XMC_GPIO_PORT15 , 13}, // Additional BUTTON1            P15.13
    /* 27  */   {XMC_GPIO_PORT15 , 12}, // Additional BUTTON2            P15.12
    /* 28  */   {XMC_GPIO_PORT4 , 1}, // SPI_SS_3 (SD CARD)             P4.1
    /* 29  */   {XMC_GPIO_PORT3 , 5}, // SPI-MOSI (SD CARD)             P3.5
    /* 30  */   {XMC_GPIO_PORT4 , 0}, // SPI-MISO (SD CARD)             P4.0
    /* 31  */   {XMC_GPIO_PORT3 , 6}, // SPI-SCK  (SD CARD)             P3.6
    /* 32  */   {XMC_GPIO_PORT1 , 6}, //                                P1.6
    /* 33  */   {XMC_GPIO_PORT1 , 7}, //                                P1.7
    /* 34  */   {XMC_GPIO_PORT1 , 4}, //                                P1.4
    /* 35  */   {XMC_GPIO_PORT1 , 5}, //                                P1.5
	
	//Additional pins for port X1 starting here
	/* 36  */   {XMC_GPIO_PORT3 , 4}, // PWM                            P3.4
	/* 37  */   {XMC_GPIO_PORT0 , 5}, // I2C_1 SDA  // SPI_4 MOSI       P0.5
	/* 38  */   {XMC_GPIO_PORT0 , 3}, // PWM                            P0.3
	/* 39  */   {XMC_GPIO_PORT0 , 1}, // PWM                            P0.1
	/* 40  */   {XMC_GPIO_PORT0 , 10}, //                               P0.10
	/* 41  */   {XMC_GPIO_PORT3 , 2}, //                                P3.2
	/* 42  */   {XMC_GPIO_PORT3 , 1}, //                                P3.1
	/* 43  */   {XMC_GPIO_PORT15 , 6}, // A6 / ADC Input                P15.6
	/* 44  */   {XMC_GPIO_PORT15 , 5}, // A7 / ADC Input               P15.5
	/* 45  */   {XMC_GPIO_PORT15 , 3}, // A8 / ADC Input               P15.3
	/* 46  */   {XMC_GPIO_PORT14 , 15}, // A9 / ADC Input              P14.15
	/* 47  */   {XMC_GPIO_PORT14 , 13}, // A10 / ADC Input              P14.13
	/* 48  */   {XMC_GPIO_PORT14 , 7}, // A11 / ADC Input                P14.7
	/* 49  */   {XMC_GPIO_PORT15 , 15}, // A12 / ADC Input              P15.15
	/* 50  */   {XMC_GPIO_PORT14 , 9}, // DAC0 // A13 / ADC Input       P14.9
	/* 51  */   {XMC_GPIO_PORT2 , 13}, //                               P2.13
	/* 52  */   {XMC_GPIO_PORT5 , 10}, //                               P5.10
	/* 53  */   {XMC_GPIO_PORT5 , 11}, // PWM                           P5.11
	/* 54  */   {XMC_GPIO_PORT1 , 14}, //                               P1.14
	/* 55  */   {XMC_GPIO_PORT14 , 8}, // DAC1 // A14 / ADC Input       P14.8
	/* 56  */   {XMC_GPIO_PORT15 , 14}, // A15 / ADC Input              P15.14
	/* 57  */   {XMC_GPIO_PORT14 , 6}, // A16 / ADC Input                P14.6
	/* 58  */   {XMC_GPIO_PORT14 , 12}, // A17 / ADC Input              P14.12
	/* 59  */   {XMC_GPIO_PORT14 , 14}, // A18 / ADC Input              P14.14
	/* 60  */   {XMC_GPIO_PORT15 , 2}, // A19 / ADC Input               P15.2
	/* 61  */   {XMC_GPIO_PORT15 , 4}, // A20 / ADC Input               P15.4
	/* 62  */   {XMC_GPIO_PORT15 , 7}, // A21 / ADC Input               P15.7
	/* 63  */   {XMC_GPIO_PORT3 , 0}, // PWM                            P3.0
	/* 64  */   {XMC_GPIO_PORT0 , 9}, // PWM                            P0.9
	/* 65  */   {XMC_GPIO_PORT0 , 0}, //                                P0.0
	/* 66  */   {XMC_GPIO_PORT0 , 2}, //                                P0.2
	/* 67  */   {XMC_GPIO_PORT0 , 4}, // SPI_4 MISO                     P0.4
	/* 68  */   {XMC_GPIO_PORT0 , 6}, // PWM                            P0.6
	/* 69  */   {XMC_GPIO_PORT0 , 11}, // I2C_1 SCL // SPI_4 SCLK       P0.11
	
	//Additional pins for port X2 starting here
	/* 70  */   {XMC_GPIO_PORT3 , 13}, // SPI_2 SCLK                     P3.13
	/* 71  */   {XMC_GPIO_PORT3 , 11}, // SPI_2 MOSI                     P3.11
	/* 72  */   {XMC_GPIO_PORT0 , 14}, // PWM                            P0.14
	/* 73  */   {XMC_GPIO_PORT3 , 14}, //                                P3.14
	/* 74  */   {XMC_GPIO_PORT0 , 7}, //                                P0.7
	/* 75  */   {XMC_GPIO_PORT1 , 2}, //                                P1.2
	/* 76  */   {XMC_GPIO_PORT6 , 1}, //                                P6.1
	/* 77  */   {XMC_GPIO_PORT5 , 3}, //                                P5.3
	/* 78  */   {XMC_GPIO_PORT6 , 5}, // PWM                            P6.5
	/* 79  */   {XMC_GPIO_PORT1 , 15}, // PWM                           P1.15
	/* 80  */   {XMC_GPIO_PORT5 , 1}, // SPI_3 MOSI                     P5.1
	/* 81  */   {XMC_GPIO_PORT5 , 3}, // PWM                            P5.3
	/* 82  */   {XMC_GPIO_PORT5 , 5}, // PWM                            P5.5
	/* 83  */   {XMC_GPIO_PORT5 , 7}, // PWM                            P5.7
	/* 84  */   {XMC_GPIO_PORT2 , 6}, //                                P2.6
	/* 85  */   {XMC_GPIO_PORT5 , 6}, //                                P5.6
	/* 86  */   {XMC_GPIO_PORT5 , 4}, //                                P5.4
	/* 87  */   {XMC_GPIO_PORT5 , 2}, //                                P5.2
	/* 88  */   {XMC_GPIO_PORT5 , 0}, // SPI_3 MISO                     P5.0
	/* 89  */   {XMC_GPIO_PORT6 , 6}, //                                P6.6
	/* 90  */   {XMC_GPIO_PORT6 , 4}, // PWM                            P6.4
	/* 91  */   {XMC_GPIO_PORT6 , 2}, // PWM                            P6.2
	/* 92  */   {XMC_GPIO_PORT6 , 0}, //                                P6.0
	/* 93  */   {XMC_GPIO_PORT0 , 8}, // SPI_3 SCLK                     P0.8
	/* 94  */   {XMC_GPIO_PORT3 , 3}, //                                P3.3
	/* 95  */   {XMC_GPIO_PORT0 , 15}, // PWM                            P0.15
	/* 96  */   {XMC_GPIO_PORT0 , 12}, // PWM                            P0.12
	/* 97  */   {XMC_GPIO_PORT3 , 12}, // SPI_2 MISO                     P3.12
};

const XMC_PIN_INTERRUPT_t mapping_interrupt[] =
{
    /* 0  */    {CCU40, CCU40_CC43, 3, 0, CCU40_IN3_P1_0},
    /* 1  */    {CCU40, CCU40_CC42, 2, 1, CCU40_IN2_P1_1},
};


XMC_I2C_t XMC_I2C_0 =
{
    .channel          = XMC_I2C1_CH1,
    .channel_config   = {
        .baudrate = (uint32_t)(100000U),
        .address = 0U
    },
    .sda              = {
        .port = (XMC_GPIO_PORT_t*)PORT3_BASE,
        .pin  = (uint8_t)15
    },
    .sda_config       = {
        .mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT2,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
    },
    .scl              = {
        .port = (XMC_GPIO_PORT_t*)PORT0_BASE,
        .pin  = (uint8_t)13
    },
    .scl_config       = {
        .mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT2,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
    },
    .input_source_dx0 = XMC_INPUT_A,
    .input_source_dx1 = XMC_INPUT_B,
    .slave_receive_irq_num                    = (IRQn_Type) 91,
    .slave_receive_irq_service_request        = 1 ,
    .protocol_irq_num                		  = (IRQn_Type) 92,
    .protocol_irq_service_request     		  = 2
};
XMC_I2C_t XMC_I2C_1 =
{
    .channel          = XMC_I2C1_CH0,
    .channel_config   = {
        .baudrate = (uint32_t)(100000U),
        .address = 0U
    },
    .sda              = {
        .port = (XMC_GPIO_PORT_t*)PORT0_BASE,
        .pin  = (uint8_t)5
    },
    .sda_config       = {
        .mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT2,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
    },
    .scl              = {
        .port = (XMC_GPIO_PORT_t*)PORT0_BASE,
        .pin  = (uint8_t)11
    },
    .scl_config       = {
        .mode = XMC_GPIO_MODE_OUTPUT_OPEN_DRAIN_ALT2,
        .output_level = XMC_GPIO_OUTPUT_LEVEL_HIGH
    },
    .input_source_dx0 = XMC_INPUT_B,
    .input_source_dx1 = XMC_INPUT_A,
    .slave_receive_irq_num                    = (IRQn_Type) 93,
    .slave_receive_irq_service_request        = 3 ,
    .protocol_irq_num                  		  = (IRQn_Type) 94,
    .protocol_irq_service_request     		  = 4
};
//****************************************************************************
//                                 END OF FILE
//****************************************************************************
