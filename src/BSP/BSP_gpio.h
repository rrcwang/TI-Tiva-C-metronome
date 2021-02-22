// gpio.h
// Target hardware: TM4C123GH6PM
// Description:     Configures pins in GPIO ports for digital input or output
// Author:          rrcwang
// Feb 20, 2021

#include <stdint.h>

// pins is a bit-packed (one-hot) representation of the pins to be configured

void GPIO_PortB_Initialize(uint8_t pins);
void GPIO_PortB_PWM_Initialize(uint8_t pins);       // TODO: create file for PWM configurations
void GPIO_PortB_Digital_Enable_In(uint8_t pins);
void GPIO_PortB_Digital_Enable_Out(uint8_t pins);

void GPIO_PortF_Initialize(uint8_t pins);
void GPIO_PortF_Digital_Enable_In(uint8_t pins);
void GPIO_PortF_Digital_Enable_Out(uint8_t pins);

// use library functions to R/W digital pins
// GPIOPinWrite(BASE, PINS)
// GPIOPinRead(BASE, PINS)