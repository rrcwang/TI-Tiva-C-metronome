// pll.h
// Target hardware: TM4C123GH6PM
// Description:     Sets the system clock frequency using the main oscillator (MOSC) to drive the PLL.
// Author:          rrcwang
// Feb 20, 2021

#ifndef BSP_PLL_H
#define BSP_PLL_H

void PLL_Init(void);
void PLL_Init_MA(void);

#endif