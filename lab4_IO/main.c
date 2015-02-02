#include <stdint.h>

#include "inc/hw_gpio.h"
#include "inc/hw_memmap.h"
#include "inc/hw_sysctl.h"
#include "inc/hw_types.h"
#include "driverlib/gpio.h"
#include "driverlib/rom.h"
#include "driverlib/sysctl.h"

#define LED_RED GPIO_PIN_1
#define LED_BLUE GPIO_PIN_2
#define LED_GREEN GPIO_PIN_3

#define SW1 GPIO_PIN_4
#define SW2 GPIO_PIN_0

int is_pushed(uint32_t pin)
{
    uint32_t status =  ~ROM_GPIOPinRead(GPIO_PORTF_BASE, pin);
    // mask for getting just the relevant pins
    status &= pin;
    return status == pin;
}

void light(uint32_t color)
{   
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE, color);
}

void setup()
{
    // Setup clock
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
   
    // Enable GPIO 
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    //
    // Unlock PF0 so we can change it to a GPIO input
    // Once we have enabled (unlocked) the commit register then re-lock it
    // to prevent further changes.  PF0 is muxed with NMI thus a special case.
    //
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY_DD;
    HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
    HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;

    //
    // Set each of the button GPIO pins as an input with a pull-up.
    //
    ROM_GPIODirModeSet(GPIO_PORTF_BASE, SW1 | SW2, GPIO_DIR_MODE_IN);
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, SW1 | SW2,
                         GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    // Enable input and outputs
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN);
}

int main()
{
    setup();

    while(true) {
        if(is_pushed(SW1 | SW2))
            light(LED_BLUE);
        else if(is_pushed(SW1))
            light(LED_RED);
        else if(is_pushed(SW2))
            light(LED_GREEN);
        else
            light(0x0);
    }
}
