#ifndef __TIMER_H__
#define __TIMER_H__

/* if define TIMER1_FSYS, timer = (0xFFFF-0xC147)/16MHz = 1.0ms */
#define TH1_INIT        0xC4 
#define TL1_INIT        0x17

void TIM1_init(void);
	
#endif
