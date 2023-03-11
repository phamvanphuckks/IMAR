/**
  ******************************************************************************************
  *  @file    timer.h
  *  @author  phucpv
  *  @version 1.0
  *  @date    15/06/2022
  *  @brief   timer for MS51FB9AE.
  ******************************************************************************************
  *  @attention
  *  1. Any commercial product related to this source code is required to specify the source.
  *  <h2><center>&copy; COPYRIGHT 2022 xxx.com </center></h2>
  ******************************************************************************************
*/
#include "MS51_16K.H"
#include "timerx.h"

/* TIMER1 Init */
void TIM1_init(void)
{
	ENABLE_TIMER1_MODE1;
	TIMER1_FSYS;

	TH1 = TH1_INIT;
	TL1 = TL1_INIT;

	ENABLE_TIMER1_INTERRUPT;                       //enable Timer1 interrupt
	ENABLE_GLOBAL_INTERRUPT;                       //enable interrupts

	set_TCON_TR1;                                  //Timer1 run
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