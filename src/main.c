/////////////////////////////////////////////////////////
//  blinks LED 
//      edge triggered interrupts handled with NVIC
//      starter code adapted from jspicer-ltu/Tiva-C-Embedded/Experiment1-GPIO


#include <stdint.h>
#include "main.h"

///////////////////////////
// signals flag to be enabled
void SemSignal(int32_t *p) {
    (*p)++;
}

///////////////////////////
// port F interrupt handler
// triggers on falling edge events on PF0, PF4

#define DEBOUNCE_DELAY   3
uint8_t input;
int32_t SW1;
int32_t SW2;
void GPIOPortF_Handler(void) {
    // poll switch for input, active low
    input = (uint8_t)GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4); // & 0x11);
	if (~(input) & 0x10) { // SW1, PF4
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
        SysCtlDelay(DEBOUNCE_DELAY);     // debounce switch
        SemSignal(&SW1);
	}
	if (~(input) & 0x01) { // SW2, PF0
        GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
        SysCtlDelay(DEBOUNCE_DELAY);     // debounce switch
        SemSignal(&SW2);
    }
}

///////////////////////////
// declare local variables
uint8_t ledNumber;
uint8_t outputPin;
int main() {
    // init system clock
    PLL_Initialize();
    ///////////////////////////
    // set up I/O on port F for LEDs and switches
    initGPIOPortF();
    initPWMPB4();
    
    ///////////////////////////
    // infinite loop
    while(1) {
        if (SW1) {
            // turn on all LEDs.    Color will turn WHITE.
            SW1 = 0;
            ledNumber = 3;
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
            setPWMEPB4DutyCycle(0.5);
        }
        else if (SW2) {
            // turn off all LEDs
            SW2 = 0;
            ledNumber = 0;  // begin blinking loop again
            GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3, 0x0);
            setPWMEPB4DutyCycle(1.0);
        }
        else if (ledNumber != 3){
            // set pin to be driven HIGH depending on which LED is chosen
            switch(ledNumber) {
                case 0:     outputPin = GPIO_PIN_1;     break;  // RED
                case 1:     outputPin = GPIO_PIN_2;     break;  // BLUE
                case 2:     outputPin = GPIO_PIN_3;     break;  // GREEN
                default:    break;
            }
            
            // turn on LED of given colour
            GPIOPinWrite(GPIO_PORTF_BASE, outputPin, outputPin);
            
            // delay for a bit
            SysCtlDelay(10000000);

            // turn off the selected LED
            GPIOPinWrite(GPIO_PORTF_BASE, outputPin, 0x0);
            
            // move to next LED
            // alternate LED colour RED, BLUE, GREEN, RED, ...
            ledNumber = (ledNumber + 1) % 3;
        }
    }
    // should never be reached
    for(;;) {}
}


///////////////////////////////
// Set up GPIO port F pins for input and outputs
// Digital GPIO output mode:    PF1     RED
//                              PF2     BLUE
//                              PF3     GREEN
//              input mode:     PF0     switch 0
//                              PF4     switch 1
void initGPIOPortF(){
    
    // enable GPIOF, which is mapped to the on-board LED
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF); // SYSCTL_RCGCGPIO_R |= 0x20;
    // wait for access to GPIOF to be enabled
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)){}
    
    // unlock port F
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY;  // pass passcode
    GPIO_PORTF_CR_R = 0x1;              // commit passcode
    
    ///////////////////////////
    // set up input (PF0, PF4)
        // set direction as input
        // enables digital mode
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    // enable weak pullup for input pints
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    
    // enable interrupts on inputs
    // clear flags
    GPIOIntClear(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    // set falling edge interrupt type on PF0, PF4
    GPIOIntTypeSet(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4, GPIO_FALLING_EDGE);
    // enable interrupts PF0, PF4
    GPIOIntEnable(GPIO_PORTF_BASE, GPIO_PIN_0 | GPIO_PIN_4);
    // set GPIOPortF_Handler defined in the bootloader NVIC table to trigger on interrupt
    IntPrioritySet(INT_GPIOF, 5);   // set interrupt priority
    IntEnable(INT_GPIOF);
    
    ///////////////////////////
    // set up output
    // enable GPIO pins for the LED (PF3-PF1)
        // set direction as output
        // enable digital mode
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    
    ///////////////////////////
    // enable PWM mode
    
    return;
}

// Color    LED(s) PortF
// dark     ---    0
// red      R--    0x02
// blue     --B    0x04
// green    -G-    0x08
// yellow   RG-    0x0A
// sky blue -GB    0x0C
// white    RGB    0x0E
// pink     R-B    0x06


void initPWMPB4() { // Module 0, Generator 1, Pin 2
    // set PWM clock to match system clock
    SysCtlPWMClockSet(SYSCTL_PWMDIV_1);
    
    // enable GPIO on port B
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOB));
    
    // configure PB4 for PWM output
    GPIOPinConfigure(GPIO_PB4_M0PWM2);
    GPIOPinTypePWM(GPIO_PORTB_BASE, GPIO_PIN_4);
    
    // enable PWM module 0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0);
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_PWM0));
    
    // configure PMW generator 1 -> M0PWM2
    PWMGenConfigure(PWM0_BASE, PWM_GEN_1, (PWM_GEN_MODE_DOWN| PWM_GEN_MODE_NO_SYNC));
    // Set generator period divisor
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_1, PWM_MAX_LOAD);
    // set duty cycle to minimum
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, 1);
    // enable generator
    PWMGenEnable(PWM0_BASE, PWM_GEN_1);
    // turn on PWM output pins
    PWMOutputState(PWM0_BASE, PWM_OUT_2_BIT, true);
}

// must be called after initPWMPB4() has set up PWM output on the pin 
//  duty cycle is given as a float in range (0, 1)
void setPWMEPB4DutyCycle(double dutyCycle) {
    // clamp input value 
    if (dutyCycle >= 1) {
        dutyCycle = 0.9999;
    } else if (dutyCycle <= 0) {
        dutyCycle = 0.0001;
    }
    
    uint32_t load = (uint32_t) (dutyCycle * PWM_MAX_LOAD);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_2, load);
}
