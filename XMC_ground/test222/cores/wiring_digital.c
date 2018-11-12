//****************************************************************************
// @Project Includes
//****************************************************************************
#include "xmc_i2c_conf.h"

void m_pinMode(uint8_t pin, uint8_t mode)
{
    XMC_GPIO_CONFIG_t gpio_conf = {
		.mode = (XMC_GPIO_MODE_t)mode};
    //gpio_conf.mode = (XMC_GPIO_MODE_t)mode;

    XMC_GPIO_Init(mapping_port_pin[pin].port, mapping_port_pin[pin].pin, &gpio_conf);
}

uint8_t digitalRead(uint8_t pin)  //GND=32
{
    uint32_t val = (pin == 32) ? 0 : XMC_GPIO_GetInput(mapping_port_pin[pin].port, mapping_port_pin[pin].pin);
    return ((val ? 1 : 0));
}

void digitalWrite(uint8_t pin, uint8_t value)
{
    XMC_GPIO_SetOutputLevel(mapping_port_pin[pin].port, mapping_port_pin[pin].pin, (value == 0) ? XMC_GPIO_OUTPUT_LEVEL_LOW : XMC_GPIO_OUTPUT_LEVEL_HIGH);

}

void digitalToggle(uint8_t pin)
{
    XMC_GPIO_ToggleOutput(mapping_port_pin[pin].port, mapping_port_pin[pin].pin);

}

//****************************************************************************
//                                 END OF FILE
//****************************************************************************
