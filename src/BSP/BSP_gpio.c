// gpio.h
// Target hardware: TM4C123GH6PM
// Description:     Configures pins in GPIO ports for digital input or output
// Author:          rrcwang
// Feb 20, 2021
// Attributions:    JSpicer for the struct overlay access idiom to read/write to registers. Immensely

#include "BSP_gpio.h"

#include <driverlib/sysctl.h>
#include <inc/TM4C123GH6PM.h>
#include <inc/hw_memmap.h>
#include <stdbool.h>
#include <stdint.h>

// A lookup up table for base addresses of the ports
//	Use a PortName_t value for an index.
const volatile uint32_t* GPIOBaseAddress[] = {
    GPIO_PORTA_BASE,
    GPIO_PORTB_BASE,
    GPIO_PORTC_BASE,
    GPIO_PORTD_BASE,
    GPIO_PORTE_BASE,
    GPIO_PORTF_BASE,
};

typedef struct {
    uint32_t DATA_BITS[255];  // see page 654 for use
    uint32_t DATA;
} GPIORegisters_t;

void GPIO_PortB_Initialize(uint8_t pins);