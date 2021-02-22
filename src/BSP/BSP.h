// gpio.h
// Target hardware: TM4C123GH6PM
// Description:     Configures pins in GPIO ports for digital input or output
// Author:          rrcwang
// Feb 20, 2021

// mapping for the IO ports.
typedef enum {
    PORTA,
    PORTB,
    PORTC,
    PORTD,
    PORTE,
    PORTF
} PortNumber_t;

// Names for the IO pins.  These are also bit flags and can
//	OR'd together for pin maps.
typedef enum {
    PIN0 = 0x01,
    PIN1 = 0x02,
    PIN2 = 0x04,
    PIN3 = 0x08,
    PIN4 = 0x10,
    PIN5 = 0x20,
    PIN6 = 0x40,
    PIN7 = 0x80
} PinNumber_t;