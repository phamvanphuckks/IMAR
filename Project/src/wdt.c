#include "MS51_16K.H"
#include "wdt.h"

void WDT_init(void)
{	
	/*
	 * Note: we have to enable the WDT hardware either through In-Application Programming (IAP) 
	 * or by configuration bits during programming.
	 */
	set_IAPEN;
	IAPAL = 0x04;
	IAPAH = 0x00;
	IAPFD = 0x0F;
	IAPCN = BYTE_PROGRAM_CONFIG;
	set_CFUEN;
	
	set_IAPGO;                                  //trigger IAP
	while((CHPCON&SET_BIT6)==SET_BIT6);         //check IAPFF (CHPCON.6)
	
	clr_CFUEN;
	clr_IAPEN;
	
	TA = 0xAA;
	TA = 0x55;
	WDCON = 0x07;                							  //Setting WDT prescale 1/256 => 1.638 s
	
	set_WDCON_WDCLR;                            //Clear WDT timer
	while((WDCON | ~SET_BIT6) == 0xFF);         //confirm WDT clear is ok before into power down mode
	set_EA;
  set_WDCON_WDTR;                             //WDT run
}
