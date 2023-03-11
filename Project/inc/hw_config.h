#ifndef __HW_CONFIG_H__
#define __HW_CONFIG_H__

/********************************************//**
 *   User defines
 ***********************************************/
#define BTN1_COS					        P00
#define BTN2_PAS						      P10
#define LED_RELAY1							  P11
#define LED_RELAY2							  P12	

#define LAST_LED_RELAY1	          0x3000
#define LAST_LED_RELAY2	          0x3004
#define STATE_MODE	              0x3008
#define STATE_ALARM_MODE	        0x300C

#define DS1307_HOURS_NOW_ADDRESS              0x3010 
#define DS1307_MINUTE_NOW_ADDRESS             0x3014 
#define DS1307_SECOND_NOW_ADDRESS             0x3018 

#define DS1307_HOURS_SETUP_ADDRESS              0x3020 
#define DS1307_MINUTE_SETUP_ADDRESS             0x3024 
#define DS1307_SECOND_SETUP_ADDRESS             0x3028 

#define FREQ_SYS	  24000000
#endif
