/* delay.c */
#include "MS51_16K.H"
#include "delay.h"

static void delay_1ms(void);

void Delay_init(void)
{
	TMOD |= (1 << 0);
	TMOD &= ~(1 << 1);
	CKCON |= (1 << 3);
	TMOD &= ~(1 << 2);
	TMOD &= ~(1 << 3);
}

void Delay_ms(uint16_t u16Delay)
{
	while (u16Delay) {
		--u16Delay;
		delay_1ms();
	}
}

void delay_1ms(void)
{
	TH0 = 0;
	TL0 = 0;
	TR0 = 1;
	while (TH0 * 256 + TL0 < 16000) {
	}
	TR0 = 0;
}
/**
  @}
*/

/**
  @}
*/

/**
  @}
*/
/******************** (C) COPYRIGHT 2022 xxx.com *****END OF FILE************/