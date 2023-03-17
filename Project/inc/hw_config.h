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

#define DS1307_HOURS_START_ADDRESS              0x3020 
#define DS1307_MINUTE_START_ADDRESS             0x3021 
#define DS1307_SECOND_START_ADDRESS             0x3022
#define DS1307_DATE_START_ADDRESS               0x3023 
#define DS1307_MONTH_START_ADDRESS              0x3024 
#define DS1307_YEAR_START_ADDRESS               0x3025 

#define DS1307_HOURS_STOP_ADDRESS               0x3030 
#define DS1307_MINUTE_STOP_ADDRESS              0x3031 
#define DS1307_SECOND_STOP_ADDRESS              0x3032 
#define DS1307_DATE_STOP_ADDRESS                0x3033 
#define DS1307_MONTH_STOP_ADDRESS               0x3034 
#define DS1307_YEAR_STOP_ADDRESS                0x3035 

#define DS1307_TIMER_FLAG                       0x3040
#define DS1307_TIME_MODE_FLAG                  0x3041

#define FREQ_SYS	  16000000
#endif
