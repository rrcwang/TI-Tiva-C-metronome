// pll.c
// Target hardware: TM4C123GH6PM
// Description:     Sets the system clock frequency using the main oscillator (MOSC) to drive the PLL.
// Author:          rrcwang Feb 20, 2021

// See main clock tree Fig. 515 (pg 222) in docs/tm4c123gh6pm.pdf

#include "BSP_pll.h"
#include <stdint.h>

#ifndef _STDBOOL
#include <stdbool.h>
#endif

#ifndef __TM4C123GH6PM_H__
#include <inc/TM4C123GH6PM.h>
#endif

#ifndef __DRIVERLIB_SYSCTL_H__
#include <driverlib/sysctl.h>
#endif


// Sets system clock frequency to highest possible 80MHz
//
// For all possible system clock frequencies, see Table 5-4 through 5-6
// NOTE: Writes to RCC2 register. Writes to RCC register should be done BEFORE
// to avoid conflict (e.g. PWMDIV, USEPWMDIV)
void PLL_Initialize(void) {
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
    // SYSCTL_USE_PLL:      400MHz PLL
    // SYSCTL_SYSDIV_2_5:   80MHz == 400MHz / (2 * 2.5)
    // SYSCTL_OSC_INT:      use main oscillator
    // SYSCTL_XTAL_16MHZ:   16MHz main/on-board external oscillator driving PLL
}

// Sets system clock frequency to highest possible 80MHz
//
// Implementation of PLL_Initialize() using memory addressing mode
// For description of RCC2 (Run-Mode Clock Configuration 2) registers, see page 260
void PLL_Initialize_MemAddr(void) {
    // 0. override RCC2 value to enable user configuration
    SYSCTL_RCC2_R |= SYSCTL_RCC2_USERCC2;
    // 1. bypass PLL during initialization -- set bypass bit 11
    SYSCTL_RCC2_R |= SYSCTL_RCC2_BYPASS2;
    // 2. set crystal value and oscillator source
    SYSCTL_RCC_R = ((SYSCTL_RCC_R & ~SYSCTL_RCC_XTAL_M) | SYSCTL_RCC_XTAL_16MHZ);
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_OSCSRC2_M;  // clear oscillator source field
    // SYSCTL_RCC_R += SYSCTL_RCC2_OSCSRC2_MO;  // set source to main oscillator == 0x0
    // 3. activate and configure 400MHz PLL
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_PWRDN2;  // clear PWRDN
    SYSCTL_RCC2_R |= SYSCTL_RCC2_DIV400;   // use 400MHz PLL
    // 5. clear the SYSDIV2 bits and set to 0x4, 400/(0x4+1) => 80MHz
    SYSCTL_RCC2_R = (SYSCTL_RCC2_R & ~(SYSCTL_RCC2_SYSDIV2_M | SYSCTL_RCC2_SYSDIV2LSB)) + (0x4 << 22);
    // 6. wait for the PLL to lock
    while ((SYSCTL_RIS_R & SYSCTL_RIS_PLLLRIS) == 0) {
    }
    // 7. clear BYPASS to enable PLL.
    SYSCTL_RCC2_R &= ~SYSCTL_RCC2_BYPASS2;
}