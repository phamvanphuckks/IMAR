#include "MS51_16K.H"
#include "gpio.h"
#include "hw_config.h"

void GPIO_init(void)
{
	  /* Set up Px As input mode */
    P00_INPUT_MODE; // COS
    P10_INPUT_MODE; // PAS
	
    /* Set up Px As output mode */
    P11_PUSHPULL_MODE; // Relay 1, LED1
	  P12_PUSHPULL_MODE; // Relay 2, LED2

    /* Set up Px As Quasi */
    P13_QUASI_MODE;  // SCL
    P14_QUASI_MODE;  // SDA
	
	  P06_QUASI_MODE; // UART_TX
}
