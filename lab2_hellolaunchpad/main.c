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

int is_sw1_pushed()
{
    uint32_t status = ROM_GPIOPinRead(GPIO_PORTF_BASE, SW1);
    return status != 0;
}

void blink(uint32_t color)
{   
    ROM_GPIOPinWrite(GPIO_PORTF_BASE, LED_RED | LED_GREEN | LED_BLUE, color);
    ROM_SysCtlDelay(5000000);
}

int main()
{
    ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, LED_RED | LED_BLUE | LED_GREEN);
    ROM_GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, SW1);
    ROM_GPIOPadConfigSet(GPIO_PORTF_BASE, SW1, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

    while(true) {
        blink(LED_BLUE);
        if(is_sw1_pushed())
            blink(LED_GREEN);
        else
            blink(LED_RED);
    }
}
