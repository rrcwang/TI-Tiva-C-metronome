#ifndef MAIN_H
#define MAIN_H

#define PART_TM4C123AE6PM

#include <stdbool.h>
#include <stdint.h>
#include <inc/hw_memmap.h>
#include <inc/hw_gpio.h>
#include <driverlib/pwm.h>
#include <driverlib/sysctl.h>
#include <driverlib/gpio.h>
#include <driverlib/pin_map.h>
#include "driverlib/interrupt.h"    // NVIC 
#include "inc/TM4C123GH6PM.h"

#include "BSP/BSP_pll.h"

// 16 MHz/PWM_MAX_LOAD is cycle period
#define PWM_MAX_LOAD 16000

///////////////////////////////
// Set up GPIO port F pins for input and outputs
// Digital GPIO output mode:    PF1     RED
//                              PF2     BLUE
//                              PF3     GREEN
//              input mode:     PF0     switch 0
//                              PF4     switch 1
void initGPIOPortF(void);
void initPWMPB4(void);
void setPWMEPB4DutyCycle(double dutyCycle);


#endif
